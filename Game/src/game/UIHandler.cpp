/* 

Amine Rehioui
Created: October 5th 2013

*/

#include "ShootTest.h"

#include "UIHandler.h"
#include "GameManager.h"
#include "UIContext.h"
#include "Utils.h"
#include "AudioDriver.h"
#include "SequenceVisitor.h"
#include "CombineVisitor.h"
#include "FileSystem.h"
#include "AudioManager.h"
#include "VKeyboard.h"

#if SHOOT_PLATFORM == SHOOT_PLATFORM_IOS
extern bool g_bSkillzLaunch;
#endif

namespace shoot
{
	//! shoot event listeners
	void UIHandler::HandleEvent(void* pListener, Event* pEvent)
	{
		GameManager* pMgr = static_cast<GameManager*>(pListener);
		UIEvent* pUIEvent = static_cast<UIEvent*>(pEvent);
		Entity* pSender = DYNAMIC_CAST(pUIEvent->m_Sender.Get(), Entity);
		UIContext* pUIContext = pSender ? DYNAMIC_CAST(pSender->GetRootContext(), UIContext) : NULL;
		if(pUIContext && pUIContext->GetState() != UIContext::S_Idle)
		{
			Log.Print("Skipping UI Event '%s' because context is in transition", pUIEvent->m_Command.c_str());
			return;
		}

		Log << "Command: " << pUIEvent->m_Command.c_str() << "\n";

		std::vector<std::string> tokens;
		Utils::Tokenize(pUIEvent->m_Command.c_str(), " ", tokens);
		std::string command = tokens[0];

		if(command == "POPPED")
		{
			if(!pMgr->m_OnPopUIEvent.empty())
			{
				tokens.clear();
				Utils::Tokenize(pMgr->m_OnPopUIEvent.c_str(), " ", tokens);
				command = tokens[0];
				pMgr->m_OnPopUIEvent.clear();
			}
			else
			{
				return;
			}
		}

		if(command == "SWITCH")
		{
			GameManager::NavigationInfo info;
			info.m_eType = GameManager::NT_Switch;
			info.m_Context = pUIContext->GetTemplatePath();
			pMgr->m_NavigationList.push_back(info);

			std::string context = std::string("common/menu") + tokens[1] + ".xml";
			pUIContext->Switch(context.c_str());
			return;
		}

		if(command == "SWITCH_ROOT")
		{
			pMgr->m_NavigationList.clear();

			std::string context = std::string("common/menu") + tokens[1] + ".xml";
			Engine::Instance()->LoadContextStack(context.c_str());
			return;
		}

		if(command == "PUSH")
		{
			GameManager::NavigationInfo info;
			info.m_eType = GameManager::NT_Push;
			pMgr->m_NavigationList.push_back(info);

			std::string context = std::string("common/menu") + tokens[1] + ".xml";
			Engine::Instance()->PushContext(context.c_str());
			return;
		}

		if(command == "POP")
		{
			pUIContext->Pop();
			for(u32 i=1; i<tokens.size(); ++i)
			{
				pMgr->m_OnPopUIEvent = pMgr->m_OnPopUIEvent + " " + tokens[i];
			}
			return;
		}

		if(command == "BACK")
		{
			if(pMgr->m_eLeaderboardState != GameManager::LS_None)
			{
				pMgr->m_bContextChangedFlag = true;
			}

			switch(pMgr->m_eLeaderboardState)
			{
			case GameManager::LS_EnterName:
			case GameManager::LS_EnterPass:
				InputManager::Instance()->CloseVKeyboard();
				break;

			case GameManager::LS_NewHighScore:
				pMgr->ShowScoresPage();
				pMgr->m_bContextChangedFlag = false;
				return;

			case GameManager::LS_InvalidPass:
			case GameManager::LS_UsernameTaken:
				Engine::Instance()->GetTopContext()->Switch("common/menulogin.xml");
				pMgr->OnlineRequestLogin();
				pMgr->m_bContextChangedFlag = false;
				return;
			}

			if(pMgr->m_NavigationList.empty())
			{
				if(pMgr->m_eLeaderboardState != GameManager::LS_None 
				&& pMgr->m_eState != GameManager::S_Menu)
				{
					Engine::Instance()->PopContext();
					pMgr->m_OnPopUIEvent = "EXIT_LEVEL";
				}
				else
				{
					switch(pMgr->m_eState)
					{
					case GameManager::S_Outro:
						pMgr->SetState(GameManager::S_AskExitGame);
						Engine::Instance()->PushContext("common/menuexitconfirmation.xml");
						FileSystem::Instance()->UnloadPartition("mothership");
						break;

					case GameManager::S_Menu:
						pMgr->SetState(GameManager::S_AskExitGame);
						Engine::Instance()->PushContext("common/menuexitconfirmation.xml");						
						break;

					case GameManager::S_InGame:
						pMgr->SetState(GameManager::S_AskExitLevel);
						Engine::Instance()->PushContext("common/menuexitconfirmation.xml");						
						break;

					case GameManager::S_InGamePause:
						Engine::Instance()->PopContext();
						pMgr->m_OnPopUIEvent = "RESUME";
						break;

					case GameManager::S_AskExitLevel:
						Engine::Instance()->PopContext();
						pMgr->m_OnPopUIEvent = "EXIT_LEVEL";
						break;

					case GameManager::S_AskExitGame:
						Engine::Instance()->RequestExit();
						break;

					case GameManager::S_ThankYou:
						pMgr->SetState(GameManager::S_Menu);
						Engine::Instance()->PopContext();
						break;

					default:
						Log << "Unknown game state: " << pMgr->m_eState << "\n";
					}
				}
			}
			else
			{
				GameManager::NavigationInfo info = pMgr->m_NavigationList.back();
				pMgr->m_NavigationList.pop_back();

				switch(info.m_eType)
				{
				case GameManager::NT_Switch:
					Engine::Instance()->GetTopContext()->Switch(info.m_Context.c_str());
					break;

				case GameManager::NT_Push:
					Engine::Instance()->PopContext();
					break;
				}
			}
			return;
		}

		if(command == "SLIDER")
		{
			std::string slider = tokens[1];
			f32 fRatio = Utils::FromString<f32>(tokens[2]);
			if(slider == "MusicSlider")
			{
				AudioManager::Instance()->SetMusicVolume(fRatio);
				pMgr->m_GameData->SetMusicVolume(fRatio);
				pMgr->m_GameData->Save();
			}
			else if(slider == "SoundSlider")
			{
				AudioDriver::Instance()->SetSoundVolume(fRatio);
				pMgr->m_GameData->SetSoundVolume(fRatio);
				pMgr->m_GameData->Save();
			}
			return;
		}

		if(command == "NEW_GAME")
		{
			pMgr->m_GameData->SetCurrentLevel(0);
			pMgr->m_GameData->Save();
			pMgr->LoadLevel();
			return;
		}

		if(command == "CONTINUE_GAME")
		{
			pMgr->LoadLevel();
			return;
		}

		if(command == "EXIT_LEVEL")
		{
			pMgr->ExitLevel();
			return;
		}

		if(command == "PAUSE")
		{
			pMgr->Pause();
			return;
		}

		if(command == "RESUME")
		{
			pMgr->Resume();
			return;
		}

		if(command == "PLAY")
		{
			Engine::Instance()->PopContext();
			Entity* pPlayerMesh = Engine::Instance()->GetContext(0)->GetChildByName("PlayerMesh");
			pPlayerMesh->GetComponent<SequenceVisitor>()->Visit(pPlayerMesh);
			pPlayerMesh->RemoveComponent(pPlayerMesh->GetComponent<CombineVisitor>());
            //g_bSkillzLaunch = true;
			return;
		}

		if(command == "SCORES")
		{
			GameManager::NavigationInfo info;
			info.m_eType = GameManager::NT_Switch;
			info.m_Context = pUIContext->GetTemplatePath();
			pMgr->m_NavigationList.push_back(info);
			pMgr->ShowScores();
			return;
		}
		
		if(command == "VKEYBOARD_FINISHED")
		{
			if(InputManager::Instance()->IsVKeyboardAvailable())
			{
				InputManager::Instance()->OnVKeyboardUpdated("", true);
			}
			else
			{
				if(!pMgr->OnVKeyboardUpdated(pMgr->m_VKeyboardText, true))
				{
					pMgr->m_VKeyboardText.clear();
				}
			}
			return;
		}

		if(command == "LEVEL_SCROLL")
		{
			s32 direction = (tokens[1] == "1") ? 1 : -1;
			s32 newLevel = s32(pMgr->m_GameData->GetCurrentLevel())+direction;
			if(newLevel >= 0 && newLevel < s32(pMgr->GetNumLevels()))
			{
				pMgr->m_GameData->SetCurrentLevel(newLevel);
				pMgr->m_GameData->Save();
				pMgr->UpdateLevelSelectMenu(pUIContext);
			}
			return;
		}

		if(command == "GET_FULL_GAME")
		{
			pMgr->m_bGetFullGameRequest = true;
			return;
		}
		
		if(command == "PLAY_INTRO")
		{
			pMgr->m_NavigationList.clear();
			pMgr->SetState(GameManager::S_Menu);
			Engine::Instance()->LoadContextStack("mothership/introscene.xml");
			AudioManager::Instance()->Play(AudioManager::M_Intro);
			return;
		}

		if(command == "PLAY_OUTRO")
		{
			pMgr->m_NavigationList.clear();
			pMgr->SetState(GameManager::S_Outro);
			Engine::Instance()->LoadContextStack("mothership/outroscene.xml");
			AudioManager::Instance()->Play(AudioManager::M_Outro);
			return;
		}

		if(command == "CREATE_ACCOUNT")
		{
			Engine::Instance()->GetTopContext()->Switch("common/menulogin.xml");
			pMgr->m_bIsCreatingAccount = true;
			pMgr->OnlineRequestLogin();
			return;
		}

		if(command == "LOGIN")
		{
			Engine::Instance()->GetTopContext()->Switch("common/menulogin.xml");
			pMgr->m_bIsCreatingAccount = false;
			pMgr->OnlineRequestLogin();
			return;
		}

		if(command == "TOGGLE_LEADERBOARD")
		{
			pMgr->ToggleLeaderboard(pUIContext);
			return;
		}

		if(command == "VK")
		{
			std::string input = tokens[1];
			if(input == "TOGGLE")
			{
				VKeyboard::Instance()->ToggleMode();
			}
			else if(input == "BACKSPACE")
			{
				int length = pMgr->m_VKeyboardText.length();
				if(length > 0)
				{
					pMgr->m_VKeyboardText = pMgr->m_VKeyboardText.substr(0, length-1);
					pMgr->OnVKeyboardUpdated(pMgr->m_VKeyboardText, false);
				}
			}
			else
			{
				pMgr->m_VKeyboardText = pMgr->m_VKeyboardText + (input == "SPACE" ? " " : input);
				pMgr->OnVKeyboardUpdated(pMgr->m_VKeyboardText, false);
			}
			return;
		}

		Log.Print("Unknown UI command '%s'\n", pUIEvent->m_Command.c_str());
	}

	//! shoot event listeners
	void UIHandler::HandleTriggerEvent(void* pListener, Event* pEvent)
	{
		GameManager* pMgr = static_cast<GameManager*>(pListener);
		TriggerEvent* pTrigger = static_cast<TriggerEvent*>(pEvent);
		if(pTrigger->Name == "MENU_ANIM_DONE")
		{
			Engine::Instance()->PushContext("common/menulevelselect.xml");
		}
		else if(pTrigger->Name == "INTRO_FINISHED")
		{
			Engine::Instance()->LoadContextStack("common/menuscenecontinue.xml");
			FileSystem::Instance()->UnloadPartition("mothership");
		}
		else if(pTrigger->Name == "OUTRO_FINISHED")
		{
			if(!pMgr->m_GameData->GetSaidThankYou())
			{
				pMgr->SetState(GameManager::S_ThankYou);
				Engine::Instance()->LoadContextStack("common/menuscenecontinuethankyou.xml");
				pMgr->m_GameData->SetSaidThankYou(true);
				pMgr->m_GameData->Save();

				if(pMgr->m_GameData->HasLoginInfo())
				{
					ScoreoidUtils::Params params;
					params["username"] = pMgr->m_GameData->GetPlayerName();
					params["key"] = "completed_v1.0";
					params["value"] = "1";
					ScoreoidUtils::Request("setPlayerData", params, NULL);
				}
			}
			else
			{
				pMgr->SetState(GameManager::S_Menu);
				Engine::Instance()->LoadContextStack("common/menuscenecontinue.xml");
			}
			FileSystem::Instance()->UnloadPartition("mothership");
		}
	}
}

