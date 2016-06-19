/* 

Amine Rehioui
Created: September 3rd 2013

*/

#include "ShootTest.h"

#include "PlayerList.h"

#include "GameManager.h"

#include "Text.h"

#include "InputManager.h"

#include "Visitor.h"

namespace shoot
{
	DEFINE_OBJECT(PlayerList);

	//! Constructor
	PlayerList::PlayerList()
		: m_ScrollPos(0)
		, m_bScrollingStarted(false)
		// properties
		, m_fScrollTreshold(10.0f)
	{
	}

	//! serializes the entity to/from a PropertyStream
	void PlayerList::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Float, "ScrollTreshold", &m_fScrollTreshold);
	}

	//! called during the initialization of the entity
	void PlayerList::Init()
	{
		super::Init();

		m_vOriginalPosition = m_vPosition;
		m_Highlighter = static_cast<Entity2D*>(GetChildByName("Highlighter"));

		Fill(true);
	}

	//! called during the update of the entity
	void PlayerList::Update()
	{
		if(!GetRootContext()->IsTopContext())
		{
			return;
		}

		InputManager* pInputMgr = InputManager::Instance();
		if(pInputMgr->IsTouchJustPressed())
		{
			m_bScrollingStarted = (GetBoundingBox() + GetPosition()).Contains(pInputMgr->GetTouchState().vPosition);
			if(m_bScrollingStarted)
			{
				m_vPreviousTouchPos = pInputMgr->GetTouchState().vPosition;
				m_fTouchYAccumulator = 0.0f;
			}
		}
		else if(pInputMgr->IsTouchPressed())
		{
			if(m_bScrollingStarted)
			{
				Vector2 vDeltaTouch = pInputMgr->GetTouchState().vPosition-m_vPreviousTouchPos;
				m_fTouchYAccumulator += vDeltaTouch.Y;

				if(Math::FAbs(m_fTouchYAccumulator) > m_fScrollTreshold)
				{
					// scroll
					bool bScrollUp = Math::FSign(m_fTouchYAccumulator) > 0.0f;
					m_fTouchYAccumulator = m_fTouchYAccumulator - m_fScrollTreshold*(bScrollUp ? 1.0f : -1.0f);
					m_ScrollPos += bScrollUp ? -1 : 1;
					if(m_ScrollPos < 0)
					{
						SetPosition(m_vOriginalPosition);
						static_cast<Visitor*>(GetComponent("ScrollDownAnim"))->Visit(this);
					}
					else if(m_ScrollPos > s32(GameManager::Instance()->GetScores().size()-GetChildCount()))
					{
						SetPosition(m_vOriginalPosition);
						static_cast<Visitor*>(GetComponent("ScrollUpAnim"))->Visit(this);
					}
					Fill();
				}

				m_vPreviousTouchPos = pInputMgr->GetTouchState().vPosition;
			}
		}
	}

	//! fills the list
	void PlayerList::Fill(bool bInitScrollPos /*= false*/)
	{
		if(bInitScrollPos)
		{
			s32 index = GameManager::Instance()->GetIndexInLeaderboard();
			if(index >= 0)
			{
				m_ScrollPos = index;
			}
		}

		u32 maxLines = GetChildCount()-1;
		const std::list<ScoreoidUtils::ScoreInfo>& scores = GameManager::Instance()->GetScores();
		m_ScrollPos = Math::Clamp(m_ScrollPos, 0, Math::Max(0, s32(scores.size()-maxLines)));
		m_Highlighter->SetVisible(false);
		const std::string& playerName = GameManager::Instance()->GetGameData()->GetPlayerName();

		u32 line = 0;
		s32 index = 0;
		for(std::list<ScoreoidUtils::ScoreInfo>::const_iterator it = scores.begin(); 
			it != scores.end() && line < maxLines; 
			++it, ++index)
		{
			if(index < m_ScrollPos)
			{
				continue;
			}

			const ScoreoidUtils::ScoreInfo& score = (*it);
			Entity2D* pLine = static_cast<Entity2D*>(GetChild(line));
			
			Text* pRank = static_cast<Text*>(pLine->GetChildByName("ListRank"));
			Text* pName = static_cast<Text*>(pLine->GetChildByName("ListName"));
			Text* pPlatform = static_cast<Text*>(pLine->GetChildByName("ListPlatform"));
			Text* pScore = static_cast<Text*>(pLine->GetChildByName("ListScore"));
			pRank->SetText(score.m_Rank.c_str());
			pName->SetText(score.m_Name.c_str());
			pPlatform->SetText(score.m_Platform.c_str());
			pScore->SetText(score.m_Score.c_str());
			
			Color cyan(0.0f, 1.0f, 1.0f, 1.0f);
			bool bPlayerLine = (score.m_Name == playerName);
			Color color = bPlayerLine ? cyan : Color::White;
			pRank->SetColor(color);
			pName->SetColor(color);
			pPlatform->SetColor(color);
			pScore->SetColor(color);

			if(bPlayerLine)
			{
				m_Highlighter->SetVisible(true);
				m_Highlighter->SetPosition(Vector2(0.0f, 40.0f*line));
			}

			++line;
		}

		// empty the remaining lines
		for(u32 i=line; i<maxLines; ++i)
		{
			Entity2D* pLine = static_cast<Entity2D*>(GetChild(i));
			static_cast<Text*>(pLine->GetChildByName("ListRank"))->SetText("");
			static_cast<Text*>(pLine->GetChildByName("ListName"))->SetText("");
			static_cast<Text*>(pLine->GetChildByName("ListPlatform"))->SetText("");
			static_cast<Text*>(pLine->GetChildByName("ListScore"))->SetText("");
		}
	}
}

