/* 

Amine Rehioui
Created: February 5th 2010

*/

#include "Shoot.h"

#include "InputManagerSDL.h"

namespace shoot
{
	//! Constructor
	InputManagerSDL::InputManagerSDL() 
	{
		memset(m_aKeyStates, 0, KT_Count*sizeof(bool));
		memset(m_aPreviousKeyStates, 0, KT_Count*sizeof(bool));

		// init SDL to shoot mappings
		for(u32 i=0; i<SDLK_LAST; ++i)
		{
			m_aShootKeyTypes[i] = KT_None;
		}
		m_aShootKeyTypes[SDLK_LEFT] = KT_Left;
		m_aShootKeyTypes[SDLK_RIGHT] = KT_Right;
		m_aShootKeyTypes[SDLK_UP] = KT_Up;
		m_aShootKeyTypes[SDLK_DOWN] = KT_Down;
		m_aShootKeyTypes[SDLK_l] = KT_L;
		m_aShootKeyTypes[SDLK_r] = KT_R;
		m_aShootKeyTypes[SDLK_a] = KT_A;
		m_aShootKeyTypes[SDLK_b] = KT_B;
		m_aShootKeyTypes[SDLK_x] = KT_X;
		m_aShootKeyTypes[SDLK_y] = KT_Y;
		m_aShootKeyTypes[SDLK_ESCAPE] = KT_Exit;
		m_aShootKeyTypes[SDLK_BACKSPACE] = KT_Back;
	}

	//! Destructor
	InputManagerSDL::~InputManagerSDL()
	{		
	}

	//! Update the input state
	void InputManagerSDL::Update()
	{
		memcpy(m_aPreviousKeyStates, m_aKeyStates, KT_Count*sizeof(bool));
		m_PreviousTouchState = m_TouchState;

		super::Update();
	}

	//! returns true only during the frame in which the key moved from unpressed to pressed state
	bool InputManagerSDL::IsKeyJustPressed(E_KeyType eKey) const
	{
		return m_aKeyStates[eKey] && !m_aPreviousKeyStates[eKey];
	}

	//! returns true for as long as the key is pressed
	bool InputManagerSDL::IsKeyPressed(E_KeyType eKey) const
	{
		return m_aKeyStates[eKey];
	}

	//! returns true only during the frame in which the key moved from pressed to unpressed state
	bool InputManagerSDL::IsKeyJustReleased(E_KeyType eKey) const
	{
		return !m_aKeyStates[eKey] && m_aPreviousKeyStates[eKey];
	}

	//! returns true only during the frame in which the touch moved from unpressed to pressed state
	bool InputManagerSDL::IsTouchJustPressed() const
	{
		return m_TouchState.bPressed && !m_PreviousTouchState.bPressed;
	}

	//! returns true for as long as the touch is pressed
	bool InputManagerSDL::IsTouchPressed() const
	{
		return m_TouchState.bPressed;
	}

	//! returns true only during the frame in which the touch moved from pressed to unpressed state
	bool InputManagerSDL::IsTouchJustReleased() const
	{
		return !m_TouchState.bPressed && m_PreviousTouchState.bPressed;
	}
	
	//! sets the touch state
	void InputManagerSDL::SetTouchState(const TouchState& state)
	{
		m_TouchState = m_RawTouchState = state;

		// take camera transformation into account
		if(Camera* p2DCamera = EntityRenderer::Instance()->Get2DCamera())
		{
			Vector3 vCameraTranslation = p2DCamera->GetViewMatrix().GetTranslation();
			Vector3 vCameraScale = p2DCamera->GetViewMatrix().GetScale();
			m_TouchState.vPosition -= Vector2(vCameraTranslation.X, vCameraTranslation.Y);
			m_TouchState.vPosition.Set(m_TouchState.vPosition.X/vCameraScale.X, m_TouchState.vPosition.Y/vCameraScale.Y);
		}
	}

	//! force key status
	void InputManagerSDL::SetKeyPressed(E_KeyType eKey, bool bSet)
	{
		if(eKey != KT_None)
		{
			m_aKeyStates[eKey] = bSet;
		}		
	}
}

