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
		m_actionList.reserve(100000);
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
		m_state = GameState::Ready;
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
		m_state = GameState::Playing;

		m_actionList.clear();

		if (m_interface)
			m_interface->OnGameBegin();

		//如果是新游戏，开始第一个场景(id 1)
		if (m_userData.m_playedStages.empty())
		{
			m_playedActionIndex = -1;
			EnterStage(1);
		}
		else//加载以前的数据
		{
			ActionNode actionN;
			m_currentStageTime = m_userData.m_currentStageTime;
			auto stageList = GetPlayedStageList();
			ActionNode* lastN = nullptr;
			uint32_t endTime = 0;
			for (auto sd : stageList)
			{
				if (lastN && lastN->m_action == nullptr)
				{
					for (int __i = 0; __i < lastN->m_stage->ToStage().size(); ++__i)
					{
						if (sd->Id() == lastN->m_stage->ToStage()[__i].first)
						{
							lastN->m_chooseIndex = __i;
							break;
						}
					}
				}
				endTime = 0;
				actionN.m_stage = sd;
				for (int i = 0; i < (int)sd->ActionList().size(); ++i)
				{
					actionN.m_action = &(sd->ActionList()[i]);
					endTime += actionN.m_action->DuringMS();
					actionN.m_endTime = endTime;
				}
				if (sd->IsHaveChooseAtEnd())
				{
					actionN.m_action = nullptr;
					actionN.m_chooseIndex = -1;
				}
				m_actionList.push_back(actionN);
				lastN = &m_actionList.back();
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
		m_state = GameState::Ready;

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
		if (m_state == GameState::Playing)
		{
			auto& currentAc = m_actionList.at(m_playedActionIndex);
			if (currentAc.GetType() == ActionNodeType::Choosing)
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
						m_state = GameState::End;
						if (m_interface)
						{
							m_interface->OnGameOK();
						}
					}
					else
					{
						auto& newAc = m_actionList[m_playedActionIndex];
						if (newAc.GetType() == ActionNodeType::Choosing)
						{
							//判断是否是自动选择
							if (m_actionList.size() < MaxActionCount() && newAc.m_stage->AutoNext())
							{//进行自动选择
								ChooseAction(0);
							}
							else
							{
								if (m_interface)
								{
									m_interface->OnNeedChoose(&newAc);
								}
							}
						}
						else
						{
							if (m_interface)
							{
								m_interface->OnEnterAction(&newAc);
							}
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
			actionN.m_action = &(sd->ActionList()[i]);
			endTime += actionN.m_action->DuringMS();
			actionN.m_endTime = endTime;
			m_actionList.push_back(actionN);
		}
		if (sd->IsHaveChooseAtEnd())
		{
			actionN.m_action = nullptr;
			m_actionList.push_back(actionN);
		}

		if (m_interface)
			m_interface->OnEnterStage(sd);

		++m_playedActionIndex;
		if (m_playedActionIndex >= m_actionList.size())
		{//游戏正常结束
			//TODO::游戏正常结束
			m_playedActionIndex = m_actionList.size();
			m_state = GameState::End;
			if (m_interface)
			{
				m_interface->OnGameOK();
			}
		}
		else if (!sd->ActionList().empty())
		{
			if (m_interface)
				m_interface->OnEnterAction(&m_actionList[m_playedActionIndex]);
		}
		else
		{
			if (m_interface)
				m_interface->OnNeedChoose(&m_actionList[m_playedActionIndex]);
		}
	}

	//进行选择
	void GameCore::ChooseAction(int index)
	{
		if (IsPlaying())
		{
			//进入下一个场景
			auto& currentAc = m_actionList[m_playedActionIndex];
			if (currentAc.GetType() == ActionNodeType::Choosing)
			{
				if (index >= 0 && index < currentAc.m_stage->ToStage().size())
				{
					currentAc.m_chooseIndex = index;
					EnterStage(currentAc.m_stage->ToStage()[index].first);
				}
			}
		}
	}

	void GameCore::Resume()
	{
		if (m_state == GameState::Paused)
			m_state = GameState::Playing;
	}

	void GameCore::Pause()
	{
		if (m_state == GameState::Playing)
			m_state = GameState::Paused;

	}

	bool GameCore::IsPlaying() const
	{
		return m_state == GameState::Playing;
	}

	const StageData* GameCore::GetCurrentStage() const
	{
		if (IsPlaying())
		{
			return m_actionList.at(m_playedActionIndex).m_stage;
		}
		return nullptr;
	}

	void GameCore::BackToAction(const ActionNode* action)
	{
		for (auto i = m_actionList.begin(); i != m_actionList.end(); ++i)
		{
			if (action == &(*i))
			{
				i->m_chooseIndex = -1;
				++i;
				for (; i != m_actionList.end(); ++i)
				{
					if (action->m_stage != i->m_stage)
					{
						m_actionList.erase(i, m_actionList.end());
						//重置m_playedActionIndex
						m_playedActionIndex = m_actionList.size() - 1;
						//重置时间
						if (m_interface)
							m_interface->OnBackToAction(action);
						break;
					}
				}
				break;
			}
		}
	}

	const StageActionData::SoundChannelDef* GameCore::GetActionSoundDef(const ActionNode* action) const
	{
		if (!action)
			return nullptr;

		if (action->m_action
			&& action->m_action->SoundChannel())
		{
			return action->m_action->SoundChannel();
		}

		for (auto i = m_actionList.rbegin(); i != m_actionList.rend(); ++i)
		{
			if (action == &(*i))
			{
				for (; i != m_actionList.rend(); ++i)
				{
					if (i->m_action
						&& i->m_action->SoundChannel())
					{
						return i->m_action->SoundChannel();
					}
				}
				return nullptr;
			}
		}
		return nullptr;
	}

	const std::vector<std::string>& GameCore::GetStageUsedSoud(const StageData* stage) const
	{
		return stage->GetUsedSound();
	}

	GameLogic::ActionNodeType ActionNode::GetType() const
	{
		if (m_chooseIndex >= 0)
			return ActionNodeType::Choosed;
		if (m_action == nullptr)
			return ActionNodeType::Choosing;
		if (m_action->Type() == 1)
			return ActionNodeType::Tips;
		return ActionNodeType::NormalText;
	}

};