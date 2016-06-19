/* 

Amine Rehioui
Created: October 11th 2011

*/

#ifndef _GAME_MANAGER_H_INCLUDED_
#define _GAME_MANAGER_H_INCLUDED_

#include "GameData.h"
#include "NetResult.h"
#include "ScoreoidUtils.h"

namespace shoot
{
	// forwards
	class Text;

	//! GameManager
	class GameManager : public Entity
	{
		DECLARE_OBJECT(GameManager, Entity);
		friend class UIHandler;

	public:

		//! game state
		enum E_State
		{
			S_Menu,
			S_InGame,
			S_InGamePause,
			S_AskExitLevel,
			S_AskExitGame,
			S_ThankYou,
			S_Outro
		};

		//! game leaderboard state
		enum E_LeaderboardState
		{
			LS_None,
			LS_CheckConnection,
			LS_CreateOrLogin,
			LS_EnterName,
			LS_EnterPass,
			LS_CheckPlayer,
			LS_CreatePlayer,
			LS_CountPlayers,
			LS_CheckBestScore,
			LS_CheckBestLevelScore,
			LS_GetRank,
			LS_GetLevelRank,
			LS_GetBestScores,
			LS_GetBestLevelScores,
			LS_NewHighScore,
			LS_SendScore,
			LS_ShowScores,
			LS_ConnectionError,
			LS_InvalidPass,
			LS_UsernameTaken
		};

		//! leaderboard error
		enum E_LeaderboardError
		{
			LE_ConnectionError,
			LE_InvalidPass,
			LE_UsernameTaken,
		};

		//! LevelInfo
		struct LevelInfo : public ISerializableStruct
		{
			void Serialize(PropertyStream& stream)
			{
				stream.Serialize(PT_File, "Path", &m_Path);
				stream.Serialize(PT_File, "Image", &m_Image);
				stream.Serialize(PT_String, "Name", &m_Name);
			}

			std::string m_Path;
			std::string m_Image;
			std::string m_Name;
		};

		//! constructor
		GameManager();

		//! destructor
		virtual ~GameManager();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! called during the initialization of the entity
		void Init();

		//! called during the update of the entity
		void Update();

		//! returns true if a point is out of the play field
		bool IsOutOfPlayfield(const Vector3& vPoint);

		//! returns the score
		inline s32 GetScore() const { return m_Score; }

		//! increments the score
		inline void IncrScore(u32 amount) { m_Score += amount; }

		//! sets the score
		inline void SetScore(s32 score) { m_Score = score; }

		//! returns the scores
		const std::list<ScoreoidUtils::ScoreInfo>& GetScores() const { return m_bLevelLeaderBoard ? m_LevelScores : m_Scores; }

		//! returns the index in the leaderboard
		inline s32 GetIndexInLeaderboard() const { return m_bLevelLeaderBoard ? m_LeaderboardLevelIndex : m_LeaderboardIndex; }

		//! initializes a UIContext
		void InitUIContext(Entity* pContext);

		//! returns the instance
		inline static GameManager* Instance() { return ms_pInstance; }

        //! Loads the current level
		void LoadLevel();
        
		//! called when a level is finished
		void OnLevelFinished();

		//! pauses the game
		void Pause();

		//! shows the scores menu
		void ShowScores();

		//! returns the level count
		inline u32 GetNumLevels() const { return m_Levels.GetSize(); }

		//! returns the game data
		inline GameData* GetGameData() const { return m_GameData; }

		//! returns the UI transition duration
		inline f32 GetUITransitionDuration() const { return m_fUITransitionDuration; }

		//! returns the UI transition origin from top
		inline f32 GetUITransitionOriginTop() const { return m_fUITransitionOriginTop; }

		//! returns the UI transition origin from bottom
		inline f32 GetUITransitionOriginBottom() const { return m_fUITransitionOriginBottom; }

		//! returns the game state
		inline E_State GetState() const { return m_eState; }

        //! returns true if the get full game button has been pressed
        inline bool HasGetFullGameRequest() const { return m_bGetFullGameRequest; }

		//! resets the get full game status
		inline void ResetGetFullGameRequest() { m_bGetFullGameRequest = false; }

#if SHOOT_PLATFORM == SHOOT_PLATFORM_WP8
		inline void SetWP8TrialMode(bool bTrialMode) { m_bWP8TrialMode = bTrialMode; }
#endif

#if SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID
		inline void SetAndroidTrialMode(bool bTrialMode) { m_bAndroidTrialMode = bTrialMode; }
#endif

	private:

		static GameManager* ms_pInstance;
		E_State m_eState;
		E_LeaderboardState m_eLeaderboardState;
		std::string m_LeaderboardScore;
		std::string m_LeaderboardLevelScore;
		s32 m_LeaderboardIndex;
		s32 m_LeaderboardLevelIndex;
		s32 m_PreviousScore;
		s32 m_Score;
		bool m_bNewHighScore;
		Handle<Text> m_ScoreText;
		Handle<Entity2D> m_ScoreTextContainer;
		std::string m_EnteredPlayerName;
		std::string m_EnteredPlayerPass;
		std::string m_MenuMessage;
		std::list<ScoreoidUtils::ScoreInfo> m_Scores;
		std::list<ScoreoidUtils::ScoreInfo> m_LevelScores;
		std::list<ScoreoidUtils::ScoreInfo> m_BestLevelScores;
		bool m_bBestLevelScoresReceived;
		s32 m_PlayerCount;
		bool m_bLevelCompleted;
		bool m_bGetFullGameRequest;
		std::string m_VKeyboardText;
		bool m_bContextChangedFlag;
		bool m_bNetCallbackReceived;
		NetResult m_NetResult;
		NetResult m_LocalNetResult;
		Mutex* m_pMutex;
		bool m_bIsCreatingAccount;
		bool m_bLevelLeaderBoard;
		bool m_bLevelScoreAvailable;

#if SHOOT_PLATFORM == SHOOT_PLATFORM_WP8
		bool m_bWP8TrialMode;
#endif

#if SHOOT_PLATFORM == SHOOT_PLATFORM_ANDROID
		bool m_bAndroidTrialMode;
		bool m_bPreviousAndroidTrialMode;
#endif

		Reference<GameData> m_GameData;
		std::string m_OnPopUIEvent;

		//! Navigation type
		enum E_NavigationType
		{
			NT_Switch,
			NT_Push
		};

		//! Navigation info
		struct NavigationInfo
		{
			E_NavigationType m_eType;
			std::string m_Context;
		};

		std::list<NavigationInfo> m_NavigationList;

		//! resumes the game
		void Resume();

		//! exits the current level
		void ExitLevel();

		//! called when a leaderboard operation failed
		void OnLeaderboardError(E_LeaderboardError error = LE_ConnectionError);

		//! sets the current state
		void SetState(E_State eNewState);

		//! sets the leaderboard state
		void SetLeaderboardState(E_LeaderboardState eNewState);

		//! checks the connection with the score server
		void OnlineCheckConnection();

		//! requests the player count
		void OnlineRequestPlayerCount();

		//! checks the player best score
		void OnlineCheckBestScore(bool bCheckLevelScore = true);

		//! requests the best scores
		void OnlineRequestBestScores();

		//! requests the best level scores
		void OnlineRequestBestLevelScores();

		//! requests the best rank
		void OnlineRequestBestRank();

		//! requests the best level rank
		void OnlineRequestBestLevelRank();

		//! sends the current score
		void OnlineSendScore();

		//! requests the player login
		void OnlineRequestLogin();

		//! called when the online account has been successfully created
		void OnOnlineAccountCreated();

		//! Processes an online result
		void OnlineProcessResult(NetResult& result);
		
		//! updates the level select menu
		void UpdateLevelSelectMenu(Entity* pContext);

		//! shows the score page
		void ShowScoresPage();

		//! toggles the leaderboard
		void ToggleLeaderboard(Entity* pContext);

		//! updates the leaderboard
		void UpdateLeaderboard(Entity* pContext);

		//! Called when a level finished loading
		static void OnLevelLoaded();

		//! Called when a NetResult is available
		static void NetCallback(NetResult& result);

		//! Called when a NetResult is available
		static void NetCallbackLevelScore(NetResult& result);

		//! Called when the VKeyboard has been updated
		static bool OnVKeyboardUpdated(std::string text, bool bFinished);

		//! returns the state string
		const char* GetStateString(E_State state) const;

		//! returns the leaderboard state string
		const char* GetStateString(E_LeaderboardState state) const;

		//! returns demo status
		bool IsDemo() const;

		// properties
		f32 m_fUITransitionDuration;
		float m_fUITransitionOriginTop;
		float m_fUITransitionOriginBottom;

		Array<LevelInfo> m_Levels;
	};
}

#endif // _GAME_MANAGER_H_INCLUDED_

