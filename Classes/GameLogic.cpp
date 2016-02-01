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

		//���������Ϸ����ʼ��һ������(id 1)
		if (m_userData.m_playedStages.empty())
		{
			m_playedActionIndex = -1;
			EnterStage(1);
		}
		else//������ǰ������
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
				//�����ȴ����ѡ��
			}
			else
			{
				m_currentStageTime += (dt * 1000);
				//�ж��Ƿ�Ҫ������һ��aciton
				if (m_currentStageTime >= currentAc.m_endTime)
				{
					//������һ��action
					++m_playedActionIndex;
					if (m_playedActionIndex >= m_actionList.size())
					{//��Ϸ��������
						//TODO::��Ϸ��������
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
							//�ж��Ƿ����Զ�ѡ��
							if (m_actionList.size() < MaxActionCount() && newAc.m_stage->AutoNext())
							{//�����Զ�ѡ��
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
		{//��Ϸ��������
			//TODO::��Ϸ��������
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

	//����ѡ��
	void GameCore::ChooseAction(int index)
	{
		if (IsPlaying())
		{
			//������һ������
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