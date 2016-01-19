#ifndef __GAME_DATA_H__
#define __GAME_DATA_H__

#include <list>
#include <vector>
#include <array>
#include <map>
#include <stdint.h>

namespace GameLogic
{

	//游戏内容场景中的行为的数据
	class StageActionData
	{
		friend class DataManager;
	private:
		//类型
		uint32_t m_type = 0;

		//文字内容
		std::string m_text;

		//持续时间
		uint32_t m_duringMS = 0;
	};

	//游戏内容场景的数据
	class StageData
	{
		friend class DataManager;
	public:
		StageData();
		~StageData();

	private:
		//Id
		uint32_t m_id = 0;

		//场景结束跳信息
		std::vector<std::pair<uint32_t, std::string>> m_goToStage;

		//场景行为列表
		std::vector<StageActionData> m_actionList;
	};

	//游戏数据管理器
	class DataManager
	{
	public:
		DataManager();

		~DataManager();

	public:
		static DataManager Instance();

	public:
		//从文件读取数据
		void ReadGameDataFromFile(const char* fileName);

		const StageData* GetStageData(uint32_t id) const;

	private:
		std::map<uint32_t, StageData*> m_stageDataSet;

	};

}

#endif // __GAME_LOGIC_H__
