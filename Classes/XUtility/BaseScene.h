#ifndef __XUTILITY_BASE_SCENE_H__
#define __XUTILITY_BASE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

namespace XUtility
{
	class BaseScene : public cocos2d::Layer
	{

	public:
		cocos2d::Node* SceneRoot() const { return m_sceneRoot; }

	protected:
		cocos2d::Node* m_sceneRoot = nullptr;
	};

	template<class T>
	class BaseSceneImpl : public BaseScene
	{
	public:
		// there's no 'id' in cpp, so we recommend returning the class instance pointer
		static cocos2d::Scene* createScene()
		{
			// 'scene' is an autorelease object
			auto scene = cocos2d::Scene::create();

			// 'layer' is an autorelease object
			auto layer = T::create();

			// add layer as a child to scene
			scene->addChild(layer);

			// return the scene
			return scene;
		}

		// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
		virtual bool init() override
		{
			//////////////////////////////
			// 1. super init first
			if (!cocos2d::Layer::init())
			{
				return false;
			}

			m_sceneRoot = cocos2d::CSLoader::createNode(GetSceneCsb());
			addChild(m_sceneRoot);

			OnSceneInited();

			return true;
		}

		// implement the "static create()" method manually
		CREATE_FUNC(T);

	protected:
		virtual const char* GetSceneCsb() const = 0;

		virtual void OnSceneInited() {};
	};
}



#endif // __XUTILITY_BASE_SCENE_H__
