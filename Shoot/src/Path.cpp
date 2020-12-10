/* 

Amine Rehioui
Created: August 3rd 2011

*/

#include "Shoot.h"

#include "Path.h"

namespace shoot
{
	DEFINE_OBJECT(Path);

	//! Constructor
	Path::Path()
		// properties
		: m_bWrapAround(false)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void Path::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		stream.Serialize(PT_Bool, "WrapAround", &m_bWrapAround);
	}

	//! adds a child to the entity
	void Path::AddChild(Entity* pChild, bool bImmediate /*= false*/)
	{
		SHOOT_ASSERT(pChild->IsA(PathElement::TypeID), "Path::AddChild: child is not of type PathElement");
		super::AddChild(pChild, bImmediate);
	}

	//! returns a position along the path
	/** \param fRatio: factor from 0.0f to 1.0f */
	Vector3 Path::GetPosition(f32 fRatio, s32* pSrcElementIndex /*= NULL*/)
	{
		auto numElements = (int)GetChildCount();
		s32 srcIndex;
		s32 destIndex;
		f32 fFraction;

		if(m_bWrapAround)
		{
			fRatio = Math::Fraction(fRatio);
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

		if(pSrcElementIndex)
		{
			*pSrcElementIndex = srcIndex;
		}

		return GetPosition(srcIndex, destIndex, fFraction);
	}

	//! returns a position along the path
	Vector3 Path::GetPosition(s32 srcIndex, s32 destIndex, f32 fLocalRatio)
	{
		auto numElements = (int)GetChildCount();
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

		Vector3 p1 = GetElement(srcIndex)->GetPosition();
		Vector3 p2 = GetElement(destIndex)->GetPosition();
		Vector3 p0 = GetElement(preSrcIndex)->GetPosition();
		Vector3 p3 = GetElement(postDestIndex)->GetPosition();

		Matrix44 local = Matrix44::Identity;
		local.SetTranslation(Math::CatmullRomInterpolate(p1, p2, p0, p3, fLocalRatio));
		Matrix44 transformed = local * GetTransformationMatrix();
		return transformed.GetTranslation();
	}

	//! returns a path element
	PathElement* Path::GetElement(u32 index)
	{
		return static_cast<PathElement*>(GetChild(index));
	}
}

