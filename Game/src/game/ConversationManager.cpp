/* 

Amine Rehioui
Created: April 22nd 2013

*/

#include "ShootTest.h"

#include "ConversationManager.h"

#include "Text.h"
#include "TextVisitor.h"
#include "Utils.h"

namespace shoot
{
	DEFINE_OBJECT(ConversationManager);

	// Phrase type literals
	const char* const g_strPhraseTypeLiterals[] =
	{
		"Enemy",
		"Player",
		0
	};

	//! static vars
	ConversationManager* ConversationManager::ms_pInstance = NULL;

	//! constructor
	ConversationManager::ConversationManager()
		: m_bActive(false)
		, m_fCurrentPhraseDuration(-1.0f)
	{
	}

	//! destructor
	ConversationManager::~ConversationManager()
	{
		ms_pInstance = NULL;
	}

	//! serializes the entity to/from a PropertyStream
	void ConversationManager::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);
	}

	//! called during the initialization of the entity
	void ConversationManager::Init()
	{
		super::Init();
		SHOOT_ASSERT(!ms_pInstance, "Multiple ConversationManager instances detected");
		ms_pInstance = this;

		for(u32 i=0; i<T_Count; ++i)
		{
			m_TextEntity[i] = static_cast<Entity2D*>(GetChildByName(std::string("TextEntity") + Utils::ToString(i+1)));
			if(Entity* pTextEntity = m_TextEntity[i].Get())
			{
				pTextEntity->SetVisible(false);
				m_TextBox[i] = static_cast<Text*>(pTextEntity->GetChildByType("Text"));
			}
		}
	}

	//! called during the update of the entity
	void ConversationManager::Update()
	{
		if(m_bActive)
		{
			if(m_fCurrentPhraseDuration < 0.0f)
			{
				if(m_lPhrases.size())
				{
					Phrase& phrase = m_lPhrases.front();

					// fade out other types a bit
					for(u32 i=0; i<T_Count; ++i)
					{
						if(i != phrase.m_eType)
						{
							if(m_TextEntity[i]->IsVisible())
							{
								Color boxColor = m_TextEntity[i]->GetComponent<GraphicComponent>()->GetMaterial()->GetColor();
								boxColor.A = .3f;
								Color textColor = m_TextBox[i]->GetComponent<GraphicComponent>()->GetMaterial()->GetColor();
								textColor.A = .3f;
								m_TextEntity[i]->GetComponent<GraphicComponent>()->GetMaterial()->SetColor(boxColor);
								m_TextBox[i]->GetComponent<GraphicComponent>()->GetMaterial()->SetColor(textColor);
							}
						}
					}

					// fade in if necessary
					//if(!m_TextEntity[phrase.m_eType]->IsVisible())
					//{
					//	if(!m_TextEntity[phrase.m_eType]->GetComponent<FadeVisitor>())
					//	{
					//		m_TextEntity[phrase.m_eType]->AddComponent(snew FadeVisitor(), true);
					//	}
					//	FadeVisitor* pFadeVisitor = m_TextEntity[phrase.m_eType]->GetComponent<FadeVisitor>();
					//	pFadeVisitor->SetType(FadeVisitor::T_FadeIn);
					//	pFadeVisitor->Init();
					//}
					Color boxColor = m_TextEntity[phrase.m_eType]->GetComponent<GraphicComponent>()->GetMaterial()->GetColor();
					boxColor.A = 1.0f;
					Color textColor = m_TextBox[phrase.m_eType]->GetComponent<GraphicComponent>()->GetMaterial()->GetColor();
					textColor.A = 1.0f;
					m_TextEntity[phrase.m_eType]->GetComponent<GraphicComponent>()->GetMaterial()->SetColor(boxColor);
					m_TextBox[phrase.m_eType]->GetComponent<GraphicComponent>()->GetMaterial()->SetColor(textColor);
					m_TextEntity[phrase.m_eType]->SetVisible(true);
					
					// start the text effect
					m_TextBox[phrase.m_eType]->SetText(phrase.m_strText.c_str());
					m_TextBox[phrase.m_eType]->PreRender();
					TextVisitor* pTextVisitor = m_TextBox[phrase.m_eType]->GetComponent<TextVisitor>();
					if(!pTextVisitor)
					{
						m_TextBox[phrase.m_eType]->AddComponent(snew TextVisitor(), true);
					}
					m_TextBox[phrase.m_eType]->GetComponent<TextVisitor>()->Init();					
					m_fCurrentPhraseDuration = phrase.m_fDuration;
				}
			}
			else
			{
				m_fCurrentPhraseDuration -= g_fDeltaTime;
				if(m_fCurrentPhraseDuration < 0.0f)
				{
					m_lPhrases.erase(m_lPhrases.begin());
					if(m_lPhrases.empty())
					{
						for(u32 i=0; i<T_Count; ++i)
						{
							// fade out
							//FadeVisitor* pFadeVisitor = m_TextEntity[i]->GetComponent<FadeVisitor>();
							//pFadeVisitor->SetType(FadeVisitor::T_FadeOut);
							//pFadeVisitor->Init();
							m_TextEntity[i]->SetVisible(false);
						}
						m_bActive = false;
					}
				}
			}
		}
	}	

	//! Add a phrase
	void ConversationManager::AddPhrase(E_Type eType, const std::string& strText, f32 fDuration)
	{
		SHOOT_ASSERT(eType < T_Count, "Invalid ConversationManager::E_Type");
		AddPhrase(Phrase(eType, strText, fDuration));
	}

	//! Start the conversation
	void ConversationManager::Start()
	{
		if(m_lPhrases.size())
		{
			m_bActive = true;
			m_fCurrentPhraseDuration = -1.0f;
		}
	}

	//! Reads/Writes struct properties from/to a stream
	void ConversationManager::Phrase::Serialize(PropertyStream& stream)
	{
		stream.Serialize(PT_Enum, "Type", &m_eType, ENUM_PARAMS1(g_strPhraseTypeLiterals));
		stream.Serialize(PT_String, "Text", &m_strText);
		stream.Serialize(PT_Float, "Duration", &m_fDuration);
	}
}

