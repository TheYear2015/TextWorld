#include "GameData.h"
#include "platform/CCFileUtils.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

namespace GameLogic
{

	DataManager::DataManager()
	{

	}

	DataManager::~DataManager()
	{
		for (auto& c : m_stageDataSet)
		{
			delete c.second;
		}
		m_stageDataSet.clear();
	}

	DataManager& DataManager::Instance()
	{
		static DataManager _ins;
		return _ins;
	}

	void DataManager::ReadGameDataFromFile(const char* fileName)
	{
		auto path = cocos2d::FileUtils::getInstance()->fullPathForFilename(fileName);
		auto data = cocos2d::FileUtils::getInstance()->getStringFromFile(path);

		rapidjson::Document doc;
		doc.Parse<0>(data.c_str());

		if (doc.HasParseError())
		{
			CCLOG("DataManager::ReadGameDataFromFile parse json error!");
			return;
		}

		for (auto& c : m_stageDataSet)
		{
			delete c.second;
		}
		m_stageDataSet.clear();

		if (doc.HasMember("Stages"))
		{
			const rapidjson::Value& stageListValue = doc["Stages"];
			if (stageListValue.IsArray() && stageListValue.Size() > 0)
			{
				int count = stageListValue.Size();
				for (int i = 0; i < count; ++i)
				{
					auto &stageValue = stageListValue[i];
					if (stageValue.IsObject())
					{
						StageData* s = new StageData();
						s->m_id = stageValue["Id"].GetInt();
						m_stageDataSet[s->m_id] = s;

						if (stageValue.HasMember("AutoGoto"))
						{
							const auto& v = stageValue["AutoGoto"];
							s->m_isAutoNext = v.GetBool();
						}

						if (stageValue.HasMember("BackMusic"))
						{
							const auto& v = stageValue["BackMusic"];
							s->m_bkMusic = v.GetString();
						}

						if (stageValue.HasMember("ContentList"))
						{
							const auto& listV = stageValue["ContentList"];
							if (listV.IsArray() && listV.Size() > 0)
							{
								int aC = listV.Size();
								s->m_actionList.resize(aC);
								for (int __i = 0; __i < aC; ++__i)
								{
									auto& aV = listV[__i];
									auto& action = s->m_actionList.at(__i);
									action.m_type = aV["Type"].GetInt();
									action.m_duringMS = aV["DuringMS"].GetInt();
									action.m_text = aV["Text"].GetString();

									if (stageValue.HasMember("SoundChannel")
										&& stageValue.HasMember("StopChannel")
										&& stageValue.HasMember("SoundIsLoop"))
									{
										bool isHaveSoundChannel = false;
										const auto& soundChannel = stageValue["SoundChannel"];
										const auto& channelStop = stageValue["StopChannel"];
										const auto& channelIsLoop = stageValue["SoundIsLoop"];

										std::string tmp;
										for (int nn = 0; nn < soundChannel.Size(); ++nn)
										{
											tmp = soundChannel[nn].GetString();
											if (!tmp.empty())
											{
												isHaveSoundChannel = true;
												break;
											}
										}
										if (!isHaveSoundChannel)
										{
											for (int nn = 0; nn < channelStop.Size(); ++nn)
											{
												if(channelStop[nn].GetBool())
												{
													isHaveSoundChannel = true;
													break;
												}
											}
										}

										if (isHaveSoundChannel)
										{
											action.m_soundDef = new	StageActionData::SoundChannelDef();
											for (int nn = 0; nn < action.m_soundDef->m_soundChannel.size(); ++nn)
											{
												action.m_soundDef->m_soundChannel[nn] = soundChannel[nn].GetString();
												action.m_soundDef->m_isChannelLoop[nn] = channelIsLoop[nn].GetBool();
												action.m_soundDef->m_isChannelStop[nn] = channelStop[nn].GetBool();
											}
										}
									}
								}
							}
						}

						if (stageValue.HasMember("GoId"))
						{
							const auto& listV = stageValue["GoId"];
							if (listV.IsArray() && listV.Size() > 0)
							{
								int gC = listV.Size();
								s->m_goToStage.resize(gC);
								for (int __i = 0; __i < gC; ++__i)
								{
									auto& gS = s->m_goToStage.at(__i);
									gS.first = listV[__i].GetInt();
								}
							}
						}
						if (stageValue.HasMember("GoString"))
						{
							const auto& listV = stageValue["GoString"];
							if (listV.IsArray() && listV.Size() > 0)
							{
								int gC = listV.Size();
								for (int __i = 0; __i < gC && __i < s->m_goToStage.size(); ++__i)
								{
									auto& gS = s->m_goToStage.at(__i);
									gS.second = listV[__i].GetString();
								}
							}
						}
					}
				}
			}
		}
	}

	const StageData* DataManager::GetStageData(uint32_t id) const
	{
		auto c = m_stageDataSet.find(id);
		return c != m_stageDataSet.end() ? c->second : nullptr;
	}


	StageData::StageData()
	{

	}

	StageData::~StageData()
	{
		if (m_usedSound)
			delete m_usedSound;
		m_usedSound = nullptr;
	}

	bool StageData::IsHaveChooseAtEnd() const
	{
		return m_goToStage[0].first != 0 || m_goToStage[1].first != 0;
	}

	const std::vector<std::string>& StageData::GetUsedSound() const
	{
		if (!m_usedSound)
		{
			m_usedSound = new std::vector<std::string>();
			std::string tmp;
			for (auto i = m_actionList.begin(); i != m_actionList.end(); ++i)
			{
				if (i->SoundChannel())
				{
					for (int jj = 0; jj < (int)GameLogic::StageActionData::SoundChannelDef::Define::ChannelCount; ++jj)
					{
						tmp = i->SoundChannel()->SoundName(jj);
						if (!tmp.empty())
						{
							m_usedSound->push_back(tmp);
						}
					}
				}
			}
		}
		return *m_usedSound;
	}

}