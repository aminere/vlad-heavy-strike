/*	

Amine Rehioui
Created: October 20th 2013

*/

#include "ShootTest.h"

#include "DamageComponent.h"

namespace shoot
{
	DEFINE_OBJECT(DamageComponent);
	
	//! constructor
	DamageComponent::DamageComponent()
		// properties
		: m_fDamagePerSecond(35.0f)
		, m_bExplodeOnContact(false)
	{
	}

	//! Reads/Writes struct properties from/to a stream
	void DamageComponent::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		stream.Serialize(PT_Float, "DamagePerSecond", &m_fDamagePerSecond);
		stream.Serialize(PT_Bool, "ExplodeOnContact", &m_bExplodeOnContact);		
	}	
}

