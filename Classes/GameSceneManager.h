#ifndef __GAME_SCENE_MANAGER_H__
#define __GAME_SCENE_MANAGER_H__

#include "BaseScene.h"
namespace XUtility
{
	enum class ScenenType
	{
		//����
		Launch,
		//���˵�
		MainMenu,
		//��Ϸ��
		Playing,
	};

	class GameSceneManager
	{
	public:
		static GameSceneManager& Instance();

		cocos2d::Scene* CreateScene(ScenenType type);

		void EnterScene(ScenenType type);
	};
}


#endif // __GAME_SCENE_MANAGER_H__
