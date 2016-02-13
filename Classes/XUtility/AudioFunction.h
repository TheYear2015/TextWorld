#ifndef __XUTILITY_AUDIO_FUNCTION_H__
#define __XUTILITY_AUDIO_FUNCTION_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include <array>
#include <string>

namespace XUtility
{

	class AudioManager
	{
	public:
		static AudioManager& Instance();

	public:
		AudioManager();
		void InitMusic(const char* music);
		void InitSound(const char* sound);
		void ReleaseAllChannelSound();
	public:
		void PlayMusic(const char* music);
		void StopMusic();
		void PauseMusic();
		void ResumeMusic();
		uint32_t PlaySound(const char* sound, bool loop);
		void StopSound(uint32_t id);

	public:
		void PlaySoundInChannel(int channel, const char* sound, bool loop);
		void StopSoundChannel(int channel);
		void StopAllChannel();

	private:
		class AudioRes
		{
		public:
			std::string m_name;
		};

		std::list<AudioRes*> m_loadedSoundRes;

		std::array<std::string, 3> m_channelSoundName;
		std::array<uint32_t, 3> m_channelArray;
	};

}

#endif // __XUTILITY_COCOS_HELPER_H__
