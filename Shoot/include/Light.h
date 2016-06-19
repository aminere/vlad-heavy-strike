/* 

Amine Rehioui
Created: October 8th 2012

*/

#ifndef _LIGHT_H_INCLUDED_
#define _LIGHT_H_INCLUDED_

namespace shoot
{
	//! Light entity
	class Light : public Entity3D				 
	{
	public:

		//! Macro to provide type information
		DECLARE_OBJECT(Light, Entity3D);

		//! Constructor
		Light();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();			
	};
}

#endif // _LIGHT_H_INCLUDED_

