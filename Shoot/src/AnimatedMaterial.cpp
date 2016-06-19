/* 

Amine Rehioui
Created: November 26th 2011

*/

#include "Shoot.h"

#include "AnimatedMaterial.h"

namespace shoot
{
	DEFINE_OBJECT(AnimatedMaterial);

	//! constructor
	AnimatedMaterial::AnimatedMaterial()
		: m_fCurrentFrameDuration(0.0f)
		, m_CurrentFrame(0)
		, m_Direction(1)
		// properties
		, m_framesPerSecond(30.0f)
		, m_fFrameDeltaTime(0.03f)
		, m_bToggle(false)
	{
	}

	//! destructor
	AnimatedMaterial::~AnimatedMaterial()
	{
	}

	//! Reads/Writes struct properties from/to a stream
	void AnimatedMaterial::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		if(stream.Serialize(PT_Float, "FPS", &m_framesPerSecond))
		{
			m_fFrameDeltaTime = 1.0f / m_framesPerSecond;
		}

		if(stream.Serialize(PT_Bool, "Toggle", &m_bToggle))
		{
			m_Direction = m_bToggle ? m_Direction : 1;
		}
	}

	//! returns a texture
	Texture* AnimatedMaterial::GetTexture(u32 index /*= 0*/)
	{
		Update();
		return super::GetTexture(m_CurrentFrame);	
	}

	//! updates the material
	void AnimatedMaterial::Update()
	{
		m_fCurrentFrameDuration += g_fDeltaTime;
		if(m_fCurrentFrameDuration > m_fFrameDeltaTime)
		{
			m_CurrentFrame += m_Direction;
			m_fCurrentFrameDuration = 0.0f;

			if(m_bToggle)
			{
				if(m_Direction > 0 && (m_CurrentFrame == m_aTextures.size()))
				{				
					m_CurrentFrame = m_aTextures.size()-2;
					m_Direction *= -1;
				}
				else if(m_Direction < 0 && (m_CurrentFrame < 0))
				{
					m_CurrentFrame = 1;
					m_Direction *= -1;
				}
				m_CurrentFrame = Math::Clamp<s32>(m_CurrentFrame, 0, m_aTextures.size()-1);
			}
			else
			{
				if(m_CurrentFrame >= s32(m_aTextures.size()))
				{
					m_CurrentFrame = 0;
				}
			}			
		}
	}
}

