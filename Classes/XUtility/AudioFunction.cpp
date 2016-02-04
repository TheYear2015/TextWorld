#include "BaseScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "AudioFunction.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace cocostudio::timeline;

namespace XUtility
{

	std::string GetSoundEffectName(const char* effect)
	{
		std::string name = "audio/";
		name.append(effect);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		name.append(".ogg");
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		name.append(".caf");
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		name.append(".ogg");
#endif
		return name;
	}

	std::string GetMusicName(const char* music)
	{
		std::string name = "audio/";
		name.append(music);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		name.append(".mp3");
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		name.append(".mp3");
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		name.append(".mp3");
#endif
		return name;
	}

	void AudioManager::InitMusic(const char* music)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(GetMusicName(music).c_str());
	}

	void AudioManager::InitSound(const char* sound)
	{
		for (auto sr : m_loadedSoundRes)
		{
			if (sr->m_name.compare(sound) == 0)
			{
				++sr->m_refCount;
				return;
			}
		}

		auto sr = new AudioRes();
		sr->m_name = sound;
		++sr->m_refCount;
		m_loadedSoundRes.push_back(sr);

		CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(GetSoundEffectName(sound).c_str());
	}

	void AudioManager::PlayMusic(const char* music)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(GetMusicName(music).c_str(), true);
	}

	void AudioManager::StopMusic()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(false);
	}

	AudioManager& AudioManager::Instance()
	{
		static AudioManager _inst;
		return _inst;
	}

	void AudioManager::PauseMusic()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	}

	void AudioManager::ResumeMusic()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}

	uint32_t AudioManager::PlaySound(const char* sound, bool loop)
	{
		bool enable = false;
		for (auto sr : m_loadedSoundRes)
		{
			if (sr->m_name.compare(sound) == 0)
			{
				enable = true;
				break;
			}
		}
		if (enable)
			return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(GetSoundEffectName(sound).c_str(), loop);
		return 0;
	}

	void AudioManager::StopSound(uint32_t id)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(id);
	}

	void AudioManager::ReleaseSound(const char* sound)
	{
		for (auto sr = m_loadedSoundRes.begin(); sr != m_loadedSoundRes.end(); ++sr)
		{
			if ((*sr)->m_name.compare(sound) == 0)
			{
				--(*sr)->m_refCount;
				if ((*sr)->m_refCount <= 0)
				{
					CocosDenshion::SimpleAudioEngine::getInstance()->unloadEffect(GetSoundEffectName(sound).c_str());
					delete *sr;
					m_loadedSoundRes.erase(sr);
				}
				break;
			}
		}

	}

	void AudioManager::PlaySoundInChannel(int channel, const char* sound, bool loop)
	{
		//Í£µôÏÖÔÚµÄ
		if (channel >= 0 && channel < m_channelArray.size())
		{
			if (m_channelSoundName[channel].compare(sound) == 0)
			{
				return;
			}
			StopSound(m_channelArray[channel]);
			m_channelArray[channel] = 0;
			m_channelSoundName[channel] = "";

			auto id = PlaySound(sound, loop);
			if (id)
			{
				m_channelArray[channel] = id;
				m_channelSoundName[channel] = sound;
			}
		}
	}

	void AudioManager::StopSoundChannel(int channel)
	{
		if (channel >= 0 && channel < m_channelArray.size())
		{
			if (m_channelArray[channel])
			{
				StopSound(m_channelArray[channel]);
				m_channelArray[channel] = 0;
				m_channelSoundName[channel] = "";
			}
		}
	}

	AudioManager::AudioManager()
	{
		m_channelArray.fill(0);
	}

	void AudioManager::StopAllChannel()
	{
		for (int i = 0; i < m_channelArray.size(); ++i)
		{
			StopSoundChannel(m_channelArray[i]);
		}
		m_channelArray.fill(0);

	}

}