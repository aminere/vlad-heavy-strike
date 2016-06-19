/* 

Amine Rehioui
Created: June 15th 2013

*/

#ifndef _GAME_DATA_H_INCLUDED_
#define _GAME_DATA_H_INCLUDED_

namespace shoot
{
	//! GameData
	class GameData : public Entity
	{
		DECLARE_OBJECT(GameData, Entity);

	public:

		//! save file name
		static const char* m_sSaveFile;

		//! constructor
		GameData();

		//! destructor
		~GameData();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! returns the current level
		inline u32 GetCurrentLevel() const { return m_CurrentLevel; }

		//! sets the current level
		inline void SetCurrentLevel(u32 level) { m_CurrentLevel = level; }

		//! returns the last unlocked level
		inline u32 GetLastUnlockedLevel() const { return m_LastUnlockedLevel; }

		//! sets the last unlocked level
		inline void SetLastUnlockedLevel(u32 level) { m_LastUnlockedLevel = level; }

		//! returns the music volume
		inline f32 GetMusicVolume() const { return m_fMusicVolume; }

		//! returns the sound volume
		inline f32 GetSoundVolume() const { return m_fSoundVolume; }

		//! sets the music volume
		inline void SetMusicVolume(f32 fVolume) { m_fMusicVolume = fVolume; }

		//! sets the sound volume
		inline void SetSoundVolume(f32 fVolume) { m_fSoundVolume = fVolume; }

		//! sets the player name
		inline void SetPlayerName(const std::string& name) { m_PlayerName = name; }

		//! gets the player name
		inline const std::string& GetPlayerName() const { return m_PlayerName; }

		//! sets the player password
		inline void SetPlayerPassword(const std::string& password) { m_PlayerPassword = password; }

		//! gets the player password
		inline const std::string& GetPlayerPassword() const { return m_PlayerPassword; }

		//! returns login info status
		inline bool HasLoginInfo() const { return !m_PlayerName.empty() && !m_PlayerPassword.empty(); }

		//! increments the play count
		inline void IncrPlayCount() { ++m_PlayCount; }

		//! gets the play count
		u32 GetPlayCount() const { return m_PlayCount; }

		//! gets intro play status
		inline bool GetIntroPlayed() const { return m_bIntroPlayed; }

		//! sets intro play status
		inline void SetIntroPlayed(bool bPlayed) { m_bIntroPlayed = bPlayed; }

		//! gets intro play status
		inline bool GetOutroPlayed() const { return m_bOutroPlayed; }

		//! sets intro play status
		inline void SetOutroPlayed(bool bPlayed) { m_bOutroPlayed = bPlayed; }

		//! gets said thank you status
		inline bool GetSaidThankYou() const { return m_bSaidThankYou; }

		//! sets said thank you status
		inline void SetSaidThankYou(bool bSaid) { m_bSaidThankYou = bSaid; }

		//! Saves the game data
		void Save();

	private:

		// properties
		u32 m_PlayCount;
		bool m_bIntroPlayed;
		bool m_bOutroPlayed;
		std::string m_PlayerName;
		std::string m_PlayerPassword;
		u32 m_CurrentLevel;
		u32 m_LastUnlockedLevel;
		f32 m_fMusicVolume;
		f32 m_fSoundVolume;
		bool m_bSaidThankYou;
	};
}

#endif // _GAME_DATA_H_INCLUDED_

