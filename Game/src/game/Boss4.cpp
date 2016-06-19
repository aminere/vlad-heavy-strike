/* 

Amine Rehioui
Created: August 20th 2013

*/

#include "ShootTest.h"

#include "Boss4.h"

#include "Boss4FSM.h"

#include "Utils.h"

namespace shoot
{
	DEFINE_OBJECT(Boss4);

	//! constructor
	Boss4::Boss4()
		: m_bLaserPhase(true)
		// properties
		, m_fLaserThickness(1.0f)
		, m_fLaserOffset(9.0f)
		, m_fLaserSpeed(10.0f)
		, m_fLaserLength(3.0f)
		, m_fLaserFrequency(.5f)
		, m_fLaserLife(2.0f)
		, m_fLaserPhaseDelay(3.0f)
		, m_fLaserPhaseDuration(2.0f)
		
		, m_fMissileOffset(7.0f)
		, m_fMissileSpeed(10.0f)
		, m_fMissileFrequency(1.0f)
		, m_fMissileLife(3.0f)
		, m_fMissileSize(2.0f)
		, m_fMissileMinDist(10.0f)
		, m_fMissileHomingFactor(4.0f)
		, m_fMissileMaxHomingAngle(60.0f)
		, m_fMissilePhaseDelay(3.0f)
		, m_fMissilePhaseDuration(2.0f)

		, m_bDestroyWhenDone(false)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void Boss4::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Float, "LaserThickness", &m_fLaserThickness);
		stream.Serialize(PT_Float, "LaserOffset", &m_fLaserOffset);
		stream.Serialize(PT_Float, "LaserSpeed", &m_fLaserSpeed);
		stream.Serialize(PT_Float, "LaserLength", &m_fLaserLength);
		stream.Serialize(PT_Float, "LaserFrequency", &m_fLaserFrequency);
		stream.Serialize(PT_Float, "LaserLife", &m_fLaserLife);
		stream.Serialize(PT_Float, "LaserPhaseDelay", &m_fLaserPhaseDelay);
		stream.Serialize(PT_Float, "LaserPhaseDuration", &m_fLaserPhaseDuration);

		stream.Serialize(PT_Float, "MissileOffset", &m_fMissileOffset);
		stream.Serialize(PT_Float, "MissileSpeed", &m_fMissileSpeed);
		stream.Serialize(PT_Float, "MissileFrequency", &m_fMissileFrequency);
		stream.Serialize(PT_Float, "MissileLife", &m_fMissileLife);
		stream.Serialize(PT_Float, "MissileSize", &m_fMissileSize);
		stream.Serialize(PT_Float, "MissileMinDist", &m_fMissileMinDist);
		stream.Serialize(PT_Float, "MissileHomingFactor", &m_fMissileHomingFactor);
		stream.Serialize(PT_Float, "MissileMaxHomingAngle", &m_fMissileMaxHomingAngle);
		stream.Serialize(PT_Float, "MissilePhaseDelay", &m_fMissilePhaseDelay);
		stream.Serialize(PT_Float, "MissilePhaseDuration", &m_fMissilePhaseDuration);

		stream.Serialize(PT_Bool, "DestroyWhenDone", &m_bDestroyWhenDone);				
		stream.SerializeReference("Retreat", &m_Retreat);
	}

	//! called during the initialization of the entity
	void Boss4::Init()
	{
		m_pFSM = snew Boss4FSM(this);
		super::Init();
				
		for(u32 i=0; i<m_sNumLaserCannons; ++i)
		{
			if(Actor* pCannon = static_cast<Actor*>(GetChildByName(std::string("LaserCannon") + Utils::ToString(i+1))))
			{
				m_LaserCannons[i] = pCannon;				
			}
		}

		for(u32 i=0; i<m_sNumLaserCannons; ++i)
		{
			if(Actor* pCannon = static_cast<Actor*>(GetChildByName(std::string("MissileCannon") + Utils::ToString(i+1))))
			{
				m_MissileCannons[i] = pCannon;				
			}
		}
	}

	//! called during the update of the entity	
	void Boss4::Update()
	{
		m_pFSM->Update();
	}

	//! returns the number of laser cannons
	u32 Boss4::GetNumLaserCannons() const
	{
		u32 numCannons = 0;
		for(u32 i=0; i<m_sNumLaserCannons; ++i)
		{
			if(m_LaserCannons[i].IsValid() && m_LaserCannons[i]->GetHitPoints() >= 0.0f)
			{
				++numCannons;
			}
		}
		return numCannons;
	}

	//! returns the number of missile cannons
	u32 Boss4::GetNumMissileCannons() const
	{
		u32 numCannons = 0;
		for(u32 i=0; i<m_sNumMissileCannons; ++i)
		{
			if(m_MissileCannons[i].IsValid() && m_MissileCannons[i]->GetHitPoints() >= 0.0f)
			{
				++numCannons;
			}
		}
		return numCannons;
	}
}

