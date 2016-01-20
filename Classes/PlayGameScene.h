#ifndef __PLAY_GAME_SCENE_H__
#define __PLAY_GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "BaseScene.h"
#include "GameLogic.h"

class PlayGame : public XUtility::BaseSceneImpl<PlayGame>, public GameLogic::GameLogicInterface
{
protected:
	virtual const char* GetSceneCsb() const override;

	virtual void OnSceneInited() override;

	virtual void onEnter() override;

	virtual void onExit() override;

	virtual void onEnterTransitionDidFinish() override;

private:
	const int NormalTextNodeTage = 11122;

	const int ChooseNodeTage = 13986;

	cocos2d::ui::Layout* CreateNormalTextNode();

	cocos2d::ui::Layout* CreateChooseNode();

	void ReleaseNormalTextNode(cocos2d::ui::Layout* node);

	void ReleaseChooseNode(cocos2d::ui::Layout* node);

public:
	virtual void OnEnterStage(const GameLogic::StageData* stageData) override;

	virtual void OnEnterAction(const GameLogic::StageActionData* actData) override;

	virtual void OnLeaveStage(const GameLogic::StageData* stageData) override;

	virtual void OnNeedChoose(const GameLogic::StageData* stageData) override;

	virtual void OnGameBegin() override;

	virtual void OnGameFailed(int param) override;

	virtual void OnGameOK() override;

private:
	cocos2d::ui::ScrollView* m_actionScrollView = nullptr;
	cocos2d::ui::Layout* m_normalTextNodeTmpl = nullptr;
	cocos2d::ui::Layout* m_chooseNodeTmpl = nullptr;

	cocos2d::Size m_normalTextNodeSize = { 0, 0 };
	cocos2d::Size m_chooseNodeSize = { 0, 0 };

	std::list<cocos2d::ui::Layout*> m_unusedNormalTextNodeList;
	std::list<cocos2d::ui::Layout*> m_unusedChooseNodeList;
};

#endif // __PLAY_GAME_SCENE_H__
