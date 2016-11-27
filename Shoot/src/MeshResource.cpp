/* 

Amine Rehioui
Created: December 28th 2010

*/

#include "Shoot.h"

#include "MeshResource.h"

#include "File.h"

#include "Utils.h"

// resource loaders
#include "MS3DMeshLoader.h"
#include "XMeshLoader.h"
#include "FBXMeshLoader.h"

namespace shoot
{
	DEFINE_OBJECT(MeshResource);

	//! constructor
	MeshResource::MeshResource()
		: m_bBoundingBoxesDirty(true)
		// properties
		, m_bDynamic(false)
	{
	}

	//! destructor
	MeshResource::~MeshResource()
	{
	}

	//! serializes the font from a stream
	void MeshResource::Serialize(PropertyStream& stream)
	{
		stream.Serialize(PT_Bool, "Dynamic", &m_bDynamic);
		bool bChanged = stream.Serialize(PT_File, "MeshPath", &m_MeshPath);

		if(bChanged)
		{
			ResourceInit();
		}
	}

	//! initializes this resource
	void MeshResource::ResourceInit()
	{
		// determine the extension and pick appropriate loader
		s32 extIndex = m_MeshPath.rfind(".");
		SHOOT_ASSERT(extIndex >= 0, "MeshResource invalid path");
		std::string extension = m_MeshPath.substr(extIndex+1);

		m_MaterialCreationInfos.clear();

#ifndef SHOOT_EDITOR
		LoadFS(m_MeshPath.c_str());
#else
		{
			if (Utils::Equals(extension.c_str(), "ms3d"))
			{
				MS3DMeshLoader(this).Load(m_MeshPath.c_str());
			}
			else if (Utils::Equals(extension.c_str(), "x"))
			{
				XMeshLoader(this).Load(m_MeshPath.c_str());
			}
			else if (Utils::Equals(extension.c_str(), "fbx"))
			{
				FBXMeshLoader(this).Load(m_MeshPath.c_str());
			}
			else
			{
				SHOOT_ASSERT(false, "Unsupported mesh format: '%s'", m_MeshPath.c_str());
			}
		}
#endif
		
		for(u32 i=0; i<m_SubMeshes.size(); ++i)
		{
			m_SubMeshes[i].m_VertexBuffer->SetDynamic(m_bDynamic);
		}
	}

	//! returns the bounding box
	const AABBox3D& MeshResource::GetBoundingBox()
	{
		if(m_bBoundingBoxesDirty)
		{
			m_BoundingBoxes.clear();

			for(u32 i=0; i<m_SubMeshes.size(); ++i)
			{
				const VertexBuffer* pVertexBuffer = m_SubMeshes[i].m_VertexBuffer.Get();
				Vector3 MinVertex = Vector3::One * Math::Maxf32;
				Vector3 MaxVertex = -Vector3::One * Math::Maxf32;

				for(u32 j=0; j<pVertexBuffer->GetNumIndices(); ++j)
				{
					Vertex3D vertex = pVertexBuffer->GetVertices()[pVertexBuffer->GetIndices()[j]];

					if(vertex.Pos.X < MinVertex.X) MinVertex.X = vertex.Pos.X;
					else if(vertex.Pos.X > MaxVertex.X) MaxVertex.X = vertex.Pos.X;

					if(vertex.Pos.Y < MinVertex.Y) MinVertex.Y = vertex.Pos.Y;
					else if(vertex.Pos.Y > MaxVertex.Y) MaxVertex.Y = vertex.Pos.Y;

					if(vertex.Pos.Z < MinVertex.Z) MinVertex.Z = vertex.Pos.Z;
					else if(vertex.Pos.Z > MaxVertex.Z) MaxVertex.Z = vertex.Pos.Z;	
				}

				m_BoundingBoxes.push_back(AABBox3D(MinVertex, MaxVertex));
				m_BoundingBox = (i == 0) ? m_BoundingBoxes[i] : (m_BoundingBox + m_BoundingBoxes[i]);				
			}

			m_bBoundingBoxesDirty = false;
		}

		return m_BoundingBox;
	}

	//! returns the bounding box
	const AABBox3D& MeshResource::GetBoundingBox(u32 subMeshIndex /*= 0*/)
	{
		GetBoundingBox();
		return m_BoundingBoxes[subMeshIndex];
	}

	//! Load from the shoot file system
	void MeshResource::LoadFS(const char* strPath)
	{
		File* pFile = File::Create(strPath, File::M_ReadBinary);
		pFile->Open();
		u32 numSubMeshes = 0;
		pFile->Read(&numSubMeshes, sizeof(u32));
		for(u32 i=0; i<numSubMeshes; ++i)
		{
			u32 numVertices = 0, numIndices = 0, flags = 0;
			Vector3 vPosition;
			pFile->Read(vPosition.v, sizeof(Vector3));
			pFile->Read(&numVertices, sizeof(u32));
			pFile->Read(&numIndices, sizeof(u32));
			pFile->Read(&flags, sizeof(u32));

			u32 nameLength = 0;
			pFile->Read(&nameLength, sizeof(u32));
			SHOOT_ASSERT(nameLength < 512, "nameLength too long");
			char name[512];
			pFile->Read(name, nameLength);
			name[nameLength] = '\0';
			std::string strName(name);

			// Create a new sub mesh only if not existing yet
			s32 subMeshIndex = -1;
			for(u32 i=0; i<m_SubMeshes.size(); ++i)
			{
				if(m_SubMeshes[i].m_VertexBuffer->GetName() == strName)
				{
					subMeshIndex = i;
					break;
				}
			}

			if(subMeshIndex == -1)
			{
				MeshResource::SubMesh subMesh = { Reference<VertexBuffer>(GraphicsDriver::Instance()->CreateVertexBuffer()), vPosition };
				m_SubMeshes.push_back(subMesh);
				subMeshIndex = m_SubMeshes.size()-1;
			}
			
			m_SubMeshes[subMeshIndex].m_VertexBuffer->SetName(strName);
			u16* pIndices = snew u16[numIndices];
			Vertex3D* pVertices = snew Vertex3D[numVertices];
			pFile->Read(pIndices, numIndices*sizeof(u16));
			pFile->Read(pVertices, numVertices*sizeof(Vertex3D));
			m_SubMeshes[subMeshIndex].m_VertexBuffer->SetVertices(pVertices, numVertices);
			m_SubMeshes[subMeshIndex].m_VertexBuffer->SetIndices(pIndices, numIndices);
			m_SubMeshes[subMeshIndex].m_VertexBuffer->SetVertexFlags(flags);

			Material::CreationInfo materialInfo;
			pFile->Read(&materialInfo.m_Color, sizeof(Color));
			u32 texturePathLength = 0;
			pFile->Read(&texturePathLength, sizeof(u32));
			if(texturePathLength > 0)
			{
				SHOOT_ASSERT(texturePathLength < 512, "texturePathLength too long");
				char texturePath[512];
				pFile->Read(texturePath, texturePathLength);
				texturePath[texturePathLength] = '\0';
				bool bMipMaps = true;
				pFile->Read(&bMipMaps, sizeof(bool));
				Material::CreationInfo::TextureInfo* pInfo = snew Material::CreationInfo::TextureInfo();
				pInfo->m_Path = texturePath;
				pInfo->m_bMipMaps = bMipMaps;
				materialInfo.m_TextureInfos.Add(pInfo);
			}
			AddMaterial(materialInfo);
		}
		pFile->Close();
		delete pFile;
	}
}
