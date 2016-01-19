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

	DataManager DataManager::Instance()
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

	}
}