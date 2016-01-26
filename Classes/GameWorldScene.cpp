#include "GameWorldScene.h"
#include "GameSceneManager.h"
#include "GameLogic.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

const char* GameWorld::GetSceneCsb() const
{
	return "MainScene.csb";
}

void GameWorld::OnSceneInited()
{
	auto newBtn = dynamic_cast<cocos2d::ui::Button*>(SceneRoot()->getChildByName("NewBtn"));
	if (newBtn)
	{
		newBtn->addTouchEventListener(CC_CALLBACK_2(GameWorld::NewBtnTouchEvent, this));
	}
	auto continueBtn = dynamic_cast<cocos2d::ui::Button*>(SceneRoot()->getChildByName("ContinueBtn"));
	if (continueBtn)
	{
		continueBtn->addTouchEventListener(CC_CALLBACK_2(GameWorld::ContinueBtnTouchEvent, this));
	}

}

void GameWorld::NewBtnTouchEvent(cocos2d::Ref* target, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;

	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;

	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		XUtility::GameSceneManager::Instance().EnterScene(XUtility::ScenenType::Playing);
	}
	break;

	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void GameWorld::ContinueBtnTouchEvent(cocos2d::Ref* target, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;

	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;

	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		XUtility::GameSceneManager::Instance().EnterScene(XUtility::ScenenType::Playing);
	}
	break;

	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
