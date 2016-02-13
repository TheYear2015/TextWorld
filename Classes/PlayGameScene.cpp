#include <algorithm>
#include "PlayGameScene.h"
#include "GameLogic.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "XUtility/AudioFunction.h"

USING_NS_CC;

using namespace cocostudio::timeline;

const char* PlayGame::GetSceneCsb() const
{
	return "GameScene.csb";
}

void PlayGame::OnSceneInited()
{
	m_actionCellArray.reserve(10000);

	auto l = SceneRoot()->getChildByName("ActionListLayer");
	if (l)
		m_actionScrollView = dynamic_cast<cocos2d::ui::ScrollView*>(l->getChildByName("ActionScrollView"));

	if (m_actionScrollView)
	{
		for (int i = 0; i < (int)GameLogic::ActionNodeType::Count; ++i)
		{
			auto n = ActionNodeGraphicMgr::Instance().CreateActionNode((GameLogic::ActionNodeType)i, m_actionScrollView);
			if (n)
			{
				m_nodeSize[i] = n->GetSize();
			}
			ActionNodeGraphicMgr::Instance().ReleaseActionNode(n);
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

	//加载自身的音乐，加载后续场景的音乐
	if (!stageData->Music().empty())
	{
		XUtility::AudioManager::Instance().InitMusic(stageData->Music().c_str());

		//延迟播放音乐
		this->scheduleOnce(schedule_selector(PlayGame::PlayStageMusic), 1.0f);
	}

	auto next = stageData->ToStage();
	for (auto& n : next)
	{
		auto s = GameLogic::DataManager::Instance().GetStageData( n.first);
		if (s && !s->Music().empty())
		{
			XUtility::AudioManager::Instance().InitMusic(s->Music().c_str());
		}
	}
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
		else
		{
			actionCell.m_logicY = 20;
		}

		m_actionCellArray.push_back(actionCell);

		if (actNode->m_action)
			CCLOG("PlayGame::OnEnterAction %s.(%d)", actNode->m_action->Text().c_str(), m_actionCellArray.size());

		if (actNode->GetType() == GameLogic::ActionNodeType::NormalText)
		{
			PlayBringAnimation(m_actionCellArray.size() - 1);
		}

		bool isOnBottom = m_actionScrollView->getInnerContainerPosition().y <= m_actionScrollView->getCustomSize().height;
		UpdateActionScrollView(true);
		if (isOnBottom)
		{
			m_actionScrollView->scrollToBottom(0.5f, true);
		}

		auto soundDef = GameLogic::GameCore::Instance().GetActionSoundDef(actNode);
		if (soundDef)
		{
			for (int i = 0; i < (int)GameLogic::StageActionData::SoundChannelDef::Define::ChannelCount; ++i)
			{
				auto& s = soundDef->SoundName(i);
				if (!s.empty())
				{
					auto loop = soundDef->IsLoop(i);
					XUtility::AudioManager::Instance().PlaySoundInChannel(i, s.c_str(), loop);
				}
				else
				{
					auto isStop = soundDef->IsStop(i);
					if (isStop)
					{
						XUtility::AudioManager::Instance().StopSoundChannel(i);
					}
				}
			}
		}
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

		bool isOnBottom = m_actionScrollView->getInnerContainerPosition().y <= m_actionScrollView->getCustomSize().height;
		UpdateActionScrollView(true);
		if (isOnBottom)
		{
			m_actionScrollView->scrollToBottom(0.5f, true);
		}

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
			height = last.m_logicY + GetActionCellHeight(last) + 20;
		}
		height = std::max(size.height, height);
	}
	else
	{
		height = m_actionScrollView->getInnerContainerSize().height;
	}

	//更新控件并更新位置
	cocos2d::Vec2 pos = { size.width * 0.5f, 0 };
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
				if (ac.m_guiNode->GetType() != (int)ac.m_action->GetType())
				{
					ActionNodeGraphicMgr::Instance().ReleaseActionNode(ac.m_guiNode);
					ac.m_guiNode = nullptr;
				}
			}
			if (!ac.m_guiNode)
			{
				//创建新的控件
				ac.m_guiNode = ActionNodeGraphicMgr::Instance().CreateActionNodeByData(ac.m_action, m_actionScrollView);
			}
			ac.m_guiNode->GetNode()->setPosition(pos);

			ShowBringAnimation(ac.m_guiNode, ii == m_newIndex);
		}
		else
		{
			if (ac.m_guiNode)
			{
				ActionNodeGraphicMgr::Instance().ReleaseActionNode(ac.m_guiNode);
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

void PlayGame::PlayBringAnimation(int index)
{
	if (m_newIndex != index)
	{
		m_newIndex = index;
		this->scheduleOnce(schedule_selector(PlayGame::EndBringAnimation), 2.0f);
	}
}

void PlayGame::EndBringAnimation(float dt)
{
	PlayBringAnimation(-1);
	UpdateActionScrollView(true);
}

void PlayGame::ShowBringAnimation(ActionNodeGraphic* node, bool show)
{
	if (node) 
	{
		node->ShowLoading(show);
	}
}

void PlayGame::PlayStageMusic(float dt)
{
	auto stage = GameLogic::GameCore::Instance().GetCurrentStage();
	XUtility::AudioManager::Instance().PlayMusic(stage->Music().c_str());

}

void PlayGame::OnBackToAction(const GameLogic::ActionNode* actNode)
{
	bool needUpdate = false;
	for (auto i = m_actionCellArray.begin(); i != m_actionCellArray.end(); ++i)
	{
		if (i->m_action == actNode)
		{
			++i;
			if (i != m_actionCellArray.end())
			{
				for (auto __i = i; __i != m_actionCellArray.end(); ++__i)
				{
					ActionNodeGraphicMgr::Instance().ReleaseActionNode(__i->m_guiNode);
				}
				m_actionCellArray.erase(i, m_actionCellArray.end());
				needUpdate = true;
			}
			break;
		}
	}
	if (needUpdate)
		UpdateActionScrollView(true);
}
