/* 

Amine Rehioui
Created: June 22nd 2013

*/

#ifndef _FALLING_ASTEROID_H_INCLUDED_
#define _FALLING_ASTEROID_H_INCLUDED_

#include "Asteroid.h"

namespace shoot
{
	//! FallingAsteroidSettings
	class FallingAsteroidSettings : public AsteroidSettings
	{
		DECLARE_OBJECT(FallingAsteroidSettings, AsteroidSettings);

	public:

		//! constructor
		FallingAsteroidSettings();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		f32 m_fHeight;
		f32 m_fDestOffset;
	};

	//! FallingAsteroid
	class FallingAsteroid : public Asteroid
	{
	public:
		
		DECLARE_OBJECT(FallingAsteroid, Asteroid);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();

	};
}

#endif // _FALLING_ASTEROID_H_INCLUDED_


