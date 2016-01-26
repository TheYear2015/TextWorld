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

public:
	virtual void OnEnterStage(const GameLogic::StageData* stageData) override;

	virtual void OnEnterAction(const GameLogic::ActionNode* actNode) override;

	virtual void OnLeaveStage(const GameLogic::StageData* stageData) override;

	virtual void OnNeedChoose(const GameLogic::ActionNode* actNode) override;

	virtual void OnGameBegin() override;

	virtual void OnGameFailed(int param) override;

	virtual void OnGameOK() override;

private:
	void LogicUpdate(float dt);
	void ChooseAction(cocos2d::Ref* target, cocos2d::ui::Widget::TouchEventType type);
	void OnActionListScrollViewEvent(cocos2d::Ref* target, cocos2d::ui::ScrollView::EventType type);

private:
	void UpdateActionScrollView(bool isChangeSize);
	cocos2d::Vec2 m_preScrollViewContainerPos;

private:
	cocos2d::ui::ScrollView* m_actionScrollView = nullptr;

private:
	cocos2d::ui::Layout* CreateActionNode(GameLogic::ActionNodeType type);

	void ReleaseActionNode(cocos2d::ui::Layout* node);

	std::array<int, (int)GameLogic::ActionNodeType::Count> m_nodeTmplTag;
	std::array<std::string, (int)GameLogic::ActionNodeType::Count> m_nodeTmplName;
	std::array<cocos2d::ui::Layout*, (int)GameLogic::ActionNodeType::Count> m_nodeTmpl;
	std::array<cocos2d::Size, (int)GameLogic::ActionNodeType::Count> m_nodeSize;


private:
	std::array<std::list<cocos2d::ui::Layout*>, (int)GameLogic::ActionNodeType::Count> m_unusedNodeList;

private:
	//行为单元
	class ActionCell
	{
	public:
		const GameLogic::ActionNode* m_action = nullptr;//行为
		cocos2d::ui::Layout* m_guiNode = nullptr;
		float m_logicY = 0;

	};

	std::vector<ActionCell> m_actionCellArray;

	float GetActionCellHeight(const ActionCell& ac) const;

};

#endif // __PLAY_GAME_SCENE_H__
