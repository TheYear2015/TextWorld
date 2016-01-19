#include "GameLogic.h"

namespace GameLogic
{

	void UserGameData::EnterStage(uint32_t id)
	{
		m_playedStages.push_back(id);
		m_currentStageTime = 0;
	}

	const std::vector<uint32_t>& UserGameData::GetPlayedStages() const
	{
		return m_playedStages;
	}

	void UserGameData::Reset()
	{
		m_playedStages.clear();
		m_currentStageTime = 0;
	}

	GameCore::GameCore()
	{

	}

	GameCore::~GameCore()
	{

	}

	GameCore& GameCore::Instance()
	{
		static GameCore _inst;
		return _inst;
	}

	void GameCore::New()
	{
		m_playingTimeMS = 0;
		m_userData.Reset();

	}

	void GameCore::SaveUserDataToFile()
	{

	}

}