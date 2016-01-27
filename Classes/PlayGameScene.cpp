#include <algorithm>
#include "PlayGameScene.h"
#include "GameLogic.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

const char* PlayGame::GetSceneCsb() const
{
	return "GameScene.csb";
}

void PlayGame::OnSceneInited()
{
	m_nodeTmplName[(int)GameLogic::ActionNodeType::NormalText] = "NormalTextNode";
	m_nodeTmplName[(int)GameLogic::ActionNodeType::Choosing] = "ChooseNode";
	m_nodeTmplName[(int)GameLogic::ActionNodeType::Choosed] = "ChoosedNode";

	m_actionCellArray.reserve(10000);

	auto l = SceneRoot()->getChildByName("ActionListLayer");
	if (l)
		m_actionScrollView = dynamic_cast<cocos2d::ui::ScrollView*>(l->getChildByName("ActionScrollView"));

	if (m_actionScrollView)
	{
		m_loadingAnimation = m_actionScrollView->getChildByName("NomralTextLoading");
		if (m_loadingAnimation)
		{
			m_loadingAnimation->setVisible(false);
			m_loadingAnimation->setLocalZOrder(20);
			auto t = m_loadingAnimation->getTag();
			auto action = dynamic_cast<cocostudio::timeline::ActionTimeline*>(m_loadingAnimation->getActionByTag(t));
			if (action)
				action->play("Bring", true);
		}

		for (int i = 0; i < m_nodeTmplName.size(); ++i)
		{
			m_nodeTmplTag[i] = TagBase + i;
			m_nodeTmpl[i] = dynamic_cast<cocos2d::ui::Layout*>(m_actionScrollView->getChildByName(m_nodeTmplName[i]));
			if (m_nodeTmpl[i])
			{
				m_nodeTmpl[i]->setLocalZOrder(10);
				m_nodeTmpl[i]->setVisible(false);
				m_nodeSize[i] = m_nodeTmpl[i]->getContentSize();
			}
		}

		m_actionScrollView->addEventListener(CC_CALLBACK_2(PlayGame::OnActionListScrollViewEvent, this));
	}

}

void PlayGame::onEnter()
{
	BaseScene::onEnter();
	GameLogic::GameCore::Instance().SetInterface(this);

	////加载用户数据，创建actionList
	//TODO:读取用户保存数据
	this->schedule(schedule_selector(PlayGame::LogicUpdate), 0.05f);
}


void PlayGame::onExit()
{
	GameLogic::GameCore::Instance().SetInterface(nullptr);
	BaseScene::onExit();
}

void PlayGame::onEnterTransitionDidFinish()
{
	BaseScene::onEnterTransitionDidFinish();
	//游戏启动
	GameLogic::GameCore::Instance().Begin();
}


void PlayGame::OnEnterStage(const GameLogic::StageData* stageData)
{
	if (stageData)
		CCLOG("PlayGame::OnEnterStage %d.", stageData->Id());
	else
		CCLOGERROR("PlayGame::OnEnterStage null.");
}

void PlayGame::OnEnterAction(const GameLogic::ActionNode* actNode)
{
	if (actNode)
	{
		ActionCell actionCell;
		actionCell.m_action = actNode;

		if (!m_actionCellArray.empty())
		{
			auto& last = m_actionCellArray.back();
			actionCell.m_logicY = last.m_logicY + GetActionCellHeight(last);
		}

		m_actionCellArray.push_back(actionCell);

		if (actNode->m_action)
			CCLOG("PlayGame::OnEnterAction %s.(%d)", actNode->m_action->Text().c_str(), m_actionCellArray.size());

		if (actNode->GetType() == GameLogic::ActionNodeType::NormalText)
		{
			PlayBringAnimation(m_actionCellArray.size() - 1);
		}

		UpdateActionScrollView(true);
		m_actionScrollView->scrollToBottom(0.5f, true);

	}
	else
		CCLOGERROR("PlayGame::OnEnterAction null.");
}

void PlayGame::OnLeaveStage(const GameLogic::StageData* stageData)
{
	if (stageData)
		CCLOG("PlayGame::OnLeaveStage %d.", stageData->Id());
	else
		CCLOGERROR("PlayGame::OnLeaveStage null.");
}

void PlayGame::OnNeedChoose(const GameLogic::ActionNode* actNode)
{
	if (actNode && actNode->m_stage)
	{
		CCLOG("PlayGame::OnNeedChoose %d.", actNode->m_stage->Id());
		ActionCell actionCell;
		actionCell.m_action = actNode;
		if (!m_actionCellArray.empty())
		{
			auto& last = m_actionCellArray.back();
			actionCell.m_logicY = last.m_logicY + GetActionCellHeight(last);
		}

		m_actionCellArray.push_back(actionCell);
		UpdateActionScrollView(true);
		m_actionScrollView->scrollToBottom(0.5f, true);

	}
	else
		CCLOGERROR("PlayGame::OnNeedChoose null.");
}

void PlayGame::OnGameBegin()
{
	CCLOG("PlayGame::OnGameBegin");
}

void PlayGame::OnGameFailed(int param)
{
	CCLOG("PlayGame::OnGameFailed %d.", param);
}

void PlayGame::OnGameOK()
{
	CCLOG("PlayGame::OnGameOK");
	UpdateActionScrollView(true);
}

void PlayGame::LogicUpdate(float dt)
{
	GameLogic::GameCore::Instance().Update(dt);
}

void PlayGame::UpdateActionScrollView(bool isChangeSize)
{
	auto scrollVPos = m_actionScrollView->getInnerContainerPosition();
	if (!isChangeSize)
	{
		if (fabs(scrollVPos.y - m_preScrollViewContainerPos.y) < 100)
		{
			return;
		}
	}
	m_preScrollViewContainerPos = scrollVPos;
	//更新坐标

	//获得新的内容尺寸
	float height = 0;
	auto size = m_actionScrollView->getContentSize();
	if (isChangeSize)
	{
		if (!m_actionCellArray.empty())
		{
			auto& last = m_actionCellArray.back();
			height = last.m_logicY + GetActionCellHeight(last);
		}
		height = std::max(size.height, height);
	}
	else
	{
		height = m_actionScrollView->getInnerContainerSize().height;
	}

	//更新控件并更新位置
	cocos2d::Vec2 pos = { 0, 0 };
	scrollVPos.x = -scrollVPos.y - size.height * 2;
	scrollVPos.y = -scrollVPos.y + size.height * 3;
	int ii = 0;
	for (auto b = m_actionCellArray.begin(); b != m_actionCellArray.end(); ++b)
	{
		auto& ac = *b;
		pos.y = height - ac.m_logicY;
		//判断是否在区域内
		if (pos.y >= scrollVPos.x && pos.y < scrollVPos.y)
		{
			if (ac.m_guiNode)
			{
				if (ac.m_guiNode->getTag() != TagBase + (int)ac.m_action->GetType())
				{
					ReleaseActionNode(ac.m_guiNode);
					ac.m_guiNode = nullptr;
				}
			}
			if (!ac.m_guiNode)
			{
				//创建新的控件
				ac.m_guiNode = CreateActionNodeByData(ac.m_action);
			}
			ac.m_guiNode->setVisible(true);
			ac.m_guiNode->setPosition(pos);

			if (ii == m_newIndex && m_loadingAnimation)
			{
				ac.m_guiNode->setVisible(false);
				auto s = GetActionCellSize(ac);
				pos.x += s.width * 0.5f;
				pos.y -= s.height * 0.5f;
				m_loadingAnimation->setVisible(true);
				m_loadingAnimation->setPosition(pos);
			}
			else
			{
				ac.m_guiNode->setVisible(true);
				ac.m_guiNode->setPosition(pos);
			}
		}
		else
		{
			if (ac.m_guiNode)
			{
				ReleaseActionNode(ac.m_guiNode);
				ac.m_guiNode = nullptr;
			}
		}
		++ii;
	}

	//需要播放出现动画的播放出现动画

	if (isChangeSize)
	{
		//刷新显示区域
		size = m_actionScrollView->getInnerContainerSize();
		size.height = height;
		m_actionScrollView->setInnerContainerSize(size);
	}
}

void PlayGame::ChooseAction(cocos2d::Ref* target, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;

	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;

	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		auto btn = dynamic_cast<cocos2d::ui::Button*>(target);
		if (btn)
		{
			GameLogic::GameCore::Instance().ChooseAction(btn->getTag());
		}
	}
	break;

	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

float PlayGame::GetActionCellHeight(const ActionCell& ac) const
{
	return m_nodeSize[(int)ac.m_action->GetType()].height;
}


cocos2d::Size PlayGame::GetActionCellSize(const ActionCell& ac) const
{
	return m_nodeSize[(int)ac.m_action->GetType()];
}


void PlayGame::OnActionListScrollViewEvent(cocos2d::Ref* target, cocos2d::ui::ScrollView::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::ScrollView::EventType::SCROLL_TO_TOP:
		break;
	case cocos2d::ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
		break;
	case cocos2d::ui::ScrollView::EventType::SCROLL_TO_LEFT:
		break;
	case cocos2d::ui::ScrollView::EventType::SCROLL_TO_RIGHT:
		break;
	case cocos2d::ui::ScrollView::EventType::SCROLLING:
		break;
	case cocos2d::ui::ScrollView::EventType::BOUNCE_TOP:
		break;
	case cocos2d::ui::ScrollView::EventType::BOUNCE_BOTTOM:
		break;
	case cocos2d::ui::ScrollView::EventType::BOUNCE_LEFT:
		break;
	case cocos2d::ui::ScrollView::EventType::BOUNCE_RIGHT:
		break;
	case cocos2d::ui::ScrollView::EventType::CONTAINER_MOVED:
	{//滑动结束
		UpdateActionScrollView(false);
	}
	break;
	default:
		break;
	}
}

cocos2d::ui::Layout* PlayGame::CreateActionNode(GameLogic::ActionNodeType type)
{
	int t = (int)type;
	if (m_unusedNodeList[t].empty())
	{
		auto n = m_nodeTmpl[t]->clone();
		n->setTag(m_nodeTmplTag[t]);
		m_actionScrollView->addChild(n);
		n->setVisible(true);
		return dynamic_cast<cocos2d::ui::Layout*>(n);
	}
	auto n = m_unusedNodeList[t].front();
	m_unusedNodeList[t].pop_front();
	return n;
}

cocos2d::ui::Layout* PlayGame::CreateActionNodeByData(const GameLogic::ActionNode* action)
{
	//创建新的控件
	auto n = CreateActionNode(action->GetType());
	if (n)
	{
		if (action->GetType() == GameLogic::ActionNodeType::NormalText)
		{
			auto text = dynamic_cast<cocos2d::ui::Text*>(n->getChildByName("Text"));
			if (text)
			{
				text->setString(action->m_action->Text());
			}
			return n;
		}
		else if (action->GetType() == GameLogic::ActionNodeType::Choosing)
		{
			auto btn1 = dynamic_cast<cocos2d::ui::Button*>(n->getChildByName("ChooseBtn1"));
			if (btn1)
			{
				btn1->setTitleText(action->m_stage->ToStage()[0].second);
				btn1->addTouchEventListener(CC_CALLBACK_2(PlayGame::ChooseAction, this));
				btn1->setTag(0);
			}
			auto btn2 = dynamic_cast<cocos2d::ui::Button*>(n->getChildByName("ChooseBtn2"));
			if (btn2)
			{
				btn2->setTitleText(action->m_stage->ToStage()[1].second);
				btn2->addTouchEventListener(CC_CALLBACK_2(PlayGame::ChooseAction, this));
				btn2->setTag(1);
			}
		}
		else if (action->GetType() == GameLogic::ActionNodeType::Choosed)
		{
			auto text = dynamic_cast<cocos2d::ui::Text*>(n->getChildByName("Text"));
			if (text)
			{
				text->setString(action->m_stage->ToStage()[action->m_chooseIndex].second);
			}
		}
	}
	return n;
}

void PlayGame::ReleaseActionNode(cocos2d::ui::Layout* node)
{
	if (node)
	{
		for (int i = 0; i < m_nodeTmplTag.size(); ++i)
		{
			if (node->getTag() == m_nodeTmplTag[i])
			{
				node->setVisible(false);
				m_unusedNodeList[i].push_back(node);
				break;
			}
		}
	}
}

void PlayGame::PlayBringAnimation(int index)
{
	if (m_newIndex != index)
	{
		m_newIndex = index;
		//结束以前的
		if (m_loadingAnimation)
		{
			m_loadingAnimation->setVisible(false);
		}
		//播放新的
		if (m_newIndex >=0)
		{
			if (m_loadingAnimation)
			{
				m_loadingAnimation->setVisible(true);
			}
			this->scheduleOnce(schedule_selector(PlayGame::EndBringAnimation), 2.0f);
		}
	}
}

void PlayGame::EndBringAnimation(float dt)
{
	PlayBringAnimation(-1);
	UpdateActionScrollView(true);
}
