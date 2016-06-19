/* 

Amine Rehioui
Created: August 26th 2013

*/

#ifndef _AUDIO_DRIVER_H_INCLUDED_
#define _AUDIO_DRIVER_H_INCLUDED_

#include "Sound.h"
#include "SoundBank.h"
#include "Music.h"

// forwards
class CkMixer;

namespace shoot
{
	//! AudioDriver 
	class AudioDriver : public Singleton<AudioDriver>
	{
		DECLARE_SINGLETON(AudioDriver);

	public:
	
		//! driver initialization
		void Init();

		//! driver update
		void Update();

		//! create a sound bank
		SoundBank* CreateSoundBank(const char* strBank);

		//! creates a sound
		Sound* CreateSound(SoundBank* pBank, const char* strName);

		//! create a music
		Music* CreateMusic(const char* strOGG);

		//! set music status
		void SetMusicEnabled(bool bEnabled);

		//! set sound status
		void SetSoundEnabled(bool bEnabled);

		//! set the music volume
		void SetMusicVolume(f32 fVolume);

		//! set the sound volume
		void SetSoundVolume(f32 fVolume);

		//! pauses all audio
		void Pause();

		//! resume all audio
		void Resume();

	private:

		CkMixer* m_pMusicMixer;
		CkMixer* m_pSoundMixer;
	};
}

#endif // _AUDIO_DRIVER_H_INCLUDED_
