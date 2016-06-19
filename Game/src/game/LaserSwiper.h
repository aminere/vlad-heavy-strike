/* 

Amine Rehioui
Created: August 18th 2013

*/

#ifndef _LASER_SWIPER_H_INCLUDED_
#define _LASER_SWIPER_H_INCLUDED_

namespace shoot
{
	//! LaserSwiperSettings
	class LaserSwiperSettings : public ActorSettings
	{
		DECLARE_OBJECT(LaserSwiperSettings, ActorSettings);

	public:

		//! constructor
		LaserSwiperSettings();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		f32 m_fSpeed;
		f32 m_fAimDuration;
		f32 m_fLaserThickness;
		f32 m_fLaserOffset;
		f32 m_fLaserSpeed;
		f32 m_fLaserLength;
	};

	//! LaserSwiper
	class LaserSwiper : public Actor
	{
	public:
		
		DECLARE_OBJECT(LaserSwiper, Actor);

		//! constructor
		LaserSwiper();

		//! destructor
		virtual ~LaserSwiper();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity		
		virtual void Update();

	private:

		Laser* m_pLaser;
		f32 m_fCurrentAngle;
		f32 m_fLaserLengthSquared;
		f32 m_fSwipeTimer;
		f32 m_fMoveTimer;
	};
}

#endif // _LASER_SWIPER_H_INCLUDED_


