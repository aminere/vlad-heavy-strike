/* 

Amine Rehioui
Created: July 17th 2013

*/

#ifndef _FBX_MESH_LOADER_H_INCLUDED_
#define _FBX_MESH_LOADER_H_INCLUDED_

#include "MeshResource.h"

// forwards
namespace fbxsdk_2015_1
{
	class FbxManager;
	class FbxNode;
}

namespace shoot
{
	//! FBX mesh loader
	class FBXMeshLoader
	{
	public:

		//! constructor
		FBXMeshLoader(MeshResource* pMesh)
			: m_pMesh(pMesh)
		{
		}

		//! loads a mesh from a file
		void Load(const char* strPath);

		//! static uninitialization
		static void Uninit();

	private:

		//! Load meshes
		void LoadMeshes(fbxsdk_2015_1::FbxNode* pNode);
				
		MeshResource* m_pMesh;
		std::vector<MeshResource::SubMesh> m_aUnusedSubMeshes;
		std::string m_strPath;

		static fbxsdk_2015_1::FbxManager* ms_pManager;
	};
}

#endif // _FBX_MESH_LOADER_H_INCLUDED_

