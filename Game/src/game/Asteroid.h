/* 

Amine Rehioui
Created: January 04th 2013

*/

#ifndef _ASTEROID_H_INCLUDED_
#define _ASTEROID_H_INCLUDED_

#include "BasicEnemy.h"

#include "Range.h"

namespace shoot
{
	//! AsteroidSettings
	class AsteroidSettings : public ActorSettings
	{
		DECLARE_OBJECT(AsteroidSettings, ActorSettings);

	public:

		//! constructor
		AsteroidSettings();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		Range m_rSize;
		Range m_rHitPoints;
		Range m_rSpeed;
		f32 m_fAngleRange;
		f32 m_fMaxRotationOffset;
	};
		
	//! Asteroid class
	class Asteroid : public BasicEnemy
	{
	public:
		
		DECLARE_OBJECT(Asteroid, BasicEnemy);

		//! Constructor
		Asteroid();

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity
		virtual void Update();

	protected:

		Vector3 m_vSpeed;
		Vector3 m_vRotationOffset;
	};
}

#endif // _ASTEROID_H_INCLUDED_


