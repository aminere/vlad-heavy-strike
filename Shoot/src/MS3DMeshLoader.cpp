/* 

Amine Rehioui
Created: December 12th 2010

*/

#include "Shoot.h"

#include "MS3DMeshLoader.h"

#include "MS3DFile.h"

#include "Utils.h"

namespace shoot
{
	//! loads a mesh from a file
	void MS3DMeshLoader::Load(const char* strPath)
	{
		// NOTE: it is impossible to use a vertex/index buffer coupling properly with MS3D since vertex UVs are index dependent

		CMS3DFile ms3D;
		bool bLoaded = ms3D.LoadFromFile(strPath);
		SHOOT_ASSERT(bLoaded, "Could not load mesh '%s'", strPath);

		// TODO: support multiple materials
		Material::CreationInfo materialInfo;		

		// get the material 
		s32 numMaterials = ms3D.GetNumMaterials();
		SHOOT_ASSERT(numMaterials <= 1, "Only one mesh material supported for now");
		ms3d_material_t *pMS3DMaterial;
		ms3D.GetMaterialAt(0, &pMS3DMaterial);

		// determine texture path
		std::string meshPath = strPath;
		size_t dirIndex = meshPath.rfind("/");
		std::string basePath = meshPath.substr(0, dirIndex+1);		
		std::string texturePath = Utils::FormatResourcePath(basePath + std::string(pMS3DMaterial->texture));
		Material::CreationInfo::TextureInfo* pInfo = snew Material::CreationInfo::TextureInfo();
		pInfo->m_Path = texturePath;
		materialInfo.m_TextureInfos.Add(pInfo);		
		m_pMesh->AddMaterial(materialInfo);

		s32 numTriangles = ms3D.GetNumTriangles();
		s32 numVertices = numTriangles*3;
		s32 numIndices = numVertices;
		
		Vertex3D* pVertices = snew Vertex3D[numVertices];
		u16* pIndices = snew u16[numIndices];

		ms3d_triangle_t* pTriangle;
		ms3d_vertex_t* pMS3DVertices[3];
		for(s32 i=0; i<numTriangles; ++i)
		{
			ms3D.GetTriangleAt(i, &pTriangle);
			ms3D.GetVertexAt(pTriangle->vertexIndices[0], &pMS3DVertices[0]);
			ms3D.GetVertexAt(pTriangle->vertexIndices[1], &pMS3DVertices[1]);
			ms3D.GetVertexAt(pTriangle->vertexIndices[2], &pMS3DVertices[2]);

			u32 aVertexOrder[3] = { 0, 2, 1 };
			for(u32 j=0; j<3; ++j)
			{
				// get vertex information
				Vertex3D* pVertex = &pVertices[i*3 + j];
				pVertex->Pos = Vector3::Create(pMS3DVertices[aVertexOrder[j]]->vertex[0], // X
										pMS3DVertices[aVertexOrder[j]]->vertex[1], // Y
										pMS3DVertices[aVertexOrder[j]]->vertex[2]); // Z

				pVertex->UV = Vector2(pTriangle->s[aVertexOrder[j]], // U
									   pTriangle->t[aVertexOrder[j]]); // V
			}

			pIndices[(i*3) + 0] = (i*3) + 0;
			pIndices[(i*3) + 1] = (i*3) + 1;
			pIndices[(i*3) + 2] = (i*3) + 2;
		}
				
		VertexBuffer* pVertexBuffer = m_pMesh->GetSubMeshes().size() ? m_pMesh->GetSubMeshes()[0].m_VertexBuffer.Get() : GraphicsDriver::Instance()->CreateVertexBuffer();
		pVertexBuffer->SetVertices(pVertices, numVertices);
		pVertexBuffer->SetVertexFlag(Vertex3D::VF_Pos);
		pVertexBuffer->SetVertexFlag(Vertex3D::VF_UV);
		pVertexBuffer->SetName(strPath);

		pVertexBuffer->SetIndices(pIndices, numIndices);

		if(!m_pMesh->GetSubMeshes().size())
		{
			MeshResource::SubMesh subMesh = { Reference<VertexBuffer>(pVertexBuffer), Vector3::Zero };
			m_pMesh->AddSubMesh(subMesh);
		}
	}
}

