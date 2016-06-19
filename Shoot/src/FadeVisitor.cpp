/* 

Amine Rehioui
Created: October 27th 2013

*/

#include "Shoot.h"	

#include "FadeVisitor.h"

#include "FadeRenderer.h"

namespace shoot
{
	DEFINE_OBJECT(FadeVisitor);

	//! constructor
	FadeVisitor::FadeVisitor()
		: m_fTimer(0.0f)
		// properties
		, m_fDuration(1.0f)
	{
	}

	//! destructor
	FadeVisitor::~FadeVisitor()
	{
	}

	//! Reads/Writes struct properties from/to a stream
	void FadeVisitor::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		
		stream.Serialize(PT_Color, "SrcColor", &m_SrcColor);
		stream.Serialize(PT_Float, "SrcAlpha", &m_SrcColor.A);
		stream.Serialize(PT_Color, "DestColor", &m_DestColor);
		stream.Serialize(PT_Float, "DestAlpha", &m_DestColor.A);
		stream.Serialize(PT_Float, "Duration", &m_fDuration);
	}

	//! visits a particular entity
	void FadeVisitor::Visit(Entity* pTarget)
	{		
		super::Visit(pTarget);
		FadeRenderer::Instance()->StartFade(m_SrcColor, m_DestColor, m_fDuration);
		m_fTimer = m_fDuration;
	}

	//! updates the visitor	
	void FadeVisitor::Update()
	{
		if(!m_bActive)
		{
			return;
		}

		if(m_fTimer > 0.0f)
		{
			m_fTimer -= g_fDeltaTime;
		}
		else
		{
			Leave();				
		}
	}	
}

