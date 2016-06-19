/* 

Amine Rehioui
Created: August 14th 2011

*/

#include "Shoot.h"

#include "CameraPath.h"

namespace shoot
{
	DEFINE_OBJECT(CameraPath);

	//! adds a child to the entity
	void CameraPath::AddChild(Entity* pChild, bool bImmediate /*= false*/)
	{
		SHOOT_ASSERT(pChild->IsA(CameraPathElement::TypeID), "CameraPath::AddChild: child is not of type CameraPathElement");
		super::AddChild(pChild, bImmediate);
	}

	//! returns a camera look at position along the path
	/** \param fRatio: factor from 0.0f to 1.0f */
	Vector3 CameraPath::GetLookAt(f32 fRatio)
	{
		s32 numElements = GetChildCount();
		s32 srcIndex;
		s32 destIndex;
		f32 fFraction;
		if(m_bWrapAround)
		{
			f32 fIndex = fRatio*f32(numElements);
			s32 iIndex = s32(Math::Floor(fIndex));
			srcIndex = Math::Clamp<s32>(iIndex, 0, numElements-1);
			destIndex = (srcIndex+1)%numElements;
			fFraction = (iIndex == numElements) ? 1.0f : Math::Fraction(fIndex);
		}
		else
		{
			f32 fIndex = fRatio*f32(numElements-1);
			srcIndex = Math::Clamp<s32>(s32(Math::Floor(fIndex)), 0, numElements-1);
			destIndex = Math::Clamp<s32>(srcIndex+1, 0, numElements-1);
			fFraction = Math::Fraction(fIndex);
		}

		return GetLookAt(srcIndex, destIndex, fFraction);
	}

	//! returns a camera look at position along the path
	Vector3 CameraPath::GetLookAt(s32 srcIndex, s32 destIndex, f32 fLocalRatio)
	{
		s32 numElements = GetChildCount();
		s32 preSrcIndex;
		s32 postDestIndex;

		if(m_bWrapAround)
		{
			preSrcIndex = (srcIndex > 0) ? (srcIndex-1) : (numElements-1);
			postDestIndex = (destIndex+1)%numElements;
		}
		else
		{
			preSrcIndex = Math::Clamp<s32>(srcIndex-1, 0, numElements-1);
			postDestIndex = Math::Clamp<s32>(destIndex+1, 0, numElements-1);
		}

		Vector3 p1 = GetElement(srcIndex)->GetCameraLookAt();
		Vector3 p2 = GetElement(destIndex)->GetCameraLookAt();
		Vector3 p0 = GetElement(preSrcIndex)->GetCameraLookAt();
		Vector3 p3 = GetElement(postDestIndex)->GetCameraLookAt();
		return Math::CatmullRomInterpolate(p1, p2, p0, p3, fLocalRatio);
	}

	//! returns a path element
	CameraPathElement* CameraPath::GetElement(u32 index)
	{
		return static_cast<CameraPathElement*>(GetChild(index));
	}
}
