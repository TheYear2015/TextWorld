#ifndef __GAME_DATA_H__
#define __GAME_DATA_H__

#include <list>
#include <vector>
#include <array>
#include <map>
#include <stdint.h>

namespace GameLogic
{

	//��Ϸ���ݳ����е���Ϊ������
	class StageActionData
	{
		friend class DataManager;

	public:
		uint32_t Type() const { return m_type; }
		const std::string& Text() const { return m_text; }
		uint32_t DuringMS() const { return m_duringMS; }

	private:
		//����
		uint32_t m_type = 0;
		//��������
		std::string m_text;
		//����ʱ��
		uint32_t m_duringMS = 0;
	};

	//��Ϸ���ݳ���������
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
		//�ڽ�����ʱ����ѡ��
		bool IsHaveChooseAtEnd() const;
		//��������
		const std::string& Music() const { return m_bkMusic; }
	private:
		//Id
		uint32_t m_id = 0;
		//�Զ�������һ��������ʹ�� m_goToStage[0]
		bool m_isAutoNext = false;
		std::string m_bkMusic;
		//������������Ϣ
		std::vector<std::pair<uint32_t, std::string>> m_goToStage;
		//������Ϊ�б�
		std::vector<StageActionData> m_actionList;
	};

	//��Ϸ���ݹ�����
	class DataManager
	{
	public:
		DataManager();

		~DataManager();

	public:
		static DataManager& Instance();

	public:
		//���ļ���ȡ����
		void ReadGameDataFromFile(const char* fileName);

		const StageData* GetStageData(uint32_t id) const;

	private:
		std::map<uint32_t, StageData*> m_stageDataSet;

	};

}

#endif // __GAME_LOGIC_H__
