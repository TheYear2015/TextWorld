#ifndef __GAME_LOGIC_H__
#define __GAME_LOGIC_H__

#include "GameData.h"

namespace GameLogic
{
	class GameLogicInterface
	{
	public:
		//当进入一个新场景时触发
		virtual void OnEnterStage(const StageData* stageData) = 0;

		//当触发一个新的行为时
		virtual void OnEnterAction(const GameLogic::StageData* stageData, const StageActionData* actData) = 0;

		//当离开当前场景时
		virtual void OnLeaveStage(const StageData* stageData) = 0;

		//当需要选择时
		virtual void OnNeedChoose(const StageData* stageData) = 0;

		//当游戏开始时，用户数据已经加载完成，游戏逻辑启动
		virtual void OnGameBegin() = 0;

		//当游戏失败时
		virtual void OnGameFailed(int param) = 0;

		//当游戏通关时
		virtual void OnGameOK() = 0;

	};


	//用户游戏数据
	class UserGameData
	{
		friend class GameCore;
	public:
		//进入场景
		void EnterStage(uint32_t id);

		const std::vector<uint32_t>& GetPlayedStages() const;

		uint32_t CurrentStageTime() const { return m_currentStageTime; }

		void CurrentStageTime(uint32_t val) { m_currentStageTime = val; }

		void Reset();

	private:
		//已经玩了的场景，最后一个是当前正在玩的场景
		std::vector<uint32_t> m_playedStages;

		//当前场景经过的时间
		uint32_t m_currentStageTime = 0;

	};


	//游戏逻辑核心
	class GameCore
	{
	public:
		enum class GameState
		{
			Ready = 0,//准备开始
			Playing,//运行中
			Paused,//暂停中
			End,//已结束
		};

	public:
		GameCore();

		~GameCore();

		static GameCore& Instance();

	public:
		//重新开始 
		void New();

		//读取用户数据
		void Load();

		//保存用户数据
		void Save();

		//是否有用户数据
		bool IsHaveUserData() const;

		//启动
		void Begin();

		//更新
		void Update(float dt);

		//继续
		void Resume();

		//暂停
		void Pause();

		//是否在运行中
		bool IsPlaying() const;

		//进行选择
		void ChooseAction(int index);

		//设置交互接口
		void SetInterface(GameLogicInterface* inf);

		//获得已经游戏过的场景(stage)
		std::vector<const StageData*> GetPlayedStageList() const;

		//获得正在游戏中的场景已经过的行为(action)
		std::vector<const StageActionData*> GetPlayingActionList() const;

		//获得游戏状态
		GameLogic::GameCore::GameState State() const { return m_state; }

	private:
		void SaveUserDataToFile();

		void EnterStage(int id);

		GameLogicInterface* m_interface = nullptr;

		uint64_t m_playingTimeMS = 0;

		//用户数据
		UserGameData m_userData;

		//游戏中数据
		class ActionNode
		{
		public:
			bool m_isChoose = false;
			const StageData* m_stage = nullptr;
			const StageActionData* m_action = nullptr;
			//结束的时间
			uint32_t m_endTime = 0;
		};
		std::vector<ActionNode> m_actionList;

		//最后一个执行了的action的索引
		int m_playedActionIndex = 0;

		//游戏进行的时间
		uint32_t m_currentStageTime;

		//游戏状态
		GameState m_state = GameState::Ready;

		uint32_t MaxActionCount() const { return 2000; };
	};
}

#endif // __GAME_LOGIC_H__
