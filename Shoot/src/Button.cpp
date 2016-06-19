/* 

Amine Rehioui
Created: September 3rd 2013

*/

#include "Shoot.h"

#include "Button.h"

#include "Sprite.h"

#include "InputManager.h"
#include "EventManager.h"

namespace shoot
{
	DEFINE_OBJECT(Button);

	//! Constructor
	Button::Button()
		: m_bPressed(false)
		// properties
		, m_bCheckBox(false)
		, m_bChecked(true)
		, m_bCustomBBox(false)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void Button::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_String, "Command", &m_Command);
		stream.Serialize(PT_Bool, "CheckBox", &m_bCheckBox);
		stream.Serialize(PT_Bool, "CustomBBox", &m_bCustomBBox);
	}

	//! called during the initialization of the entity
	void Button::Init()
	{
		super::Init();

		m_Icon = static_cast<Sprite*>(GetChildByName("Icon"));

		// Init bounding box
		if(!m_bCustomBBox)
		{
			if(Entity2D* pBG = static_cast<Entity2D*>(GetChildByName("BG")))
			{
				m_BoundingBox = pBG->GetBoundingBox();
			}
			else if(m_Icon.IsValid())
			{
				m_BoundingBox = m_Icon->GetBoundingBox();
			}
		}

		m_vOriginalPosition = m_vPosition;
	}

	//! called during the update of the entity
	void Button::Update()
	{
		if(!GetRootContext()->IsTopContext() || !m_bVisible)
		{
			return;
		}

		if(InputManager::Instance()->IsTouchJustPressed())
		{
			if(IsTouched())
			{
				SetPressed(true);
			}
		}
		else if(InputManager::Instance()->IsTouchPressed())
		{
			if(m_bPressed && !IsTouched())
			{
				SetPressed(false);
			}
		}
		else if(InputManager::Instance()->IsTouchJustReleased())
		{
			if(m_bPressed)
			{
				if(m_bCheckBox)
				{
					SetChecked(!m_bChecked);
				}

				SendUIEvent();

				SetPressed(false);

				// somewhat of a hack, ensures no more elements can catch a touch released in the current frame
				InputManager::Instance()->Update();
			}
		}
	}

	//! returns true if the button is touched
	bool Button::IsTouched() const
	{
		Matrix44 transformation = Matrix44::Identity;
		transformation.Translate(-Vector3::Create(m_vCenter.X, m_vCenter.Y, 0.0f));
		transformation.Scale(Vector3::Create(m_vScale.X, m_vScale.Y, 1.0f));
		transformation.Rotate(Vector3::Create(0.0f, 0.0f, m_fRotation*Math::DegToRadFactor));		
		transformation.Translate(Vector3::Create(m_vOriginalPosition.X, m_vOriginalPosition.Y, 0.0f));
		if(Entity2D* p2DEntity = GetAncestor<Entity2D>())
		{
			transformation *= p2DEntity->GetTransformationMatrix();
		}

		Matrix44 inverse;
		if(transformation.GetInverse(inverse))
		{
			Vector2 vTouchPos = InputManager::Instance()->GetTouchState().vPosition;
			Vector3 invTouchPos3D = inverse.TransformVect(Vector3::Create(vTouchPos.X, vTouchPos.Y, 0.0f));
			Vector2 vInvTouchPos(invTouchPos3D.X, invTouchPos3D.Y);
			return GetBoundingBox().Contains(vInvTouchPos);
		}

		return false;
	}

	//! sends the UI event
	void Button::SendUIEvent()
	{
		UIEvent* pEvent = snew UIEvent();
		pEvent->m_Sender = this;
		pEvent->m_Command = m_Command;
		pEvent->m_bChecked = m_bChecked;
		EventManager::Instance()->SendEvent(pEvent);
	}

	//! changes pressed status
	void Button::SetPressed(bool bPressed)
	{
		if(bPressed)
		{
			m_vPosition = m_vOriginalPosition + Vector2(5.0f, 5.0f);
		}
		else
		{
			m_vPosition = m_vOriginalPosition;
		}
		m_bLocalTransformationMatrixDirty = true;
		m_bPressed = bPressed;
	}

	//! sets checked status
	void Button::SetChecked(bool bChecked)
	{
		if(m_Icon.IsValid())
		{
			m_Icon->SetCurrentFrame(bChecked ? 0 : 1);
		}

		m_bChecked = bChecked;
	}
}

