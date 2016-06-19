/* 

Amine Rehioui
Created: December 10th 2010

*/

#ifndef _ANIMATED_MESH_H_INCLUDED_
#define _ANIMATED_MESH_H_INCLUDED_

#include "MeshResource.h"
#include "Bone.h"

namespace shoot
{
	//! animated mesh class
	class AnimatedMesh : public MeshResource
	{
		DECLARE_OBJECT(AnimatedMesh, MeshResource);

	public:

		//! constructor
		AnimatedMesh();

		//! destructor
		virtual ~AnimatedMesh();

	private:

		std::vector<Bone*> m_aBones;
	};
}

#endif // _ANIMATED_MESH_H_INCLUDED_

