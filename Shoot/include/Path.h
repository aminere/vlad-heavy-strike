/* 

Amine Rehioui
Created: July 28th 2011

*/

#ifndef _PATH_H_INCLUDED_
#define _PATH_H_INCLUDED_

#include "PathElement.h"

namespace shoot
{
	//! Path entity
	class Path : public Entity3D				 
	{
	public:

		//! Macro to provide type information
		DECLARE_OBJECT(Path, Entity3D);

		//! Constructor
		Path();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! adds a child to the entity
		virtual void AddChild(Entity* pChild, bool bImmediate = false);

		//! returns a position along the path
		/** \param fRatio: factor from 0.0f to 1.0f */
		Vector3 GetPosition(f32 fRatio, s32* pSrcElementIndex = NULL);

		//! returns a position along the path
		Vector3 GetPosition(s32 srcIndex, s32 destIndex, f32 fLocalRatio);

		//! returns a path element
		PathElement* GetElement(u32 index);

		//! returns wrap around status
		bool IsWrapAround() const { return m_bWrapAround; }
			
	protected:
		
		// properties
		bool m_bWrapAround;
	};
}

#endif // _PATH_H_INCLUDED_

