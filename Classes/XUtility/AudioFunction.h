#ifndef __XUTILITY_AUDIO_FUNCTION_H__
#define __XUTILITY_AUDIO_FUNCTION_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

namespace XUtility
{

	class AudioManager
	{
	public:
		static AudioManager& Instance();

	public:
		void InitMusic(const char* music);
		void InitSound(const char* sound);
		void ReleaseSound(const char* sound);

	public:
		void PlayMusic(const char* music);
		void StopMusic();
		void PauseMusic();
		void ResumeMusic();
		uint32_t PlaySound(const char* sound, bool loop);
		void StopSound(uint32_t id);
		void ReleaseSound(uint32_t id);

	private:
		class AudioRes
		{
		public:
			std::string m_name;
			int m_refCount = 0;
		};

		std::list<AudioRes*> m_loadedSoundRes;
	};

}

#endif // __XUTILITY_COCOS_HELPER_H__
