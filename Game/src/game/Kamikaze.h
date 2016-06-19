/* 

Amine Rehioui
Created: July 14th 2013

*/

#ifndef _KAMIKAZE_H_INCLUDED_
#define _KAMIKAZE_H_INCLUDED_

#include "BasicEnemy.h"

namespace shoot
{
	//! Kamikaze settings
	class KamikazeSettings : public BasicEnemySettings
	{
		DECLARE_OBJECT(KamikazeSettings, BasicEnemySettings);

	public:

		//! constructor
		KamikazeSettings();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		f32 m_fDuration;
	};

	//! Kamikaze
	class Kamikaze : public Actor
	{
	public:
		
		DECLARE_OBJECT(Kamikaze, Actor);

		//! constructor
		Kamikaze();

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();

	private:

		Vector3 m_vDirection;
		f32 m_fTimer;
	};
}

#endif // _KAMIKAZE_H_INCLUDED_


