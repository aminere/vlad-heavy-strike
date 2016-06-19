/* 

Amine Rehioui
Created: February 23rd 2013

*/

#include "Shoot.h"

namespace shoot
{
	// type literals
	const char* const g_strRenderingPasses[] =
	{
		"SkyBox",			
		"3D",
		"Transparent3D",
		"2D",
		"Transparent2D",
		"2DPostFade",
		"None",
		0
	};

	DEFINE_OBJECT(GraphicComponent);

	//! constructor
	GraphicComponent::GraphicComponent()
		: m_RenderingPriority(0)
		, m_eRenderingPass(RP_None)
	{
	}

	//! destructor
	GraphicComponent::~GraphicComponent()
	{
	}

	//! serializes the entity to/from a PropertyStream
	void GraphicComponent::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.SerializeReference("Material", &m_Material);
		stream.Serialize(PT_UInt, "RenderingPriority", &m_RenderingPriority);
		stream.Serialize(PT_Enum, "RenderingPass", &m_eRenderingPass, ENUM_PARAMS1(g_strRenderingPasses));
	}
}

