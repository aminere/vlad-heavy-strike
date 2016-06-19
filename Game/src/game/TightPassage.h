/* 

Amine Rehioui
Created: March 6th 2013

*/

#ifndef _TIGHT_PASSAGE_H_INCLUDED_
#define _TIGHT_PASSAGE_H_INCLUDED_

#include "Event.h"

namespace shoot
{
	//! TightPassage settings
	class TightPassageSettings : public ActorSettings
	{
		DECLARE_OBJECT(TightPassageSettings, ActorSettings);

	public:

		//! constructor
		TightPassageSettings();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		Vector3 m_vLaserOffset;
		f32 m_fLaserSpeed;
		f32 m_fLaserDuration;
	};

	//! TightPassage class
	class TightPassage : public Actor
	{
	public:
		
		DECLARE_OBJECT(TightPassage, Actor);

		//! constructor
		TightPassage();

		//! destructor
		virtual ~TightPassage();

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();

		//! shoot event listeners
		static void HandleTriggerEvent(void* pListener, Event* pEvent);

	private:

		static u32 m_sNumCannons;
		f32 m_fLaserTimer;

		std::vector<Entity3D*> m_aCannons;
	};
}

#endif // _TIGHT_PASSAGE_H_INCLUDED_


