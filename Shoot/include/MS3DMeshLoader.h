/* 

Amine Rehioui
Created: December 12th 2010

*/

#ifndef _MS3D_MESH_LOADER_H_INCLUDED_
#define _MS3D_MESH_LOADER_H_INCLUDED_

#include "MeshResource.h"

namespace shoot
{
	//! MS3D mesh loader
	class MS3DMeshLoader
	{
	public:

		//! constructor
		MS3DMeshLoader(MeshResource* pMesh)
			: m_pMesh(pMesh)
		{
		}

		//! loads a mesh from a file
		void Load(const char* strPath);

	private:

		MeshResource* m_pMesh;
	};
}

#endif // _MS3D_MESH_LOADER_H_INCLUDED_

