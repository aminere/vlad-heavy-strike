/* 

Amine Rehioui
Created: September 3rd 2013

*/

#ifndef _BUTTON_H_INCLUDED_
#define _BUTTON_H_INCLUDED_

namespace shoot
{
	// forwards
	class Sprite;

	//! Button
	class Button : public Entity2D				 
	{	
	public:

		//! Macro to provide type information
		DECLARE_OBJECT(Button, Entity2D);

		//! Constructor
		Button();

		//! serializes the entity to/from a PropertyStream
		virtual void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		virtual void Init();

		//! called during the update of the entity
		virtual void Update();

		//! sets checked status
		void SetChecked(bool bChecked);

	protected:

		//! returns true if the button is touched
		virtual bool IsTouched() const;

		//! sends the UI event
		virtual void SendUIEvent();

		//! changes pressed status
		void SetPressed(bool bPressed);

		bool m_bPressed;
		bool m_bChecked;
		Handle<Sprite> m_Icon;
		Vector2 m_vOriginalPosition;

		// properties
		std::string m_Command;
		bool m_bCheckBox;
		bool m_bCustomBBox;
	};
}

#endif // _BUTTON_H_INCLUDED_

