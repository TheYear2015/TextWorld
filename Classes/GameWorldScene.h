#ifndef __GAME_WORLD_SCENE_H__
#define __GAME_WORLD_SCENE_H__

#include "cocos2d.h"
#include "XUtility/BaseScene.h"

class GameWorld : public XUtility::BaseSceneImpl<GameWorld>
{
protected:
	virtual const char* GetSceneCsb() const override;

	virtual void OnSceneInited() override;

	void NewBtnTouchEvent(cocos2d::Ref* target, cocos2d::ui::Widget::TouchEventType type);

	void ContinueBtnTouchEvent(cocos2d::Ref* target, cocos2d::ui::Widget::TouchEventType type);

};

#endif // __GAME_WORLD_SCENE_H__
