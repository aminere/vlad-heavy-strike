/* 

Amine Rehioui
Created: June 15th 2013

*/

#include "ShootTest.h"

#include "GameData.h"

#include "File.h"

namespace shoot
{
	DEFINE_OBJECT(GameData);

	//! statics
	const char* GameData::m_sSaveFile = "GameData.xml";

	//! constructor
	GameData::GameData()
		// properties
		: m_PlayCount(0)
		, m_bIntroPlayed(false)
		, m_bOutroPlayed(false)
		, m_CurrentLevel(0)
		, m_LastUnlockedLevel(0)
		, m_fMusicVolume(1.0f)
		, m_fSoundVolume(1.0f)
		, m_bSaidThankYou(false)
	{
	}

	//! destructor
	GameData::~GameData()
	{
	}
	
	//! serializes the entity to/from a PropertyStream
	void GameData::Serialize(PropertyStream& stream)
	{
		stream.Serialize(PT_UInt, "PlayCount", &m_PlayCount);
		stream.Serialize(PT_Bool, "IntroPlayed", &m_bIntroPlayed);
		stream.Serialize(PT_Bool, "OutroPlayed", &m_bOutroPlayed);
		stream.Serialize(PT_String, "PlayerName", &m_PlayerName);
		stream.Serialize(PT_String, "PlayerPassword", &m_PlayerPassword);
		stream.Serialize(PT_UInt, "CurrentLevel", &m_CurrentLevel);
		stream.Serialize(PT_UInt, "LastUnlockedLevel", &m_LastUnlockedLevel);		
		stream.Serialize(PT_Float, "MusicVolume", &m_fMusicVolume);
		stream.Serialize(PT_Float, "SoundVolume", &m_fSoundVolume);
		stream.Serialize(PT_Bool, "SaidThankYou", &m_bSaidThankYou);
	}

	//! Saves the game data
	void GameData::Save()
	{
		File::UseWritablePath uwp;
		super::SaveToXML(m_sSaveFile);
	}
}

