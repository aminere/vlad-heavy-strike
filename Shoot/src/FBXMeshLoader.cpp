/* 

Amine Rehioui
Created:  July 17th 2013

*/

#include "Shoot.h"

#include "FBXMeshLoader.h"

#include "Utils.h"

#include "File.h"

#ifndef SHOOT_FILE_SYSTEM
#include "fbxsdk.h"
#endif // SHOOT_FILE_SYSTEM

namespace shoot
{
	// statics
	fbxsdk::FbxManager* FBXMeshLoader::ms_pManager = NULL;

	//! loads a mesh from a file
	void FBXMeshLoader::Load(const char* strPath)
	{
#ifdef SHOOT_FILE_SYSTEM
		m_pMesh->LoadFS(strPath);
#else
		m_strPath = strPath;

		if(!ms_pManager)
		{
			ms_pManager = FbxManager::Create();
			FbxIOSettings* pIOs = FbxIOSettings::Create(ms_pManager, IOSROOT);
			ms_pManager->SetIOSettings(pIOs);
		}
		
		FbxScene* pScene = FbxScene::Create(ms_pManager, "Scene");
		int lFileMajor, lFileMinor, lFileRevision;
		int lSDKMajor,  lSDKMinor,  lSDKRevision;		
		FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

		FbxImporter* lImporter = FbxImporter::Create(ms_pManager, "");
		std::string path = std::string("data/") + strPath;
		bool lImportStatus = lImporter->Initialize(path.c_str(), -1, ms_pManager->GetIOSettings());
		lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);
		if( !lImportStatus )
		{
			FbxString error = lImporter->GetStatus().GetErrorString();
			SHOOT_ASSERT(false, "FBX Error: %s", error.Buffer());
			if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
			{
				SHOOT_ASSERT(false, "FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
				SHOOT_ASSERT(false, "FBX file format version for file '%s' is %d.%d.%d\n\n", strPath, lFileMajor, lFileMinor, lFileRevision);
			}
			return;
		}
		bool lStatus = lImporter->Import(pScene);
		SHOOT_ASSERT(lStatus, "FBX import error");		
		lImporter->Destroy();

		std::vector<MeshResource::SubMesh>& aSubMeshes = m_pMesh->GetSubMeshes();
		m_aUnusedSubMeshes = aSubMeshes;

        LoadMeshes(pScene->GetRootNode());
		
		// Remove unused VertexBuffers
		for(; !m_aUnusedSubMeshes.empty(); m_aUnusedSubMeshes.erase(m_aUnusedSubMeshes.begin()))
		{
			std::vector<MeshResource::SubMesh>::iterator it = std::find(aSubMeshes.begin(), aSubMeshes.end(), m_aUnusedSubMeshes[0]);			
			aSubMeshes.erase(it);
		}
				
		pScene->Destroy();
#endif // SHOOT_FILE_SYSTEM
	}

#ifndef SHOOT_FILE_SYSTEM
	//! Load meshes
	void FBXMeshLoader::LoadMeshes(FbxNode* pNode)
	{
		FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
		if(lNodeAttribute)
		{
			FbxNodeAttribute::EType type = lNodeAttribute->GetAttributeType();
			const char* strName = pNode->GetName();
			if(type == FbxNodeAttribute::eMesh)
			{
				FbxMesh* pMesh = pNode->GetMesh();
				if(!pMesh->IsTriangleMesh())
				{
					FbxGeometryConverter converter(ms_pManager);
					converter.Triangulate(pMesh, false);
					pMesh = pNode->GetMesh();
					if (!pMesh->IsTriangleMesh())
					{
						SHOOT_ASSERT(false, "mesh '%s': FBX triangulation failed, please triangulate in Max/Maya", pNode->GetName());
						return;
					}
				}
				if(pMesh->IsTriangleMesh())
				{
					// Look for an existing VertexBuffer
					std::vector<MeshResource::SubMesh>& aSubMeshes = m_pMesh->GetSubMeshes();
					s32 subMeshIndex = -1;
					for(u32 i=0; i<aSubMeshes.size(); ++i)
					{
						if(aSubMeshes[i].m_VertexBuffer->GetName() == strName)
						{
							subMeshIndex = i;
							m_aUnusedSubMeshes.erase(std::find(m_aUnusedSubMeshes.begin(), m_aUnusedSubMeshes.end(), aSubMeshes[i]));
							break;
						}
					}
					
					// get the position
					Vector3 vPosition = Vector3::Create(f32(pNode->LclTranslation.Get()[0]), -f32(pNode->LclTranslation.Get()[2]), f32(pNode->LclTranslation.Get()[1]));

					if(subMeshIndex == -1)
					{
						// Create a new sub mesh
						MeshResource::SubMesh subMesh = { Reference<VertexBuffer>(GraphicsDriver::Instance()->CreateVertexBuffer()), vPosition };
						m_pMesh->AddSubMesh(subMesh);
						subMeshIndex = aSubMeshes.size()-1;
					}
					
					aSubMeshes[subMeshIndex].m_VertexBuffer->SetName(strName);
					aSubMeshes[subMeshIndex].m_vPosition = vPosition;
					u32 numIndices = pMesh->GetPolygonCount()*3;
					u32 numVertices = numIndices;
					Vertex3D* pVertices = snew Vertex3D[numVertices];
					u16* pIndices = snew u16[numIndices];

					// extract indices
					u32 currentIndex = 0;
					for(s32 i=0; i<pMesh->GetPolygonCount(); ++i)
					{
						u32 numPolygonVtx = pMesh->GetPolygonSize(i);
						SHOOT_ASSERT(numPolygonVtx == 3, "mesh '%s': Found non-triangular polygon in a supposedly triangulated mesh", strName);
						u32 idx[3] = { 0, 2, 1 }; // shoot needs clock-wise triangles
						for (u32 j=0; j<numPolygonVtx; ++j)
						{
							u32 index = pMesh->GetPolygonVertex(i, idx[j]);
							FbxVector4 vtx = pMesh->GetControlPoints()[index];
							pVertices[currentIndex].Pos = Vector3::Create(f32(vtx[0]), f32(vtx[1]), f32(vtx[2]));
							pIndices[currentIndex] = currentIndex;
							currentIndex++;
						}
					}

					// extract UVs
					if(FbxGeometryElementUV* pUVElement = pMesh->GetElementUV())
					{
						if(pUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex)
						{
							s32 result = pUVElement->RemapIndexTo(FbxGeometryElement::eByPolygonVertex);
							SHOOT_ASSERT(result == 1, "mesh '%s': UVElement should have PolygonVertex mapping (and failed to change it). Current mapping: %d", strName, pUVElement->GetMappingMode());
						}

						bool bIndexed = pUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
						s32 count = bIndexed ? pUVElement->GetIndexArray().GetCount() : pUVElement->GetDirectArray().GetCount();
						s32 dir[3] = { 0, 1, -1 }; // hack because vertices were shuffled to make clock-wise triangles (see above)
						for(s32 i=0; i<count && i<s32(numVertices); ++i)
						{
							u32 localIdx = i%3;
							s32 idx = i + dir[localIdx];
							u32 uvIndex = bIndexed ? pUVElement->GetIndexArray().GetAt(idx) : idx;
							FbxVector2 uv = pUVElement->GetDirectArray().GetAt(uvIndex);
							pVertices[i].UV = Vector2(f32(uv[0]), 1.0f-f32(uv[1]));
						}
						aSubMeshes[subMeshIndex].m_VertexBuffer->SetVertexFlag(Vertex3D::VF_UV);
					}

					// vertex colors
					if(FbxGeometryElementVertexColor* pVtxColor = pMesh->GetElementVertexColor())
					{
						if(pVtxColor->GetMappingMode() != FbxGeometryElement::eByPolygonVertex)
						{
							s32 result = pVtxColor->RemapIndexTo(FbxGeometryElement::eByPolygonVertex);
							SHOOT_ASSERT(result == 1, "mesh '%s': VertexColor should have PolygonVertex mapping (and failed to change it). Current mapping: %d", strName, pVtxColor->GetMappingMode());
						}

						bool bIndexed = pVtxColor->GetReferenceMode() != FbxGeometryElement::eDirect;
						s32 count = bIndexed ? pVtxColor->GetIndexArray().GetCount() : pVtxColor->GetDirectArray().GetCount();
						s32 dir[3] = { 0, 1, -1 }; // hack because vertices were shuffled to make clock-wise triangles (see above)
						for(s32 i=0; i<count && i<s32(numVertices); ++i)
						{
							u32 localIdx = i%3;
							s32 idx = i + dir[localIdx];
							u32 colorIndex = bIndexed ? pVtxColor->GetIndexArray().GetAt(idx) : idx;
							FbxColor fbxColor = pVtxColor->GetDirectArray().GetAt(colorIndex);
							pVertices[i].color = Color(f32(fbxColor[0]), f32(fbxColor[1]), f32(fbxColor[2]), f32(fbxColor[3]));
						}
						aSubMeshes[subMeshIndex].m_VertexBuffer->SetVertexFlag(Vertex3D::VF_Color);
					}

					aSubMeshes[subMeshIndex].m_VertexBuffer->SetVertices(pVertices, numVertices);
					aSubMeshes[subMeshIndex].m_VertexBuffer->SetIndices(pIndices, numIndices);
					aSubMeshes[subMeshIndex].m_VertexBuffer->SetVertexFlag(Vertex3D::VF_Pos);

					// extract material info
					Material::CreationInfo materialInfo;
					if(FbxSurfaceMaterial *pMaterial = static_cast<FbxSurfaceMaterial*>(pNode->GetSrcObject(FbxCriteria::ObjectType(FbxSurfaceMaterial::ClassId))))
					{						
						FbxPropertyT<FbxColor> diffuse = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
						if(diffuse.IsValid())
						{
							FbxColor fbxColor = diffuse.Get();
							materialInfo.m_Color = Color(f32(fbxColor[0]), f32(fbxColor[1]), f32(fbxColor[2]), f32(fbxColor[3]));
						}

						u32 diffuseIndex = FbxLayerElement::eTextureDiffuse - FbxLayerElement::sTypeTextureStartIndex;
						if(FbxFileTexture* pTexture = pMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[diffuseIndex]).GetSrcObject<FbxFileTexture>(0))
						{
							std::string texturePath = pTexture->GetRelativeFileName();
							if(texturePath.length() > 0)
							{
								// determine texture path
								std::string meshPath = m_strPath;
								size_t dirIndex = meshPath.rfind("/");
								std::string basePath = meshPath.substr(0, dirIndex+1);
								texturePath = Utils::FormatResourcePath(basePath + texturePath);
								Material::CreationInfo::TextureInfo* pInfo = snew Material::CreationInfo::TextureInfo();
								pInfo->m_Path = texturePath;
								materialInfo.m_TextureInfos.Add(pInfo);
							}
						}
					}
					m_pMesh->AddMaterial(materialInfo);	
				}
				else
				{
					SHOOT_ASSERT(false, " mesh '%s': FBX triangulation failed, please triangulate in Max/Maya", strName);
				}
			}
		}

		s32 numChildren = pNode->GetChildCount();
		for (s32 i=0; i<numChildren; ++i)
		{
			LoadMeshes(pNode->GetChild(i));
		}				
	}
#endif // SHOOT_FILE_SYSTEM	

	//! static uninitialization
	void FBXMeshLoader::Uninit()
	{
#ifndef SHOOT_FILE_SYSTEM
		if(ms_pManager)
		{
			ms_pManager->Destroy();
			ms_pManager = NULL;
		}
#endif // SHOOT_FILE_SYSTEM
	}
}


