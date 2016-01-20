#include "GameLogic.h"
#include "platform/CCFileUtils.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

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

	void GameCore::SetInterface(GameLogicInterface* inf)
	{
		m_interface = inf;
	}

	void GameCore::Begin()
	{
		if (m_interface)
			m_interface->OnGameBegin();
	}

	std::vector<const StageData*> GameCore::GetPlayedStageList() const
	{
		std::vector<const StageData*> rev;
		for (auto id : m_userData.m_playedStages)
		{
			auto sd = GameLogic::DataManager::Instance().GetStageData(id);
			if (sd)
			{
				rev.push_back(sd);
			}
		}

		return rev;
	}

	std::vector<const StageActionData*> GameCore::GetPlayingActionList() const
	{
		std::vector<const StageActionData*> rev;
		if (!m_userData.m_playedStages.empty())
		{
			auto id = m_userData.m_playedStages.back();
			auto sd = GameLogic::DataManager::Instance().GetStageData(id);
			for (auto& a : sd->ActionList())
			{
				rev.push_back(&a);
			}
		}
		return rev;
	}

	void GameCore::Load()
	{
		m_userData.Reset();


		auto path = cocos2d::FileUtils::getInstance()->getWritablePath();
		path.append("userdata.json");

		auto data = cocos2d::FileUtils::getInstance()->getStringFromFile(path);

		rapidjson::Document doc;
		doc.Parse<0>(data.c_str());

		if (doc.HasParseError())
		{
			CCLOG("DataManager::ReadGameDataFromFile parse json error!");
			return;
		}
		if (doc.HasMember("Stages"))
		{
			auto& stageListValue = doc["Stages"];
			if (stageListValue.IsArray() && stageListValue.Size() > 0)
			{
				int count = stageListValue.Size();
				for (int i = 0; i < count; ++i)
				{
					m_userData.m_playedStages.push_back(stageListValue[i].GetInt());
				}
			}

			auto& time = doc["Time"];
			m_userData.m_currentStageTime = time.GetInt();
		}
	}

	void GameCore::Save()
	{
		rapidjson::Document doc;
		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
		doc.AddMember("Time", m_userData.m_currentStageTime, allocator);

		rapidjson::Value stages(rapidjson::kArrayType);

		for (auto id : m_userData.m_playedStages)
			stages.PushBack(id, allocator);

		doc.AddMember("Stages", stages, allocator);

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		doc.Accept(writer);

		auto path = cocos2d::FileUtils::getInstance()->getWritablePath();
		path.append("userdata.json");
		cocos2d::FileUtils::getInstance()->writeStringToFile(buffer.GetString(), path);
	}

}