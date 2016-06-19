/* 

Amine Rehioui
Created: August 18th 2013

*/

#ifndef _BOMBER_H_INCLUDED_
#define _BOMBER_H_INCLUDED_

namespace shoot
{
	//! BomberSettings
	class BomberSettings : public ActorSettings
	{
		DECLARE_OBJECT(BomberSettings, ActorSettings);

	public:

		//! constructor
		BomberSettings();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);
	};

	//! Bomber
	class Bomber : public Actor
	{
	public:
		
		DECLARE_OBJECT(Bomber, Actor);

		//! constructor
		Bomber();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();		
	};
}

#endif // _BOMBER_H_INCLUDED_


