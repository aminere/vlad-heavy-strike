/* 

Amine Rehioui
Created: December 28th 2010

*/

#ifndef _MESH_RESOURCE_H_INCLUDED_
#define _MESH_RESOURCE_H_INCLUDED_

#include "Utils.h"

namespace shoot
{
	//! Mesh Resource
	class MeshResource : public Resource
	{
		DECLARE_OBJECT(MeshResource, Resource);

	public:

		//! SubMesh
		struct SubMesh
		{
			Reference<VertexBuffer> m_VertexBuffer;
			Vector3 m_vPosition;

			//! comparison operator
			bool operator == (const SubMesh& other) const
			{
				return m_VertexBuffer == other.m_VertexBuffer;
			}
		};

		//! constructor
		MeshResource();

		//! destructor
		~MeshResource();

		//! serializes the font from a stream
		void Serialize(PropertyStream& stream);

		//! Load from disk
		void ResourceInit();

		//! Adds a SubMesh
		inline void AddSubMesh(SubMesh& subMesh) { m_SubMeshes.push_back(subMesh); }

		//! Adds a material
		inline void AddMaterial(const Material::CreationInfo& info) { m_MaterialCreationInfos.push_back(info); }

		//! returns the SubMesh infos
		inline std::vector<SubMesh>& GetSubMeshes() { return m_SubMeshes; }

		//! returns the material creation infos
		inline const std::vector<Material::CreationInfo>& GetMaterialInfos() const { return m_MaterialCreationInfos; }

		//! returns true if this resource is using the specified data
		virtual bool IsUsingData(const char* strPath) const
		{ 
			return Utils::Equals(m_MeshPath.c_str(), strPath);
		}

		//! returns the bounding box
		const AABBox3D& GetBoundingBox();

		//! returns the bounding box per submesh
		const AABBox3D& GetBoundingBox(u32 subMeshIndex);

		//! returns the mesh path
		inline const std::string& GetMeshPath() const { return m_MeshPath; }

		//! Load from the shoot file system
		void LoadFS(const char* strPath);

	private:
		
		std::vector<SubMesh> m_SubMeshes;
		std::vector<Material::CreationInfo> m_MaterialCreationInfos;
		AABBox3D m_BoundingBox;
		std::vector<AABBox3D> m_BoundingBoxes;
		bool m_bBoundingBoxesDirty;

		// properties
		bool m_bDynamic;
		std::string m_MeshPath;		
	};
}

#endif // _MESH_RESOURCE_H_INCLUDED_

