/* 

Amine Rehioui
Created: April 7th 2011

*/

#include "Shoot.h"	

#include "Rotation3DVisitor.h"

namespace shoot
{
	DEFINE_OBJECT(Rotation3DVisitor);

	//! constructor
	Rotation3DVisitor::Rotation3DVisitor()	
		: m_p3DEntity(NULL)

		// properties		
		, m_fDuration(1.0f)
	{
	}

	//! destructor
	Rotation3DVisitor::~Rotation3DVisitor()
	{
	}

	//! Reads/Writes struct properties from/to a stream
	void Rotation3DVisitor::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);	

		stream.Serialize(PT_Struct, "Src", &m_Src);
		stream.Serialize(PT_Struct, "Dest", &m_Dest);
		stream.Serialize(PT_Float, "Duration", &m_fDuration);		
	}

	//! visits a particular entity
	void Rotation3DVisitor::Visit(Entity* pTarget)
	{
		SHOOT_ASSERT(pTarget->IsA(Entity3D::TypeID), "Rotation3DVisitor target is not of type Entity3D");
		m_p3DEntity = static_cast<Entity3D*>(pTarget);
		m_fCurrentTime = 0.0f;
		ESP<Vector3, PT_Vec3>::Compute<Entity3D>(m_Src, m_Dest, m_vSrcRotation, m_vDestRotation, m_p3DEntity, &Entity3D::GetRotation);
		super::Visit(pTarget);
	}

	//! updates the visitor	
	void Rotation3DVisitor::Update()
	{
		if(!m_bActive) 
		{
			return;
		}

		if(m_fCurrentTime < m_fDuration)
		{
			Vector3 rotation = Math::Lerp(m_vSrcRotation, m_vDestRotation, m_fCurrentTime/m_fDuration);			
			m_p3DEntity->SetRotation(rotation);

			m_fCurrentTime += g_fDeltaTime;
		}
		else
		{
			m_p3DEntity->SetRotation(m_vDestRotation);
			m_iPlayCount++;

			switch(m_ePlaybackType)
			{
			case PT_Once:
				Leave();
				break;

			case PT_Loop:
			case PT_Toggle:
				if((m_iMaxPlayCount < 0) || (m_iPlayCount < m_iMaxPlayCount))
				{
					(m_ePlaybackType == PT_Toggle) ? Math::Swap(m_vSrcRotation, m_vDestRotation) : void();
					m_fCurrentTime = 0.0f;
				}
				else
				{
					Leave();
				}
				break;
				
			default:
				Leave();
			}			
		}
	}
}

