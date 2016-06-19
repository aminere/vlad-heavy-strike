/* 

Amine Rehioui
Created: August 3rd 2011

*/

#include "Shoot.h"

#include "PathElement.h"

namespace shoot
{
	DEFINE_OBJECT(PathElement);

	//! Constructor
	PathElement::PathElement()
		: m_vUpVector(Vector3::Create(0.0f, 0.0f, 1.0f))
		, m_fFollowDuration(1.0f)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void PathElement::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		stream.Serialize(PT_Vec3, "UpVector", &m_vUpVector);
		stream.Serialize(PT_Float, "FollowDuration", &m_fFollowDuration);
	}
}

