/* 

Amine Rehioui
Created: August 28th 2013

*/

#ifndef _AUDIO_MANAGER_H_INCLUDED_
#define _AUDIO_MANAGER_H_INCLUDED_

#include "Sound.h"
#include "Music.h"

namespace shoot
{
	//! AudioManager
	class AudioManager : public Entity
	{
		DECLARE_OBJECT(AudioManager, Entity);

	public:

		//! E_Sound
		enum E_Sound
		{
			S_Pulse,
			S_Laser,
			S_Explosion,
			S_LongExplosion,
			S_None,
			S_Count
		};

		//! E_Music
		enum E_Music
		{
			M_Metal,
			M_Intro,
			M_Outro,
			M_Count,
			M_None
		};

		//! constructor
		AudioManager();

		//! destructor
		virtual ~AudioManager();

		//! serializes the entity to/from a PropertyStream
		void Serialize(PropertyStream& stream);

		//! called during the update of the entity
		void Update();

		//! Plays a sound
		void Play(E_Sound sound, bool bStopIfPlaying = false);

		//! Plays a music
		void Play(E_Music music);

		//! sets the music volume
		void SetMusicVolume(f32 fVolume);

		//! returns the instance
		inline static AudioManager* Instance() { return ms_pInstance; }

	private:

		//! picks a random metal track
		Music* PickMetalTrack();

		static AudioManager* ms_pInstance;

		//! SoundInfo
		struct SoundInfo : public ISerializableStruct
		{
			//! constructor
			SoundInfo()
				: m_Sound(S_None)
			{
			}

			//! Reads/Writes struct properties from/to a stream
			void Serialize(PropertyStream& stream);

			E_Sound m_Sound;
			std::string m_Bank;
			Array<std::string> m_Names;
		};		

		std::map<std::string, Reference<SoundBank> > m_SoundBankMap;		
		std::map<E_Sound, std::vector< Reference<Sound> > > m_SoundMap;

		E_Music m_MusicType;
		Reference<Music> m_Music;		
		std::vector<const char*> m_MetalTracks;
		std::vector<u32> m_RemainingMetalTracks;
		f32 m_fMusicVolume;

		//! returns the SoundInfo
		SoundInfo* GetSoundInfo(E_Sound sound);

		// properties
		Array<SoundInfo> m_SoundInfos;
		f32 m_fInGameMusicFactor;
	};
}

#endif // _AUDIO_MANAGER_H_INCLUDED_

