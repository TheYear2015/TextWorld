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
		virtual void OnEnterAction(const StageActionData* actData) = 0;

		//当离开当前场景时
		virtual void OnLeaveStage(const StageData* stageData) = 0;

		//当需要选择时
		virtual void OnNeedChoose(const StageData* stageData) = 0;

		//当游戏开始时
		virtual void OnGameBegin() = 0;

		//当游戏失败时
		virtual void OnGameFailed(int param) = 0;

		//当游戏通关时
		virtual void OnGameOK() = 0;

	};


	//用户游戏数据
	class UserGameData
	{
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
		GameCore();

		~GameCore();

		static GameCore& Instance();

	public:
		//重新开始 
		void New();

		//读取用户数据
		void Load();

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

	private:
		void SaveUserDataToFile();

		GameLogicInterface* m_interface = nullptr;

		uint64_t m_playingTimeMS = 0;

		//用户数据
		UserGameData m_userData;
	};
}

#endif // __GAME_LOGIC_H__
