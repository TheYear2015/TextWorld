#ifndef __ACTION_NODE_GRAPHIC_H__
#define __ACTION_NODE_GRAPHIC_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "XUtility/BaseScene.h"
#include "GameLogic.h"

class ActionNodeGraphic
{
public:
	virtual cocos2d::Node* GetNode() = 0;

	virtual cocos2d::Size GetSize() const = 0;

	virtual int GetType() const = 0;

	virtual bool Init() = 0;

	virtual void SetData(const GameLogic::ActionNode* action) = 0;

	virtual void ShowLoading(bool show) = 0;
};

class ActionNodeGraphicMgr
{
public:
	static ActionNodeGraphicMgr& Instance();

	ActionNodeGraphicMgr();

	ActionNodeGraphic* CreateActionNode(GameLogic::ActionNodeType type, cocos2d::Node* parent);

	ActionNodeGraphic* CreateActionNodeByData(const GameLogic::ActionNode* action, cocos2d::Node* parent);

	void ReleaseActionNode(ActionNodeGraphic* node);

private:
	ActionNodeGraphic* NewCreateActionNode(GameLogic::ActionNodeType type);

	std::array<int, (int)GameLogic::ActionNodeType::Count> m_nodeTmplTag;
	std::array<std::list<ActionNodeGraphic*>, (int)GameLogic::ActionNodeType::Count> m_unusedNodeList;
};

class NormalActionNode :public ActionNodeGraphic
{

public:
	virtual cocos2d::Node* GetNode() override;

	virtual cocos2d::Size GetSize() const override;

	virtual int GetType() const override;

	virtual bool Init() override;

	virtual void SetData(const GameLogic::ActionNode* action) override;

	virtual void ShowLoading(bool show) override;


private:
	cocos2d::Node* m_root = nullptr;
	cocos2d::Node* m_node = nullptr;
	cocos2d::Node* m_content = nullptr;
	cocos2d::Node* m_loading = nullptr;

};

class TipsActionNode :public ActionNodeGraphic
{

public:
	virtual cocos2d::Node* GetNode() override;

	virtual cocos2d::Size GetSize() const override;

	virtual int GetType() const override;

	virtual bool Init() override;

	virtual void SetData(const GameLogic::ActionNode* action) override;

	virtual void ShowLoading(bool show) override;


private:
	cocos2d::Node* m_root = nullptr;
	cocos2d::Node* m_node = nullptr;
	cocos2d::Node* m_content = nullptr;
	cocos2d::Node* m_loading = nullptr;

};


class ChoosingActionNode :public ActionNodeGraphic
{
public:

	virtual cocos2d::Node* GetNode() override;

	virtual cocos2d::Size GetSize() const override;

	virtual int GetType() const override;

	virtual bool Init() override;

	virtual void SetData(const GameLogic::ActionNode* action) override;

	virtual void ShowLoading(bool show) override;

	void ChooseAction(cocos2d::Ref* target, cocos2d::ui::Widget::TouchEventType type);

private:

	cocos2d::Node* m_root = nullptr;
	cocos2d::Node* m_node = nullptr;

};

class ChoosedActionNode :public ActionNodeGraphic
{
public:

	virtual cocos2d::Node* GetNode() override;

	virtual cocos2d::Size GetSize() const override;

	virtual int GetType() const override;

	virtual bool Init() override;

	virtual void SetData(const GameLogic::ActionNode* action) override;

	virtual void ShowLoading(bool show) override;

	void ChooseAction(cocos2d::Ref* target, cocos2d::ui::Widget::TouchEventType type);

private:
	cocos2d::Node* m_root = nullptr;
	cocos2d::Node* m_node = nullptr;
	const GameLogic::ActionNode* m_action = nullptr;

};

#endif // __ACTION_NODE_GRAPHIC_H__
