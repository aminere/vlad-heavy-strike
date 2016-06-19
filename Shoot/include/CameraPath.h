/* 

Amine Rehioui
Created: August 14th 2011

*/

#ifndef _CAMERA_PATH_H_INCLUDED_
#define _CAMERA_PATH_H_INCLUDED_

#include "Path.h"
#include "CameraPathElement.h"

namespace shoot
{
	//! Camera Path entity
	class CameraPath : public Path				 
	{
	public:

		//! Macro to provide type information
		DECLARE_OBJECT(CameraPath, Path);

		//! adds a child to the entity
		virtual void AddChild(Entity* pChild, bool bImmediate = false);

		//! returns a camera look at position along the path
		/** \param fRatio: factor from 0.0f to 1.0f */
		Vector3 GetLookAt(f32 fRatio);

		//! returns a camera look at position along the path
		Vector3 GetLookAt(s32 srcIndex, s32 destIndex, f32 fLocalRatio);

		//! returns a path element
		CameraPathElement* GetElement(u32 index);
	};
}

#endif // _CAMERA_PATH_H_INCLUDED_

