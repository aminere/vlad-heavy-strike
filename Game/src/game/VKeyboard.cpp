/* 

Amine Rehioui
Created: November 11th 2013

*/

#include "ShootTest.h"

#include "VKeyboard.h"

#include "VKButton.h"

#include "Text.h"

namespace shoot
{
	DEFINE_OBJECT(VKeyboard);

	// statics
	VKeyboard* VKeyboard::m_spInstance = NULL;

	//! Constructor
	VKeyboard::VKeyboard()
		: m_bShowLetters(true)
	{
		SHOOT_ASSERT(!m_spInstance, "Multiple VKeyboard instances detected");
		m_spInstance = this;
	}

	//! Destructor
	VKeyboard::~VKeyboard()
	{
		m_spInstance = NULL;
	}

	//! serializes the entity to/from a PropertyStream
	void VKeyboard::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
	}

	//! called during the initialization of the entity
	void VKeyboard::Init()
	{
		super::Init();
		
		if(!InputManager::Instance()->IsVKeyboardAvailable())
		{
			const f32 buttonH = 60.0f;
			const f32 rowMargin = 8.0f;

			char row1[10] = { 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P' };
			char row1Numbers[10] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
			f32 row1Y = 4.0f;
			m_Row1 = FillRow(row1, 10, row1Y);
			m_Row1Numbers = FillRow(row1Numbers, 10, row1Y);
			AddChild(m_Row1);
			AddChild(m_Row1Numbers);
						
			char row2[9] = { 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L' };
			char row2Numbers[10] = { '@', '#', '$', '%', '~', '*', '-', '/', '(', ')' };
			f32 row2Y = row1Y + buttonH + rowMargin;
			m_Row2 = FillRow(row2, 9, row2Y);
			m_Row2Numbers = FillRow(row2Numbers, 10, row2Y);
			AddChild(m_Row2);
			AddChild(m_Row2Numbers);

			char row3[7] = { 'Z', 'X', 'C', 'V', 'B', 'N', 'M' };
			char row3Numbers[7] = { '!', '"', '\'', ':', ';', ',', '?' };
			f32 row3Y = row2Y + buttonH + rowMargin;
			m_Row3 = FillRow(row3, 7, row3Y);
			m_Row3Numbers = FillRow(row3Numbers, 7, row3Y);
			AddChild(m_Row3);
			AddChild(m_Row3Numbers);

			m_Row1Numbers->SetVisible(false);
			m_Row2Numbers->SetVisible(false);
			m_Row3Numbers->SetVisible(false);
			m_Row1Numbers->SetEnabled(false);
			m_Row2Numbers->SetEnabled(false);
			m_Row3Numbers->SetEnabled(false);
		}
		else
		{
			GetParent()->SetVisible(false);
			GetParent()->SetEnabled(false);
		}
	}

	//! fills a row
	Entity* VKeyboard::FillRow(char* letters, s32 letterCount, f32 rowY)
	{
		const f32 width = 480.0f;
		const f32 spacing = 2.0f;
		const f32 buttonW = 44.0f;
		const f32 buttonH = 60.0f;
		f32 row1X = (width-(buttonW*letterCount + spacing*(letterCount-1)))/2.0f;
		Reference<VKButton> buttonTemplate = static_cast<VKButton*>(ObjectManager::Instance()->Get("common/menuvkbutton.xml"));
		Entity* pRow = snew Entity();
		for(s32 i=0; i<letterCount; ++i)
		{
			VKButton* pButton = static_cast<VKButton*>(buttonTemplate->Copy());
			pButton->SetLetter(std::string("") + letters[i]);
			pButton->SetPosition(Vector2(row1X, rowY));
			pButton->SetBoundingBox(AABBox2D(Vector2(), Vector2(buttonW, buttonH)));
			static_cast<Entity2D*>(pButton->GetChildByName("BG"))->SetScale(Vector2(buttonW/32.0f, buttonH/32.0f));
			Text* pLetter = static_cast<Text*>(pButton->GetChildByName("Letter"));
			pLetter->SetPosition(Vector2(0.0f, (buttonH-32.0f)/2.0f));
			pLetter->SetAreaSize(Size(s32(buttonW), 32));
			pRow->AddChild(pButton);
			row1X += (buttonW + spacing);
		}
		return pRow;
	}

	//! toggles the mode
	void VKeyboard::ToggleMode()
	{
		m_bShowLetters = !m_bShowLetters;
		m_Row1->SetVisible(m_bShowLetters);
		m_Row2->SetVisible(m_bShowLetters);
		m_Row3->SetVisible(m_bShowLetters);
		m_Row1Numbers->SetVisible(!m_bShowLetters);
		m_Row2Numbers->SetVisible(!m_bShowLetters);
		m_Row3Numbers->SetVisible(!m_bShowLetters);
		m_Row1->SetEnabled(m_bShowLetters);
		m_Row2->SetEnabled(m_bShowLetters);
		m_Row3->SetEnabled(m_bShowLetters);
		m_Row1Numbers->SetEnabled(!m_bShowLetters);
		m_Row2Numbers->SetEnabled(!m_bShowLetters);
		m_Row3Numbers->SetEnabled(!m_bShowLetters);
		Text* pToggleText = static_cast<Text*>(GetParent()->GetChildByName("ModeToggle")->GetChildByName("Text"));
		pToggleText->SetText(m_bShowLetters ? "?123" : "ABC");
	}

	//! called during the update of the entity
	void VKeyboard::Update()
	{
		if(!GetRootContext()->IsTopContext())
		{
			return;
		}		
	}	
}

