#ifndef __PLAY_GAME_SCENE_H__
#define __PLAY_GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "BaseScene.h"

class PlayGame : public XUtility::BaseSceneImpl<PlayGame>
{
protected:
	virtual const char* GetSceneCsb() const override;

	virtual void OnSceneInited() override;

	virtual void onEnter() override;

private:
	const int NormalTextNodeTage = 11122;

	const int ChooseNodeTage = 13986;

	cocos2d::ui::Layout* CreateNormalTextNode();

	cocos2d::ui::Layout* CreateChooseNode();

	void ReleaseNormalTextNode(cocos2d::ui::Layout* node);

	void ReleaseChooseNode(cocos2d::ui::Layout* node);


private:
	cocos2d::ui::ScrollView* m_actionScrollView = nullptr;
	cocos2d::ui::Layout* m_normalTextNodeTmpl = nullptr;
	cocos2d::ui::Layout* m_chooseNodeTmpl = nullptr;

	std::list<cocos2d::ui::Layout*> m_unusedNormalTextNodeList;
	std::list<cocos2d::ui::Layout*> m_unusedChooseNodeList;
};

#endif // __PLAY_GAME_SCENE_H__
