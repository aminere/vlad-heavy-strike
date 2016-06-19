/* 

Amine Rehioui
Created: October 8th 2012

*/

#include "Shoot.h"

#include "Light.h"

namespace shoot
{
	DEFINE_OBJECT(Light);

	//! Constructor
	Light::Light()	
	{		
	}

	//! serializes the entity to/from a PropertyStream
	void Light::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
	}

	//! called during the initialization of the entity
	void Light::Init()
	{
		super::Init();
	}	
}

