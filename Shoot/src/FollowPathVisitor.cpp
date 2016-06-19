/* 

Amine Rehioui
Created: August 7th 2011

*/

#include "Shoot.h"	

#include "FollowPathVisitor.h"

#include "Path.h"

namespace shoot
{
	DEFINE_OBJECT(FollowPathVisitor);

	//! constructor
	FollowPathVisitor::FollowPathVisitor()
		: m_p3DEntity(NULL)
		, m_fTimer(0.0f)
		, m_fDuration(0.0f)
		, m_CurrentElement(0)
		, m_bOrientEntity(true)		
	{
	}

	//! Reads/Writes struct properties from/to a stream
	void FollowPathVisitor::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Link, "Path", &m_Path);
		stream.Serialize(PT_Bool, "OrientEntity", &m_bOrientEntity);		
	}

	//! visits a particular entity
	void FollowPathVisitor::Visit(Entity* pTarget)
	{
		m_p3DEntity = DYNAMIC_CAST(pTarget, Entity3D);
		SHOOT_ASSERT(m_p3DEntity, "FollowPathVisitor target is not of type Entity3D");

		if(!m_Path.Get())
		{
			m_Path.Init(pTarget);
		}

		if(m_Path.Get() && (m_Path->GetChildCount() > 1))
		{
			m_CurrentElement = 0;
			m_fTimer = 0.0f;
			m_fDuration = m_Path->GetElement(m_CurrentElement)->GetFollowDuration();
			m_p3DEntity->SetUseRotationMatrix(m_bOrientEntity);
			super::Visit(pTarget);
		}	
	}

	//! updates the visitor	
	void FollowPathVisitor::Update()
	{
		if(!m_bActive)
		{
			return;
		}
		
		f32 fRatio = m_fTimer/m_fDuration;
		Vector3 vPosition = m_Path->GetPosition(m_CurrentElement, m_CurrentElement+1, fRatio);
		m_p3DEntity->SetAbsolutePosition(vPosition);		

		if(m_bOrientEntity)
		{
			Vector3 vPosition2 = m_Path->GetPosition(m_CurrentElement, m_CurrentElement+1, fRatio+.001f);
			Vector3 vDirection = (vPosition2 - vPosition).Normalize();
			Matrix44 lookAt;
			lookAt.BuildLookAtLH(Vector3::Zero, vDirection, m_Path->GetElement(m_CurrentElement)->GetUpVector());
			Matrix44 invLookAt;
			if(lookAt.GetInverse(invLookAt))
			{
				Matrix44 baseRotation;
				baseRotation.SetRotation(Vector3::Create(Math::PI/2.0f, 0.0f, 0.0f));
				m_p3DEntity->SetRotationMatrix(baseRotation*invLookAt);
			}
		}

		if(m_fTimer < m_fDuration)
		{
			m_fTimer += g_fDeltaTime;
		}
		else if(m_CurrentElement < s32(m_Path->GetChildCount())-2)
		{
			++m_CurrentElement;
			f32 newDuration = m_Path->GetElement(m_CurrentElement)->GetFollowDuration();
			f32 fOverTime = (m_fTimer-m_fDuration)*(m_fDuration/newDuration);			
			m_fDuration = newDuration-fOverTime;
			m_fTimer = fOverTime + g_fDeltaTime;
		}
		else
		{
			m_iPlayCount++;
			switch(m_ePlaybackType)
			{
			case PT_Once:
				Leave();
				break;

			case PT_Loop:			
				if((m_iMaxPlayCount < 0) || (m_iPlayCount < m_iMaxPlayCount))
				{
					m_CurrentElement = 0;
					f32 newDuration = m_Path->GetElement(m_CurrentElement)->GetFollowDuration();
					f32 fOverTime = (m_fTimer-m_fDuration)*(m_fDuration/newDuration);			
					m_fDuration = newDuration-fOverTime;
					m_fTimer = fOverTime + g_fDeltaTime;
				}
				else
				{
					Leave();
				}
				break;

			case PT_Toggle:
				SHOOT_WARNING(false, "FollowPathVisitor Unsupported PlaybackType: PT_Toggle");
				Leave();
				break;
				
			default:
				Leave();
			}
		}	
	}
}

