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

	public:
		uint32_t Type() const { return m_type; }
		const std::string& Text() const { return m_text; }
		uint32_t DuringMS() const { return m_duringMS; }

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

		uint32_t Id() const { return m_id; }
		bool AutoNext() const { return m_isAutoNext; }
		const std::vector<std::pair<uint32_t, std::string>>& ToStage() const { return m_goToStage; }
		const std::vector<StageActionData>& ActionList() const { return m_actionList; }
		//在结束的时候有选择
		bool IsHaveChooseAtEnd() const;
		//背景音乐
		const std::string& Music() const { return m_bkMusic; }
	private:
		//Id
		uint32_t m_id = 0;
		//自动进入下一个场景，使用 m_goToStage[0]
		bool m_isAutoNext = false;
		std::string m_bkMusic;
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
		static DataManager& Instance();

	public:
		//从文件读取数据
		void ReadGameDataFromFile(const char* fileName);

		const StageData* GetStageData(uint32_t id) const;

	private:
		std::map<uint32_t, StageData*> m_stageDataSet;

	};

}

#endif // __GAME_LOGIC_H__
