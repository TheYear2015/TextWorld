#ifndef __GAME_LOGIC_H__
#define __GAME_LOGIC_H__

#include "GameData.h"

namespace GameLogic
{
	class GameLogicInterface
	{
	public:
		//������һ���³���ʱ����
		virtual void OnEnterStage(const StageData* stageData) = 0;

		//������һ���µ���Ϊʱ
		virtual void OnEnterAction(const GameLogic::StageData* stageData, const StageActionData* actData) = 0;

		//���뿪��ǰ����ʱ
		virtual void OnLeaveStage(const StageData* stageData) = 0;

		//����Ҫѡ��ʱ
		virtual void OnNeedChoose(const StageData* stageData) = 0;

		//����Ϸ��ʼʱ���û������Ѿ�������ɣ���Ϸ�߼�����
		virtual void OnGameBegin() = 0;

		//����Ϸʧ��ʱ
		virtual void OnGameFailed(int param) = 0;

		//����Ϸͨ��ʱ
		virtual void OnGameOK() = 0;

	};


	//�û���Ϸ����
	class UserGameData
	{
		friend class GameCore;
	public:
		//���볡��
		void EnterStage(uint32_t id);

		const std::vector<uint32_t>& GetPlayedStages() const;

		uint32_t CurrentStageTime() const { return m_currentStageTime; }

		void CurrentStageTime(uint32_t val) { m_currentStageTime = val; }

		void Reset();

	private:
		//�Ѿ����˵ĳ��������һ���ǵ�ǰ������ĳ���
		std::vector<uint32_t> m_playedStages;

		//��ǰ����������ʱ��
		uint32_t m_currentStageTime = 0;

	};


	//��Ϸ�߼�����
	class GameCore
	{
	public:
		enum class GameState
		{
			Ready = 0,//׼����ʼ
			Playing,//������
			Paused,//��ͣ��
			End,//�ѽ���
		};

	public:
		GameCore();

		~GameCore();

		static GameCore& Instance();

	public:
		//���¿�ʼ 
		void New();

		//��ȡ�û�����
		void Load();

		//�����û�����
		void Save();

		//�Ƿ����û�����
		bool IsHaveUserData() const;

		//����
		void Begin();

		//����
		void Update(float dt);

		//����
		void Resume();

		//��ͣ
		void Pause();

		//�Ƿ���������
		bool IsPlaying() const;

		//����ѡ��
		void ChooseAction(int index);

		//���ý����ӿ�
		void SetInterface(GameLogicInterface* inf);

		//����Ѿ���Ϸ���ĳ���(stage)
		std::vector<const StageData*> GetPlayedStageList() const;

		//���������Ϸ�еĳ����Ѿ�������Ϊ(action)
		std::vector<const StageActionData*> GetPlayingActionList() const;

		//�����Ϸ״̬
		GameLogic::GameCore::GameState State() const { return m_state; }

	private:
		void SaveUserDataToFile();

		void EnterStage(int id);

		GameLogicInterface* m_interface = nullptr;

		uint64_t m_playingTimeMS = 0;

		//�û�����
		UserGameData m_userData;

		//��Ϸ������
		class ActionNode
		{
		public:
			bool m_isChoose = false;
			const StageData* m_stage = nullptr;
			const StageActionData* m_action = nullptr;
			//������ʱ��
			uint32_t m_endTime = 0;
		};
		std::vector<ActionNode> m_actionList;

		//���һ��ִ���˵�action������
		int m_playedActionIndex = 0;

		//��Ϸ���е�ʱ��
		uint32_t m_currentStageTime;

		//��Ϸ״̬
		GameState m_state = GameState::Ready;

		uint32_t MaxActionCount() const { return 2000; };
	};
}

#endif // __GAME_LOGIC_H__
