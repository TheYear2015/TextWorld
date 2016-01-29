#ifndef __GAME_SCENE_MANAGER_H__
#define __GAME_SCENE_MANAGER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

	enum class ScenenType
	{
		//启动
		Launch,
		//主菜单
		MainMenu,
		//游戏中
		Playing,
	};

	class GameSceneManager
	{
	public:
		static GameSceneManager& Instance();

		cocos2d::Scene* CreateScene(ScenenType type);

		void EnterScene(ScenenType type);
	};



#endif // __GAME_SCENE_MANAGER_H__
