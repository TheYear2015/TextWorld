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
		m_actionList.reserve(10000);
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
		m_actionList.clear();

		if (m_interface)
			m_interface->OnGameBegin();

		//如果是新游戏，开始第一个场景(id 1)
		if (m_userData.m_playedStages.empty())
		{
			m_playedActionIndex = 0;

			EnterStage(1);
		}
		else//加载以前的数据
		{
			ActionNode actionN;
			m_currentStageTime = m_userData.m_currentStageTime;
			auto stageList = GetPlayedStageList();
			for (auto sd : stageList)
			{
				uint32_t endTime = 0;
				actionN.m_stage = sd;
				for (int i = 0; i < (int)sd->ActionList().size(); ++i)
				{
					actionN.m_isChoose = false;
					actionN.m_action = &(sd->ActionList()[i]);
					endTime += actionN.m_action->DuringMS();
					actionN.m_endTime = endTime;
					m_actionList.push_back(actionN);
				}
				if (sd->IsHaveChooseAtEnd())
				{
					actionN.m_isChoose = true;
					actionN.m_action = nullptr;
					m_actionList.push_back(actionN);
				}
			}
		}
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
			CCLOG("GameCore::Load() parse json error!");
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

	void GameCore::Update(float dt)
	{
		auto& currentAc = m_actionList[m_playedActionIndex];
		if (currentAc.m_isChoose)
		{
			//继续等待玩家选择
		}
		else
		{
			m_currentStageTime += (dt * 1000);
			//判断是否要进入下一个aciton
			if (m_currentStageTime >= currentAc.m_endTime)
			{
				//进入下一个action
				++m_playedActionIndex;
				if (m_playedActionIndex >= m_actionList.size())
				{//游戏正常结束
					//TODO::游戏正常结束
					if (m_interface)
					{
						m_interface->OnGameOK();
					}
				}
				else
				{
					auto& newAc = m_actionList[m_playedActionIndex];
					if (newAc.m_isChoose)
					{
						if (m_interface)
						{
							m_interface->OnNeedChoose(newAc.m_stage);
						}
					}
					else
					{
						if (m_interface)
						{
							m_interface->OnEnterAction(newAc.m_stage, newAc.m_action);
						}
					}

				}
			}
		}
	}

	void GameCore::EnterStage(int id)
	{
		m_currentStageTime = 0;
		ActionNode actionN;
		auto sd = DataManager::Instance().GetStageData(id);
		uint32_t endTime = 0;
		actionN.m_stage = sd;
		for (int i = 0; i < (int)sd->ActionList().size(); ++i)
		{
			actionN.m_isChoose = false;
			actionN.m_action = &(sd->ActionList()[i]);
			endTime += actionN.m_action->DuringMS();
			actionN.m_endTime = endTime;
			m_actionList.push_back(actionN);
		}
		if (sd->IsHaveChooseAtEnd())
		{
			actionN.m_isChoose = true;
			actionN.m_action = nullptr;
			m_actionList.push_back(actionN);
		}

		if (m_interface)
			m_interface->OnEnterStage(sd);

		if (!sd->ActionList().empty())
		{
			if (m_interface)
				m_interface->OnEnterAction(sd, &(sd->ActionList()[0]));
		}
		else
		{
			if (m_interface)
				m_interface->OnNeedChoose(sd);
		}

	}

	//进行选择
	void GameCore::ChooseAction(int index)
	{
		//进入下一个场景
		auto& currentAc = m_actionList[m_playedActionIndex];
		if (currentAc.m_isChoose)
		{
			if (index >= 0 && index < currentAc.m_stage->ToStage().size())
			{
				EnterStage(currentAc.m_stage->ToStage()[index].first);
			}
		}
	}

};