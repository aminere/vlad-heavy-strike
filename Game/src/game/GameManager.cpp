/* 

Amine Rehioui
Created: October 11th 2011

*/

#include "ShootTest.h"

#include "GameManager.h"

#include "File.h"

#include "FileSystem.h"

#include "AudioManager.h"
#include "AudioDriver.h"
#include "EventManager.h"
#include "Button.h"
#include "UISlider.h"
#include "Text.h"
#include "Utils.h"
#include "TextVisitor.h"
#include "SequenceVisitor.h"
#include "PlayerList.h"

#include "UIHandler.h"
#include "InputManager.h"
#include "Mutex.h"

#if SHOOT_PLATFORM == SHOOT_PLATFORM_WP8
extern bool g_bWP8TrialStatusUpdated;
#elif SHOOT_PLATFORM == SHOOT_PLATFORM_IOS
extern bool g_bSkillzReportScore;
extern bool g_bSkillzAbort;
extern int g_SkillzScore;
extern bool IsSkillzGame();
#endif

namespace shoot
{
	DEFINE_OBJECT(GameManager);

	//! static vars
	GameManager* GameManager::ms_pInstance = NULL;

	//! constructor
	GameManager::GameManager()
		: m_eState(S_Menu)
		, m_eLeaderboardState(LS_None)
		, m_PreviousScore(0)
		, m_Score(0)
		, m_LeaderboardIndex(-1)
		, m_LeaderboardLevelIndex(-1)
		, m_bLevelCompleted(false)
		, m_PlayerCount(-1)
		, m_bNewHighScore(false)
		, m_bGetFullGameRequest(false)
		, m_bBestLevelScoresReceived(false)
		, m_bContextChangedFlag(false)
		, m_bNetCallbackReceived(false)
		, m_bIsCreatingAccount(false)
		, m_bLevelLeaderBoard(false)
		, m_bLevelScoreAvailable(false)
#if SHOOT_PLATFORM == SHOOT_PLATFORM_WP8
		, m_bWP8TrialMode(true)
#endif
#if SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID
		, m_bAndroidTrialMode(false)
		, m_bPreviousAndroidTrialMode(false)
#endif
		// properties
		, m_fUITransitionDuration(1.0f)
		, m_fUITransitionOriginTop(-100.0f)
		, m_fUITransitionOriginBottom(100.0f)
	{
		SHOOT_ASSERT(!ms_pInstance, "Multiple GameManager instances detected");
		ms_pInstance = this;
		m_pMutex = Mutex::Create();

		{
			File::UseWritablePath uwp;
			if(File::Exists(GameData::m_sSaveFile))
			{
				m_GameData = static_cast<GameData*>(ObjectManager::Instance()->Get(GameData::m_sSaveFile));
			}
			else
			{
				m_GameData = snew GameData();
				m_GameData->Save();
			}
		}

		AudioManager::Instance()->SetMusicVolume(m_GameData->GetMusicVolume());
		AudioDriver::Instance()->SetSoundVolume(m_GameData->GetSoundVolume());
	}

	//! destructor
	GameManager::~GameManager()
	{
		EventManager::Instance()->UnregisterListener<UIEvent>(this);
		EventManager::Instance()->UnregisterListener<TriggerEvent>(this);
		sdelete(m_pMutex);
		ms_pInstance = NULL;
	}

	//! serializes the entity to/from a PropertyStream
	void GameManager::Serialize(PropertyStream& stream)
	{
		super::Serialize(stream);

		stream.Serialize(PT_Float, "UITransitionDuration", &m_fUITransitionDuration);
		stream.Serialize(PT_Float, "UITransitionOriginTop", &m_fUITransitionOriginTop);
		stream.Serialize(PT_Float, "UITransitionOriginBottom", &m_fUITransitionOriginBottom);

		stream.SerializeArray("Levels", &m_Levels, PT_Struct);
	}

	//! called during the initialization of the entity
	void GameManager::Init()
	{
		super::Init();
		
#ifndef SHOOT_EDITOR
		if(!m_GameData->GetIntroPlayed())
		{
			Engine::Instance()->LoadContextStack("mothership/introscene.xml");
			m_GameData->SetIntroPlayed(true);
			m_GameData->Save();
			AudioManager::Instance()->Play(AudioManager::M_Intro);
		}
		else
		{
			Engine::Instance()->LoadContextStack("common/menuscene.xml");
			AudioManager::Instance()->Play(AudioManager::M_Metal);
		}
#endif // SHOOT_EDITOR

		EventManager::Instance()->RegisterListener<UIEvent>(this, &UIHandler::HandleEvent);
		EventManager::Instance()->RegisterListener<TriggerEvent>(this, &UIHandler::HandleTriggerEvent);
	}

	//! called during the update of the entity
	void GameManager::Update()
	{
		switch(m_eState)
		{
		case S_InGame:
			if(m_PreviousScore != m_Score)
			{
				std::string score = Utils::ToString(m_Score);
				m_ScoreText->SetText(score.c_str());
				m_ScoreTextContainer->GetComponent<SequenceVisitor>()->Visit(m_ScoreTextContainer);
				m_ScoreTextContainer->SetPosition(Vector2(480.0f-(score.length()*25.0f+32.0f)+80.0f, 16.0f));
				m_ScoreTextContainer->SetVisible(true);
				m_PreviousScore = m_Score;
			}
			break;

		case S_Menu:
			{
				if(m_bBestLevelScoresReceived)
				{
					m_LeaderboardLevelScore = m_BestLevelScores.front().m_Score;
					Context* pContext = Engine::Instance()->GetTopContext();
					if(pContext && pContext->GetTemplatePath() == "common/menulevelselect.xml")
					{
						if(Text* pScore = static_cast<Text*>(pContext->GetChildByName("BestLevelScore")))
						{
							pScore->SetText((std::string("Best Score: ") + m_LeaderboardLevelScore).c_str());
							pScore->GetComponent<TextVisitor>()->Visit(pScore);
						}
					}
					m_bBestLevelScoresReceived = false;
				}

#if SHOOT_PLATFORM == SHOOT_PLATFORM_WP8
				if(g_bWP8TrialStatusUpdated)
				{
					Context* pContext = Engine::Instance()->GetTopContext();
					if(pContext && pContext->GetTemplatePath() == "common/menulevelselect.xml")
					{
						UpdateLevelSelectMenu(pContext);
					}
					g_bWP8TrialStatusUpdated = false;
				}
#endif

#if SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID
				if(m_bAndroidTrialMode != m_bPreviousAndroidTrialMode)
				{
					Context* pContext = Engine::Instance()->GetTopContext();
					if(pContext && pContext->GetTemplatePath() == "common/menulevelselect.xml")
					{
						UpdateLevelSelectMenu(pContext);
					}
					m_bPreviousAndroidTrialMode = m_bAndroidTrialMode;
				}
#endif
			}
			break;
		}

		if(m_eLeaderboardState != GameManager::LS_None
		&& m_bNetCallbackReceived
		&& !m_bContextChangedFlag)
		{
			m_pMutex->Lock();
			m_LocalNetResult = m_NetResult;
			m_pMutex->Unlock();
			OnlineProcessResult(m_LocalNetResult);
			m_bNetCallbackReceived = false;
		}
	}
	
	//! returns true if a point is out of the play field
	bool GameManager::IsOutOfPlayfield(const Vector3& vPoint)
	{
		Player* pPlayer = Player::Instance();
		const f32 fDisappearOffset = 8.0f;
		const Vector3& vPlayerPos = pPlayer->GetPosition();
		if((vPoint.Y+fDisappearOffset < vPlayerPos.Y)
		|| (vPoint.X+fDisappearOffset < vPlayerPos.X-pPlayer->GetMotionBounds().Size().X/2.0f)
		|| (vPoint.X-fDisappearOffset > vPlayerPos.X+pPlayer->GetMotionBounds().Size().X/2.0f))
		{
			return true;
		}
		return false;
	}

	//! called when a level is finished
	void GameManager::OnLevelFinished()
	{
        m_bLevelCompleted = true;
        
#if SHOOT_PLATFORM == SHOOT_PLATFORM_IOS
        if(IsSkillzGame()) // check if in skillz tournament
        {
          g_bSkillzReportScore = true;
          g_SkillzScore = m_Score;
          ExitLevel();
          return;
        }
#endif
        
		Engine::Instance()->PushContext("common/menupausebg.xml");
		Engine::Instance()->PushContext("common/menuconnecting.xml");
		OnlineCheckConnection();
	}

	//! Loads the current level
	void GameManager::LoadLevel()
	{
		Engine::Instance()->LoadContextStack(m_Levels[m_GameData->GetCurrentLevel()].m_Path.c_str(), false, &OnLevelLoaded);
	}

	//! pauses the game
	void GameManager::Pause()
	{
		if(m_eState == S_InGame && m_eLeaderboardState == LS_None)
		{
			Engine::Instance()->PushContext("common/menupausebg.xml");
			Engine::Instance()->PushContext("common/menupause.xml");
			SetState(S_InGamePause);
		}
	}

	//! resumes the game
	void GameManager::Resume()
	{
		Engine::Instance()->PopContext();
		if(m_eState == S_AskExitGame)
		{
			SetState(S_Menu);
		}
		else
		{
			SetState(S_InGame);
		}
	}

	//! exits the current level
	void GameManager::ExitLevel()
	{
		m_NavigationList.clear();
        
#if SHOOT_PLATFORM == SHOOT_PLATFORM_IOS
        if(m_bLevelCompleted)
		{
            m_bLevelCompleted = false;
			u32 nextLevel = m_GameData->GetCurrentLevel()+1;
			if(nextLevel < m_Levels.GetSize())
			{
				m_GameData->SetCurrentLevel(nextLevel);
				u32 lastUnlockedLevel = m_GameData->GetLastUnlockedLevel();
				m_GameData->SetLastUnlockedLevel(Math::Max(lastUnlockedLevel, nextLevel));
				m_GameData->Save();
			}
        }
        else
        {
            if(IsSkillzGame())
            {
                g_bSkillzAbort = true;
            }
            
            SetState(S_Menu);
            Engine::Instance()->LoadContextStack("common/menuscenecontinue.xml");
            AudioManager::Instance()->Play(AudioManager::M_Metal);
        }
#else
        if(m_bLevelCompleted)
		{
			m_bLevelCompleted = false;
			u32 nextLevel = m_GameData->GetCurrentLevel()+1;
			if(nextLevel < m_Levels.GetSize())
			{
				m_GameData->SetCurrentLevel(nextLevel);
				u32 lastUnlockedLevel = m_GameData->GetLastUnlockedLevel();
				m_GameData->SetLastUnlockedLevel(Math::Max(lastUnlockedLevel, nextLevel));
				m_GameData->Save();
			}
			else if(!m_GameData->GetOutroPlayed())
			{
				SetState(S_Outro);
				Engine::Instance()->LoadContextStack("mothership/outroscene.xml");
				m_GameData->SetOutroPlayed(true);
				m_GameData->Save();
				AudioManager::Instance()->Play(AudioManager::M_Outro);
				return;
			}
		}
        
		SetState(S_Menu);
		Engine::Instance()->LoadContextStack("common/menuscenecontinue.xml");
		AudioManager::Instance()->Play(AudioManager::M_Metal);
#endif
	}

	//! called when a leaderboard operation failed
	void GameManager::OnLeaderboardError(E_LeaderboardError error /*= LE_ConnectionError*/)
	{
		switch(error)
		{
		case LE_InvalidPass:
			m_MenuMessage = "Invalid Password";
			SetLeaderboardState(LS_InvalidPass);
			break;

		case LE_UsernameTaken:
			m_MenuMessage = "Username already exists";
			SetLeaderboardState(LS_UsernameTaken);
			break;

		default:
			m_MenuMessage = "Online Leaderboards unreachable";
			SetLeaderboardState(LS_ConnectionError);
		}

		Engine::Instance()->GetTopContext()->Switch("common/menuscorebox.xml");
	}

	//! sets the current state
	void GameManager::SetState(E_State eNewState)
	{
		Log.Print("GameManager::SetState '%s'\n", GetStateString(eNewState));
		m_eState = eNewState;

		if(m_eLeaderboardState != LS_None)
		{
			SetLeaderboardState(LS_None);
		}
	}

	//! sets the leaderboard state
	void GameManager::SetLeaderboardState(E_LeaderboardState eNewState)
	{
		Log.Print("GameManager::SetLeaderboardState '%s'\n", GetStateString(eNewState));
		m_eLeaderboardState = eNewState;
	}

	//! checks the connection with the score server
	void GameManager::OnlineCheckConnection()
	{
		m_bNewHighScore = false;
		m_bContextChangedFlag = false;
		SetLeaderboardState(LS_CheckConnection);
		ScoreoidUtils::Params params;
		ScoreoidUtils::Request("getGame", params, &NetCallback);
	}

	//! requests the player count
	void GameManager::OnlineRequestPlayerCount()
	{
		SetLeaderboardState(LS_CountPlayers);
		ScoreoidUtils::Params params;
		ScoreoidUtils::Request("countPlayers", params, &NetCallback);
	}

	//! checks the player best score
	void GameManager::OnlineCheckBestScore(bool bCheckLevelScore /*= true*/)
	{
		ScoreoidUtils::Params params;
		params["usernames"] = m_GameData->GetPlayerName();
		if(m_eState == S_InGame && bCheckLevelScore)
		{
			SetLeaderboardState(LS_CheckBestLevelScore);
			params["difficulty"] = Utils::ToString(m_GameData->GetCurrentLevel());
		}
		else
		{
			SetLeaderboardState(LS_CheckBestScore);
		}
		ScoreoidUtils::Request("getBestScores", params, &NetCallback);
	}

	//! requests the best scores
	void GameManager::OnlineRequestBestScores()
	{
		SetLeaderboardState(LS_GetBestScores);
		ScoreoidUtils::Params params;
		params["order"] = "desc";
		params["order_by"] = "score";
		params["limit"] = "100";
		ScoreoidUtils::Request("getBestScores", params, &NetCallback);
	}

	//! requests the best level scores
	void GameManager::OnlineRequestBestLevelScores()
	{
		SetLeaderboardState(LS_GetBestLevelScores);
		ScoreoidUtils::Params params;
		params["order"] = "desc";
		params["order_by"] = "score";
		params["limit"] = "100";
		params["difficulty"] = Utils::ToString(m_GameData->GetCurrentLevel());
		ScoreoidUtils::Request("getBestScores", params, &NetCallback);
	}

	//! requests the best rank
	void GameManager::OnlineRequestBestRank()
	{
		SetLeaderboardState(LS_GetRank);
		ScoreoidUtils::Params params;
		params["username"] = m_GameData->GetPlayerName();
		ScoreoidUtils::Request("getPlayerRank", params, &NetCallback);
	}

	//! requests the best level rank
	void GameManager::OnlineRequestBestLevelRank()
	{
		SetLeaderboardState(LS_GetLevelRank);
		ScoreoidUtils::Params params;
		params["username"] = m_GameData->GetPlayerName();
		params["difficulty"] = Utils::ToString(m_GameData->GetCurrentLevel());
		ScoreoidUtils::Request("getPlayerRank", params, &NetCallback);
	}

	//! sends the current score
	void GameManager::OnlineSendScore()
	{
		SetLeaderboardState(LS_SendScore);
		ScoreoidUtils::Params params;
		params["username"] = m_GameData->GetPlayerName();
		params["score"] = Utils::ToString(m_Score);
		params["platform"] = ScoreoidUtils::GetPlatformString();
		params["difficulty"] = Utils::ToString(m_GameData->GetCurrentLevel());
		ScoreoidUtils::Request("createScore", params, &NetCallback);
	}

	//! requests the player login
	void GameManager::OnlineRequestLogin()
	{
		SetLeaderboardState(LS_EnterName);
		if(InputManager::Instance()->IsVKeyboardAvailable())
		{
			InputManager::Instance()->RequestVKeyboard(&OnVKeyboardUpdated);
		}
		else
		{
			m_VKeyboardText.clear();
		}
	}

	//! called when the online account has been successfully created
	void GameManager::OnOnlineAccountCreated()
	{
		m_GameData->SetPlayerName(m_EnteredPlayerName);
		m_GameData->SetPlayerPassword(m_EnteredPlayerPass);
		m_GameData->Save();

		OnlineCheckBestScore();
	}

	//! updates the level select menu
	void GameManager::UpdateLevelSelectMenu(Entity* pContext)
	{
		u32 currentLevel = m_GameData->GetCurrentLevel();
		pContext->GetChildByName("Previous")->SetVisible(currentLevel > 0);
		pContext->GetChildByName("Next")->SetVisible(currentLevel < m_Levels.GetSize()-1);

		bool bUnlocked = true;
		if(!IsDemo())
		{
			bUnlocked = (currentLevel <= m_GameData->GetLastUnlockedLevel());
		}
		else
		{
			bUnlocked = (currentLevel <= m_GameData->GetLastUnlockedLevel()) && (currentLevel < 1);
		}

		pContext->GetChildByName("Lock")->SetVisible(!bUnlocked);
		pContext->GetChildByName("Level")->SetEnabled(bUnlocked);
		pContext->GetChildByName("Play")->SetEnabled(bUnlocked);
		pContext->GetChildByName("Play")->SetVisible(bUnlocked);
		Text* pTitle = static_cast<Text*>(pContext->GetChildByName("Title"));
		pTitle->SetText(m_Levels[currentLevel].m_Name.c_str());
		pTitle->GetComponent<TextVisitor>()->Visit(pTitle);
		Entity* pLevelImage = pContext->GetChildByName("Level");
		Texture* pTexture = ResourceManager::Instance()->GetResource<Texture>(m_Levels[currentLevel].m_Image.c_str());
		pLevelImage->GetComponent<GraphicComponent>()->GetMaterial()->SetTexture(0, pTexture);
		pContext->GetChildByName("BestLevelScore")->SetVisible(false);
				
		if(IsDemo())
		{
			if(currentLevel >= 1)
			{
				pContext->GetChildByName("Lock")->SetVisible(true);
				pContext->GetChildByName("Play")->SetVisible(false);
				pContext->GetChildByName("Scores")->SetVisible(false);
				pContext->GetChildByName("GetFullGame")->SetVisible(true);
			}
			else
			{
				pContext->GetChildByName("Scores")->SetVisible(true);
				pContext->GetChildByName("GetFullGame")->SetVisible(false);
			}
		}
		else
		{
			pContext->GetChildByName("Scores")->SetVisible(true);
			pContext->GetChildByName("GetFullGame")->SetVisible(false);
		}
				
		if(m_GameData->HasLoginInfo())
		{
			ScoreoidUtils::Params params;
			params["usernames"] = m_GameData->GetPlayerName();
			params["difficulty"] = Utils::ToString(currentLevel);
			m_bBestLevelScoresReceived = false;
			ScoreoidUtils::Request("getBestScores", params, &NetCallbackLevelScore);
		}
	}

	//! shows the score page
	void GameManager::ShowScoresPage()
	{
		m_bLevelLeaderBoard = m_bLevelScoreAvailable;
		bool bScoreAvailable = m_bLevelScoreAvailable || !m_LeaderboardScore.empty();
		if(bScoreAvailable)
		{
			if(m_bLevelScoreAvailable)
			{
				OnlineRequestBestLevelRank();
			}
			else
			{
				OnlineRequestBestRank();
			}
		}
		else
		{
			SetLeaderboardState(LS_ShowScores);
			Engine::Instance()->GetTopContext()->Switch("common/menuscore.xml");
		}
	}

	//! toggles the leaderboard
	void GameManager::ToggleLeaderboard(Entity* pContext)
	{
		m_bLevelLeaderBoard = !m_bLevelLeaderBoard;
		UpdateLeaderboard(pContext);
		static_cast<PlayerList*>(pContext->GetChildByType("PlayerList"))->Fill(true);
	}

	//! updates the leaderboard
	void GameManager::UpdateLeaderboard(Entity* pContext)
	{
		bool bScoreAvailable = m_bLevelLeaderBoard ? !m_LeaderboardLevelScore.empty() : !m_LeaderboardScore.empty();
		bool bRankAvailable = m_bLevelLeaderBoard ? (m_LeaderboardLevelIndex != -1) : (m_LeaderboardIndex != -1);
		if(bScoreAvailable)
		{
			std::string score = std::string("Best Score: ") + (m_bLevelLeaderBoard ? m_LeaderboardLevelScore : m_LeaderboardScore);
			static_cast<Text*>(pContext->GetChildByName("BestScore"))->SetText(score.c_str());
		}
		if(bRankAvailable)
		{
			std::string rank = std::string("Rank: #") + Utils::ToString(m_bLevelLeaderBoard ? (m_LeaderboardLevelIndex+1) : (m_LeaderboardIndex+1));
			static_cast<Text*>(pContext->GetChildByName("Rank"))->SetText(rank.c_str());
		}
		pContext->GetChildByName("BestScore")->SetVisible(bScoreAvailable);
		pContext->GetChildByName("Rank")->SetVisible(bRankAvailable);
		pContext->GetChildByName("TrophyText")->SetVisible(m_bLevelLeaderBoard);
		pContext->GetChildByName("SmallTrophyContainer")->SetVisible(m_bLevelScoreAvailable);
		Entity* pToggleButton = pContext->GetChildByName("ToggleLeaderboard");
		pToggleButton->SetVisible(m_bLevelScoreAvailable);
		std::string levelText = Utils::ToString(m_GameData->GetCurrentLevel()+1);
		std::string levelPrefix = std::string("LEVEL ") + levelText;
		if(m_bLevelScoreAvailable)
		{
			pContext->GetChildByName("SmallTrophy")->SetVisible(m_bLevelLeaderBoard);
			pContext->GetChildByName("SmallTrophyLevel")->SetVisible(!m_bLevelLeaderBoard);				
			pContext->GetChildByName("SmallTrophyText")->SetVisible(!m_bLevelLeaderBoard);
			
			static_cast<Text*>(pToggleButton->GetChildByName("Text"))->SetText(m_bLevelLeaderBoard ? "GLOBAL SCORES" : (levelPrefix + " SCORES").c_str());				
			static_cast<Text*>(pContext->GetChildByName("TrophyText"))->SetText(levelText.c_str());
			static_cast<Text*>(pContext->GetChildByName("SmallTrophyText"))->SetText(levelText.c_str());
		}
		static_cast<Text*>(pContext->GetChildByName("Title"))->SetText(m_bLevelLeaderBoard ? (levelPrefix + " LEADERBOARD").c_str() : "GLOBAL LEADERBOARD");

		pContext->GetChildByName("Trophy")->SetVisible(!m_bLevelLeaderBoard);
		pContext->GetChildByName("TrophyLevel")->SetVisible(m_bLevelLeaderBoard);
	}
	
	//! Called when a level finished loading
	void GameManager::OnLevelLoaded()
	{
		FileSystem::Instance()->UnloadAllPartitions("common");
		GraphicsDriver::Instance()->LoadObjects();

		GameManager* pMgr = GameManager::Instance();
		pMgr->m_bLevelCompleted = false;
		pMgr->m_Score = 0;
		pMgr->m_PreviousScore = 0;
		pMgr->m_ScoreText = static_cast<Text*>(Engine::Instance()->GetContext(1)->GetChildByName("ScoreText"));
		pMgr->m_ScoreTextContainer = static_cast<Entity2D*>(Engine::Instance()->GetContext(1)->GetChildByName("ScoreTextContainer"));
		pMgr->m_ScoreTextContainer->SetVisible(false);

		pMgr->m_GameData->IncrPlayCount();
		pMgr->m_GameData->Save();

		pMgr->SetState(S_InGame);
		AudioManager::Instance()->Play(AudioManager::M_Metal);
	}

	//! initializes a UIContext
	void GameManager::InitUIContext(Entity* pContext)
	{
		std::string path = pContext->GetTemplatePath();
		if(path == "common/menuoptions.xml")
		{
			if(UISlider* pSlider = DYNAMIC_CAST(pContext->GetChildByName("MusicSlider"), UISlider))
			{
				pSlider->SetRatio(m_GameData->GetMusicVolume());
			}
			if(UISlider* pSlider = DYNAMIC_CAST(pContext->GetChildByName("SoundSlider"), UISlider))
			{
				pSlider->SetRatio(m_GameData->GetSoundVolume());
			}

			bool bOutroAvailable = m_GameData->GetOutroPlayed();
			pContext->GetChildByName("Outro")->SetEnabled(bOutroAvailable);
			pContext->GetChildByName("OutroLock")->SetVisible(!bOutroAvailable);
		}
		else if(path == "common/menuscorebox.xml")
		{
			if(Text* pPrompt = DYNAMIC_CAST(pContext->GetChildByName("Prompt"), Text))
			{
				pPrompt->SetText(m_MenuMessage.c_str());
			}

			if(m_eLeaderboardState == LS_ConnectionError
			|| m_eLeaderboardState == LS_InvalidPass
			|| m_eLeaderboardState == LS_UsernameTaken)
			{
				pContext->GetChildByName("ErrorIcon")->SetVisible(true);
			}
		}
		else if(path == "common/menuscore.xml")
		{
			UpdateLeaderboard(pContext);

			std::string playerCount = std::string("(") + Utils::ToString(m_PlayerCount) + " Players)";
			static_cast<Text*>(pContext->GetChildByName("PlayerCount"))->SetText(playerCount.c_str());
		}
		else if(path == "common/menulevelselect.xml")
		{
			UpdateLevelSelectMenu(pContext);
		}
		else if(path == "common/menuexitconfirmation.xml")
		{
			Text* pMessage = static_cast<Text*>(pContext->GetChildByName("Message"));
			pMessage->SetText((m_eState == S_AskExitLevel) ? "EXIT LEVEL?" : "EXIT GAME?");
		}
	}

	//! shows the scores menu
	void GameManager::ShowScores()
	{
		Engine::Instance()->GetTopContext()->Switch("common/menuconnecting.xml");
		OnlineCheckConnection();
	}

	//! Called when the VKeyboard has been updated
	bool GameManager::OnVKeyboardUpdated(std::string text, bool bFinished)
    {
		GameManager* pMgr = GameManager::Instance();
        Log.Print("OnVKeyboardUpdated: %s, finished: %d\n", text.c_str(), bFinished);
		Text* pTextField = DYNAMIC_CAST(Engine::Instance()->GetTopContext()->GetChildByName("TextField"), Text);
		pTextField->SetText(text.c_str());

		if(bFinished)
		{
			if(text.empty())
			{
				return false;
			}

			switch(pMgr->m_eLeaderboardState)
			{
			case LS_EnterName:
				{
					pMgr->m_EnteredPlayerName = text;
					pTextField->SetText("");
					Text* pPrompt = DYNAMIC_CAST(Engine::Instance()->GetTopContext()->GetChildByName("Prompt"), Text);		
					pPrompt->SetText("Enter Password:");
					pPrompt->GetComponent<TextVisitor>()->Visit(pPrompt);
					pMgr->SetLeaderboardState(LS_EnterPass);
					return false;
				}

			case LS_EnterPass:
				{
					pMgr->SetLeaderboardState(LS_CheckPlayer);
					pMgr->m_EnteredPlayerPass = text;
					Engine::Instance()->GetTopContext()->Switch("common/menuconnecting.xml");
					ScoreoidUtils::Params params;
					params["username"] = pMgr->m_EnteredPlayerName;
					params["password"] = pMgr->m_EnteredPlayerPass;
					ScoreoidUtils::Request("getPlayer", params, &NetCallback);					
				}
				break;

			default:
				SHOOT_WARNING(false, "OnVKeyboardUpdated error: invalid state (%d)\n", pMgr->m_eState);				
				// TODO handle - maybe go back to mainmenu??
				break;
			}
			return true;
		}
		return false;
    }

	//! Called when a NetResult is available
	void GameManager::NetCallbackLevelScore(NetResult& result)
	{
		if(result.m_Status != NetResult::S_DataReceived)
		{
			return;
		}
		
		GameManager* pMgr = GameManager::Instance();
		std::string strResult((const char*)&result.m_Data[0]);
		if(ScoreoidUtils::GetScoreInfos(strResult, pMgr->m_BestLevelScores) && !pMgr->m_BestLevelScores.empty())
		{
			pMgr->m_bBestLevelScoresReceived = true;
		}
	}

	//! Called when a NetResult is available
	void GameManager::NetCallback(NetResult& result)
	{
		GameManager* pMgr = GameManager::Instance();
		pMgr->m_pMutex->Lock();
		pMgr->m_NetResult = result;
		pMgr->m_pMutex->Unlock();
		pMgr->m_bNetCallbackReceived = true;
	}

	//! Processes an online result
	void GameManager::OnlineProcessResult(NetResult& result)
	{
		if(result.m_Status != NetResult::S_DataReceived)
		{
			OnLeaderboardError();
			return;
		}

		std::string strResult((const char*)&result.m_Data[0], result.m_Data.size());

#ifndef _RETAIL_
		Log << strResult.c_str() << "\n";
#endif		

		switch(m_eLeaderboardState)
		{
		case LS_CheckConnection:
			if(ScoreoidUtils::IsResultOK(strResult))
			{
				bool bLoginInfoAvailable = m_GameData->HasLoginInfo();
				if(!bLoginInfoAvailable)
				{
					Engine::Instance()->GetTopContext()->Switch("common/menucreateorlogin.xml");
					SetLeaderboardState(LS_CreateOrLogin);
				}
				else
				{
					OnlineCheckBestScore();
				}
			}
			else
			{
				OnLeaderboardError();
			}
			break;

		case LS_CheckPlayer:
			{
				bool bPlayerExists = false;
				bool bPasswordMatch = false;
				ScoreoidUtils::CheckPlayerInfo(strResult, bPlayerExists, bPasswordMatch);
				if(bPlayerExists)
				{
					if(bPasswordMatch)
					{
						OnOnlineAccountCreated();
					}
					else
					{
						if(m_bIsCreatingAccount)
						{
							OnLeaderboardError(LE_UsernameTaken);
						}
						else
						{
							OnLeaderboardError(LE_InvalidPass);
						}
					}
				}
				else
				{
					// create the player
					SetLeaderboardState(LS_CreatePlayer);
					ScoreoidUtils::Params params;
					params["username"] = m_EnteredPlayerName;
					params["password"] = m_EnteredPlayerPass;
					ScoreoidUtils::Request("createPlayer", params, &NetCallback);					
				}
			}				
			break;

		case LS_CreatePlayer:
			if(ScoreoidUtils::IsResultOK(strResult))
			{
				OnOnlineAccountCreated();
			}
			else
			{
				OnLeaderboardError();
			}
			break;

		case LS_CountPlayers:
			if(ScoreoidUtils::GetPlayerCount(strResult, m_PlayerCount))
			{
				bool bMenuLevelScoreAvailable = (m_eState == S_Menu) && !m_BestLevelScores.empty();
				bool bInGameLevelScoreAvailable = (m_eState == S_InGame) && !m_LeaderboardScore.empty();
				m_bLevelScoreAvailable = (bMenuLevelScoreAvailable || bInGameLevelScoreAvailable);
				if(m_bLevelScoreAvailable)
				{
					OnlineRequestBestLevelScores();
				}
				else
				{
					OnlineRequestBestScores();
				}
			}
			else
			{
				OnLeaderboardError();
			}
			break;

		case LS_CheckBestLevelScore:
			if(ScoreoidUtils::GetScoreInfos(strResult, m_Scores))
			{
				s32 bestLevelScore = -1;
				m_LeaderboardLevelScore.clear();
				if(!m_Scores.empty())
				{
					m_LeaderboardLevelScore = m_Scores.front().m_Score;
					bestLevelScore = Utils::FromString<s32>(m_Scores.front().m_Score);
				}
				
				m_bNewHighScore = (m_Score > bestLevelScore);
				if(m_bNewHighScore)
				{
					m_LeaderboardLevelScore = Utils::ToString<s32>(m_Score);
				}

				OnlineCheckBestScore(false);
			}
			else
			{
				OnLeaderboardError();
			}
			break;

		case LS_CheckBestScore:
			if(ScoreoidUtils::GetScoreInfos(strResult, m_Scores))
			{
				m_LeaderboardScore.clear();
				if(!m_Scores.empty())
				{
					m_LeaderboardScore = m_Scores.front().m_Score;
				}

				if(m_eState == S_InGame)
				{
					if(m_bNewHighScore)
					{
						OnlineSendScore();
					}
					else
					{
						ExitLevel();
					}
				}
				else
				{
					OnlineRequestPlayerCount();
				}
			}
			else
			{
				OnLeaderboardError();
			}
			break;

		case LS_GetBestLevelScores:
			if(ScoreoidUtils::GetScoreInfos(strResult, m_LevelScores))
			{
				OnlineRequestBestScores();
			}
			else
			{
				OnLeaderboardError();
			}
			break;

		case LS_GetBestScores:
			if(ScoreoidUtils::GetScoreInfos(strResult, m_Scores))
			{
				if(m_bNewHighScore)
				{
					SetLeaderboardState(LS_NewHighScore);
					m_MenuMessage = "You got a new High Score!";
					Engine::Instance()->GetTopContext()->Switch("common/menuscorebox.xml");
				}
				else
				{
					ShowScoresPage();
				}
			}
			else
			{
				OnLeaderboardError();
			}
			break;
		
		case LS_GetLevelRank:
			if(ScoreoidUtils::GetPlayerRank(strResult, m_LeaderboardLevelIndex))
			{
				OnlineRequestBestRank();
			}
			else
			{
				OnLeaderboardError();
			}
			break;

		case LS_GetRank:
			if(ScoreoidUtils::GetPlayerRank(strResult, m_LeaderboardIndex))
			{
				SetLeaderboardState(LS_ShowScores);
				Engine::Instance()->GetTopContext()->Switch("common/menuscore.xml");
			}
			else
			{
				OnLeaderboardError();
			}
			break;

		case LS_SendScore:
			if(ScoreoidUtils::IsResultOK(strResult))
			{
				OnlineRequestPlayerCount();
			}
			else
			{
				OnLeaderboardError();
			}
			break;
		}
	}

	//! returns state string
	const char* GameManager::GetStateString(E_State state) const
	{
#define HANDLE_STATE(x) case x: return #x;
		switch(state)
		{
			HANDLE_STATE(S_Menu);
			HANDLE_STATE(S_InGame);
			HANDLE_STATE(S_InGamePause);
			HANDLE_STATE(S_AskExitLevel);
		}
		return "UnknownState";
	}

	//! returns the leaderboard state string
	const char* GameManager::GetStateString(E_LeaderboardState state) const
	{
#define HANDLE_LS_STATE(x) case x: return #x;
		switch(state)
		{
			HANDLE_LS_STATE(LS_None);
			HANDLE_LS_STATE(LS_CheckConnection);
			HANDLE_LS_STATE(LS_EnterName);
			HANDLE_LS_STATE(LS_EnterPass);
			HANDLE_LS_STATE(LS_CheckPlayer);
			HANDLE_LS_STATE(LS_CreatePlayer);
			HANDLE_LS_STATE(LS_CountPlayers);
			HANDLE_LS_STATE(LS_CheckBestScore);
			HANDLE_LS_STATE(LS_CheckBestLevelScore);
			HANDLE_LS_STATE(LS_GetRank);
			HANDLE_LS_STATE(LS_GetLevelRank);
			HANDLE_LS_STATE(LS_GetBestScores);
			HANDLE_LS_STATE(LS_GetBestLevelScores);
			HANDLE_LS_STATE(LS_NewHighScore);
			HANDLE_LS_STATE(LS_SendScore);
			HANDLE_LS_STATE(LS_ShowScores);
			HANDLE_LS_STATE(LS_ConnectionError);
			HANDLE_LS_STATE(LS_InvalidPass);
		}
		return "UnknownState";
	}

	//! returns demo status
	bool GameManager::IsDemo() const
	{
#if SHOOT_PLATFORM == SHOOT_PLATFORM_WP8
		return m_bWP8TrialMode;
#else
	#ifdef _VLAD_DEMO_
		#if SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID
			return m_bAndroidTrialMode;
		#else
			return true;
		#endif
	#else
			return false;
	#endif
#endif
	}
}

