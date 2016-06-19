/* 

Amine Rehioui
Created: October 27th 2013

*/

#include "Shoot.h"	

#include "MaterialVisitor.h"

namespace shoot
{
	DEFINE_OBJECT(MaterialVisitor);

	//! constructor
	MaterialVisitor::MaterialVisitor()
		// properties
		: m_fDuration(1.0f)
	{
	}

	//! destructor
	MaterialVisitor::~MaterialVisitor()
	{
	}

	//! Reads/Writes struct properties from/to a stream
	void MaterialVisitor::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
		
		stream.Serialize(PT_Color, "SrcColor", &m_SrcColor);
		stream.Serialize(PT_Float, "SrcAlpha", &m_SrcColor.A);
		stream.Serialize(PT_Color, "DestColor", &m_DestColor);
		stream.Serialize(PT_Float, "DestAlpha", &m_DestColor.A);
		stream.Serialize(PT_Float, "Duration", &m_fDuration);
	}

	//! visits a particular entity
	void MaterialVisitor::Visit(Entity* pTarget)
	{
		m_fInterpolator = 0.0f;
		GraphicComponent* pGraphic = pTarget->GetComponent<GraphicComponent>();

		super::Visit(pTarget);
	}

	//! updates the visitor	
	void MaterialVisitor::Update()
	{
		if(!m_bActive)
		{
			return;
		}

		if(m_fInterpolator < 1.0f)
		{
			Color color = Math::Lerp(m_SrcColor, m_DestColor, m_fInterpolator);
			UpdateColor(color);

			m_fInterpolator += (g_fDeltaTime/m_fDuration);
		}		
		else
		{
			UpdateColor(m_DestColor);

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
					(m_ePlaybackType == PT_Toggle) ? Math::Swap(m_SrcColor, m_DestColor) : void();
					m_fInterpolator = 0.0f;
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

	//! updates the color
	void MaterialVisitor::UpdateColor(const Color& color)
	{
		GraphicComponent* pGraphic = m_pTarget->GetComponent<GraphicComponent>();
		pGraphic->GetMaterial()->SetColor(color);		
	}
}

