/* 

Amine Rehioui
Created: August 13th 2013

*/

#include "ShootTest.h"

#include "Boss3.h"

#include "Boss3FSM.h"

namespace shoot
{
	DEFINE_OBJECT(Boss3);

	//! constructor
	Boss3::Boss3()
		: m_fRadialTimer(2.0f)
		, m_bSweepPhase(true)
		// properties
		, m_fRadialFrequency(m_fRadialTimer)
		, m_fSweepFrequency(4.0f)
		, m_fCoreShotFrequency(4.0f)
		, m_fAngleRange(60.0f)
		, m_fAngleStep(25.0f)
		, m_fBulletLife(3.0f)
		, m_fBulletSpeed(12.0f)
		, m_fBulletOffset(0.5f)

		, m_fCannonAnimDuration(1.0f)
		, m_fCannonDestOffset(.5f)
		, m_fLaserThickness(1.2f)
		, m_fLaserOffset(1.5f)
		, m_fLaserSpeed(40.0f)
		, m_fLaserLength(30.0f)
		, m_fLaserFrequency(.35f)
		, m_fLaserLife(1.5f)
		, m_fLaserAimDuration(.25f)
		, m_fSweepDuration(2.5f)

		, m_fCoreAnimDuration(1.0f)
		, m_fCoreShotDuration(2.0f)
		, m_fCoreShotSpeed(15.0f)
		, m_fCoreShotBulletFrequency(.5f)
		, m_fCoreShotBulletLife(3.0f)
		, m_fCoreShotBulletSize(10.0f)
		, m_fCoreShotMinDist(10.0f)
		, m_fCoreShotHomingFactor(10.0f)
		, m_fCoreShotMaxHomingAngle(90.0f)

		, m_bDestroyWhenDone(false)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void Boss3::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Float, "RadialFrequency", &m_fRadialFrequency);
		stream.Serialize(PT_Float, "SweepFrequency", &m_fSweepFrequency);
		stream.Serialize(PT_Float, "CoreShotFrequency", &m_fCoreShotFrequency);
		stream.Serialize(PT_Float, "AngleRange", &m_fAngleRange);
		stream.Serialize(PT_Float, "AngleStep", &m_fAngleStep);
		stream.Serialize(PT_Float, "BulletLife", &m_fBulletLife);
		stream.Serialize(PT_Float, "BulletSpeed", &m_fBulletSpeed);
		stream.Serialize(PT_Float, "BulletOffset", &m_fBulletOffset);

		stream.Serialize(PT_Float, "CannonAnimDuration", &m_fCannonAnimDuration);
		stream.Serialize(PT_Float, "CannonDestOffset", &m_fCannonDestOffset);
		stream.Serialize(PT_Float, "LaserThickness", &m_fLaserThickness);
		stream.Serialize(PT_Float, "LaserOffset", &m_fLaserOffset);
		stream.Serialize(PT_Float, "LaserSpeed", &m_fLaserSpeed);
		stream.Serialize(PT_Float, "LaserLength", &m_fLaserLength);
		stream.Serialize(PT_Float, "LaserFrequency", &m_fLaserFrequency);
		stream.Serialize(PT_Float, "LaserLife", &m_fLaserLife);
		stream.Serialize(PT_Float, "LaserAimDuration", &m_fLaserAimDuration);		
		stream.Serialize(PT_Float, "SweepDuration", &m_fSweepDuration);

		stream.Serialize(PT_Float, "CoreAnimDuration", &m_fCoreAnimDuration);
		stream.Serialize(PT_Float, "CoreShotDuration", &m_fCoreShotDuration);
		stream.Serialize(PT_Float, "CoreShotSpeed", &m_fCoreShotSpeed);
		stream.Serialize(PT_Float, "CoreShotBulletFrequency", &m_fCoreShotBulletFrequency);
		stream.Serialize(PT_Float, "CoreShotBulletLife", &m_fCoreShotBulletLife);
		stream.Serialize(PT_Float, "CoreShotBulletSize", &m_fCoreShotBulletSize);
		stream.Serialize(PT_Float, "CoreShotMinDist", &m_fCoreShotMinDist);
		stream.Serialize(PT_Float, "CoreShotHomingFactor", &m_fCoreShotHomingFactor);
		stream.Serialize(PT_Float, "CoreShotMaxHomingAngle", &m_fCoreShotMaxHomingAngle);

		stream.Serialize(PT_Bool, "DestroyWhenDone", &m_bDestroyWhenDone);
		stream.SerializeReference("Retreat", &m_Retreat);
	}

	//! called during the initialization of the entity
	void Boss3::Init()
	{
		m_pFSM = snew Boss3FSM(this);
		super::Init();

		if(Entity* pRadials = GetChildByName("Radials"))
		{
			for(u32 i=0; i<pRadials->GetChildCount(); ++i)
			{
				SHOOT_ASSERT(pRadials->GetChild(i)->IsA(Actor::TypeID), "Radial not of type Actor");
				m_Radials.push_back(Handle<Actor>(static_cast<Actor*>(pRadials->GetChild(i))));
			}
		}

		m_Lid1 = static_cast<Entity3D*>(GetChildByName("Lid1"));
		m_Lid2 = static_cast<Entity3D*>(GetChildByName("Lid2"));
		m_Cannon = static_cast<Actor*>(GetChildByName("Cannon"));
		m_Core = static_cast<Actor*>(GetChildByName("Core"));
	}

	//! called during the update of the entity	
	void Boss3::Update()
	{
		m_pFSM->Update();
	}

	//! updates the radials
	void Boss3::UpdateRadials()
	{
		if(m_fRadialTimer < 0.0f)
		{
			if(Actor* pRadial = GetClosestRadial())
			{
				Vector3 vBulletPos = pRadial->GetTransformationMatrix().GetTranslation() + Vector3::Create(0.0f, -m_fBulletOffset, 0.0f);
				f32 fAngleStep = m_fAngleStep * Math::DegToRadFactor;
				Bullet::BulletParams params;
				f32 fRangeRad = m_fAngleRange * Math::DegToRadFactor;
				f32 fStartAngle = (Math::PI-fRangeRad)/2.0f;
				f32 fEndAngle = Math::PI-fStartAngle;
				for(f32 f = fStartAngle; f <= fEndAngle; f += fAngleStep)
				{
					params.vDirection = Vector3::Create(Math::Cos(f), -Math::Sin(f), 0.0f);
					params.fSpeed = m_fBulletSpeed;
					params.vPosition = vBulletPos;
					params.fLife = m_fBulletLife;
					params.fRadius = .5f;

					SFXManager::Instance()->GetEnemyPulseManager()->AddBullet(params);
				}
			}

			m_fRadialTimer = m_fRadialFrequency;
		}

		m_fRadialTimer -= g_fDeltaTime;
	}

	//! returns a radial
	Actor* Boss3::GetClosestRadial() const
	{
		f32 fMinDist = Math::Maxf32;
		Actor* pClosestRadial = NULL;
		f32 playerX = Player::Instance()->GetMeshEntity()->GetTransformationMatrix().GetTranslation().X;

		for(u32 i=0; i<m_Radials.size(); ++i)
		{
			Actor* pRadial = m_Radials[i].Get();
			if(pRadial && pRadial->GetHitPoints() >= 0.0f)
			{
				f32 fDist = Math::FAbs(pRadial->GetTransformationMatrix().GetTranslation().X-playerX);
				if(fDist < fMinDist)
				{
					pClosestRadial = pRadial;
					fMinDist = fDist;
				}
			}
		}
		
		return pClosestRadial;
	}
}

