/* 

Amine Rehioui
Created: December 12th 2010

*/

#ifndef _ANIMATED_MESH_ENTITY_H_INCLUDED_
#define _ANIMATED_MESH_ENTITY_H_INCLUDED_

#include "AnimatedMesh.h"

namespace shoot
{
	//! 3D Animated Mesh entity
	class AnimatedMeshEntity : public Entity3D
	{
	public:

		//! Macro to provide type information
		DECLARE_OBJECT(AnimatedMeshEntity, Entity3D);

		//! constructor
		AnimatedMeshEntity();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();
			
		//! Reloads the mesh entity given a mesh path
		void SetMesh(std::string strPath);

	private:

		Reference<AnimatedMesh> m_Mesh;

		// properties
		std::string m_strPath;
	};
}

#endif // _ANIMATED_MESH_ENTITY_H_INCLUDED_

