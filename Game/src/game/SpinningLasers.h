/* 

Amine Rehioui
Created: July 26th 2013

*/

#ifndef _SPINNING_LASERS_H_INCLUDED_
#define _SPINNING_LASERS_H_INCLUDED_

namespace shoot
{
	//! SpinningLasers settings
	class SpinningLasersSettings : public ActorSettings
	{
		DECLARE_OBJECT(SpinningLasersSettings, ActorSettings);

	public:

		//! constructor
		SpinningLasersSettings();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		f32 m_fRotationSpeed;
		f32 m_fShootFrequency;
		f32 m_fLaserDuration;
		f32 m_fLaserOffset;
		f32 m_fLaserSpeed;
		f32 m_fLaserLength;
		f32 m_fDisappearOffset;
		f32 m_fHaltTime;
		f32 m_Damage;
	};

	//! SpinningLasers class
	class SpinningLasers : public Actor
	{
	public:
		
		DECLARE_OBJECT(SpinningLasers, Actor);

		//! constructor
		SpinningLasers();

		//! destructor
		virtual ~SpinningLasers();

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();

	private:

		f32 m_fLaserTimer;
		f32 m_fHaltTimer;
	};
}

#endif // _SPINNING_LASERS_H_INCLUDED_


