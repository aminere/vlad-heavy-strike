/* 

Amine Rehioui
Created: August 28th 2013

*/

#ifndef _MUSIC_H_INCLUDED_
#define _MUSIC_H_INCLUDED_

// forwards
class CkSound;

namespace shoot
{
	//! Music
	class Music : public Resource
	{
		DECLARE_OBJECT(Music, Resource);

	public:

		//! constructor
		Music() : m_pMusic(NULL)
		{
			SHOOT_ASSERT(false, "Invalid Constructor");
		}

		//! constructor
		Music(const char* strOGG);

		//! destructor
		virtual ~Music();

		//! plays the music
		void Play();

		//! stops the music
		void Stop();

		//! returns the playing status
		bool IsPlaying();

		//! returns the cricket sound
		inline CkSound* GetSound() { return m_pMusic; }

	private:

		bool m_bStartedPlaying;
		CkSound* m_pMusic;
	};
}

#endif // _MUSIC_H_INCLUDED_

