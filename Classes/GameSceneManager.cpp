#include "GameSceneManager.h"
#include "GameLogic.h"
#include "GameLaunchScene.h"
#include "GameWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;
namespace XUtility
{
	cocos2d::Scene* GameSceneManager::CreateScene(ScenenType type)
	{
		cocos2d::Scene* scene = nullptr;
		switch (type)
		{
		case ScenenType::Launch:
			scene = GameLaunch::createScene();
			break;
		case ScenenType::MainMenu:
			scene = GameWorld::createScene();
			break;
		}
		if (scene != nullptr)
		{
			scene->setTag((int)type);
		}
		return scene;
	}

	GameSceneManager& GameSceneManager::Instance()
	{
		static GameSceneManager _inst;
		return _inst;
	}

	void GameSceneManager::EnterScene(ScenenType type)
	{
		auto scene = CreateScene(type);
		if (scene != nullptr)
		{
			auto fade = TransitionFade::create(0.5f, scene, cocos2d::Color3B::BLACK);
			Director::getInstance()->replaceScene(fade);
		}
	}

}
