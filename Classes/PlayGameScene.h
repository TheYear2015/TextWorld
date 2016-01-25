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

	virtual void OnEnterAction(const GameLogic::StageData* stageData, const GameLogic::StageActionData* actData) override;

	void UpdateActionScrollView(bool isChangeSize);

	virtual void OnLeaveStage(const GameLogic::StageData* stageData) override;

	virtual void OnNeedChoose(const GameLogic::StageData* stageData) override;

	virtual void OnGameBegin() override;

	virtual void OnGameFailed(int param) override;

	virtual void OnGameOK() override;

private:
	void LogicUpdate(float dt);
	void ChooseAction(cocos2d::Ref* target, cocos2d::ui::Widget::TouchEventType type);
	void OnActionListScrollViewEvent(cocos2d::Ref* target, cocos2d::ui::ScrollView::EventType type);

private:
	cocos2d::ui::ScrollView* m_actionScrollView = nullptr;
	cocos2d::ui::Layout* m_normalTextNodeTmpl = nullptr;
	cocos2d::ui::Layout* m_chooseNodeTmpl = nullptr;

	cocos2d::Size m_normalTextNodeSize = { 0, 0 };
	cocos2d::Size m_chooseNodeSize = { 0, 0 };

	std::list<cocos2d::ui::Layout*> m_unusedNormalTextNodeList;
	std::list<cocos2d::ui::Layout*> m_unusedChooseNodeList;

	//行为单元
	class ActionCell
	{
	public:
		const GameLogic::StageData* m_stage = nullptr;
		const GameLogic::StageActionData* m_action = nullptr;//行为， null表示是选择
		cocos2d::Node* m_guiNode = nullptr;
		float m_logicY = 0;
	};

	std::vector<ActionCell> m_actionCellArray;

	float GetActionCellHeight(const ActionCell& ac) const;

};

#endif // __PLAY_GAME_SCENE_H__
