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
	m_actionCellArray.reserve(1000);

	auto l = SceneRoot()->getChildByName("ActionListLayer");
	if (l)
		m_actionScrollView = dynamic_cast<cocos2d::ui::ScrollView*>(l->getChildByName("ActionScrollView"));

	if (m_actionScrollView)
	{
		m_normalTextNodeTmpl = dynamic_cast<cocos2d::ui::Layout*>(m_actionScrollView->getChildByName("NormalTextNode"));
		m_chooseNodeTmpl = dynamic_cast<cocos2d::ui::Layout*>(m_actionScrollView->getChildByName("ChooseNode"));
		if (m_normalTextNodeTmpl)
		{
			m_normalTextNodeTmpl->setVisible(false);
			m_normalTextNodeSize = m_normalTextNodeTmpl->getContentSize();
		}
		if (m_chooseNodeTmpl)
		{
			m_chooseNodeTmpl->setVisible(false);
			m_chooseNodeSize = m_chooseNodeTmpl->getContentSize();
		}

		m_actionScrollView->addEventListener(CC_CALLBACK_2(PlayGame::OnActionListScrollViewEvent, this));
	}

}

cocos2d::ui::Layout* PlayGame::CreateNormalTextNode()
{
	if (m_unusedNormalTextNodeList.empty())
	{
		auto n = m_normalTextNodeTmpl->clone();
		n->setTag(NormalTextNodeTage);
		m_actionScrollView->addChild(n);
		n->setVisible(true);
		return dynamic_cast<cocos2d::ui::Layout*>(n);
	}
	auto n = m_unusedNormalTextNodeList.front();
	m_unusedNormalTextNodeList.pop_front();
	return n;
}

cocos2d::ui::Layout* PlayGame::CreateChooseNode()
{
	if (m_unusedChooseNodeList.empty())
	{
		auto n = m_chooseNodeTmpl->clone();
		n->setTag(ChooseNodeTage);
		m_actionScrollView->addChild(n);
		n->setVisible(true);
		return dynamic_cast<cocos2d::ui::Layout*>(n);
	}
	auto n = m_unusedChooseNodeList.front();
	m_unusedChooseNodeList.pop_front();
	return n;

}

void PlayGame::ReleaseNormalTextNode(cocos2d::ui::Layout* node)
{
	if (node && node->getTag() == NormalTextNodeTage)
	{
		node->setVisible(false);
		m_unusedNormalTextNodeList.push_back(node);
	}
}

void PlayGame::ReleaseChooseNode(cocos2d::ui::Layout* node)
{
	if (node && node->getTag() == ChooseNodeTage)
	{
		node->setVisible(false);
		m_unusedChooseNodeList.push_back(node);
	}

}

void PlayGame::onEnter()
{
	BaseScene::onEnter();
	GameLogic::GameCore::Instance().SetInterface(this);

	////�����û����ݣ�����actionList
	//TODO:��ȡ�û���������
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
	//��Ϸ����
	GameLogic::GameCore::Instance().Begin();
}


void PlayGame::OnEnterStage(const GameLogic::StageData* stageData)
{
	if (stageData)
		CCLOG("PlayGame::OnEnterStage %d.", stageData->Id());
	else
		CCLOGERROR("PlayGame::OnEnterStage null.");
}

void PlayGame::OnEnterAction(const GameLogic::StageData* stageData, const GameLogic::StageActionData* actData)
{
	if (actData)
	{
		ActionCell actionCell;
		actionCell.m_stage = stageData;
		actionCell.m_action = actData;

		if (!m_actionCellArray.empty())
		{
			auto& last = m_actionCellArray.back();
			actionCell.m_logicY = last.m_logicY + GetActionCellHeight(last);
		}

		m_actionCellArray.push_back(actionCell);

		CCLOG("PlayGame::OnEnterAction %s.(%d)", actData->Text(), m_actionCellArray.size());

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

void PlayGame::OnNeedChoose(const GameLogic::StageData* stageData)
{
	if (stageData)
	{
		CCLOG("PlayGame::OnNeedChoose %d.", stageData->Id());
		ActionCell actionCell;
		actionCell.m_stage = stageData;
		actionCell.m_action = nullptr;
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
	//��������

	//����µ����ݳߴ�
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

	//���¿ؼ�������λ��
	cocos2d::Vec2 pos = { 0, 0 };
	scrollVPos.x = -scrollVPos.y - size.height * 2;
	scrollVPos.y = -scrollVPos.y + size.height * 3;
	for (auto b = m_actionCellArray.begin(); b != m_actionCellArray.end(); ++b)
	{
		auto& ac = *b;
		pos.y = height - ac.m_logicY;
		//�ж��Ƿ���������
		if (pos.y >= scrollVPos.x && pos.y < scrollVPos.y)
		{
			if (!ac.m_guiNode)
			{
				//�����µĿؼ�
				if (ac.m_action)
				{
					auto n = CreateNormalTextNode();
					auto text = dynamic_cast<cocos2d::ui::Text*>(n->getChildByName("Text"));
					if (text)
					{
						text->setString(ac.m_action->Text());
					}
					ac.m_guiNode = n;
				}
				else
				{
					//�����µĿؼ�
					auto n = CreateChooseNode();
					auto btn1 = dynamic_cast<cocos2d::ui::Button*>(n->getChildByName("ChooseBtn1"));
					if (btn1)
					{
						btn1->setTitleText(ac.m_stage->ToStage()[0].second);
						//btn1->addTouchEventListener(CC_CALLBACK_2(PlayGame::ChooseAction, this));
						btn1->setTag(0);
					}
					auto btn2 = dynamic_cast<cocos2d::ui::Button*>(n->getChildByName("ChooseBtn2"));
					if (btn2)
					{
						btn2->setTitleText(ac.m_stage->ToStage()[1].second);
						//btn2->addTouchEventListener(CC_CALLBACK_2(PlayGame::ChooseAction, this));
						btn2->setTag(1);
					}
					//�����������󣬱�ʾ�Ѿ�������ѡ��
					ac.m_guiNode = n;
				}
			}
			ac.m_guiNode->setVisible(true);
			ac.m_guiNode->setPosition(pos);
		}
		else
		{
			if (ac.m_guiNode)
			{
				if (ac.m_action)
				{
					ReleaseNormalTextNode(ac.m_guiNode);
				}
				else
				{
					ReleaseChooseNode(ac.m_guiNode);
				}
				ac.m_guiNode = nullptr;
			}
		}
	}

	if (isChangeSize)
	{
		//ˢ����ʾ����
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
	return ac.m_action == nullptr ? m_chooseNodeSize.height : m_normalTextNodeSize.height;
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
	{//��������
		UpdateActionScrollView(false);
	}
	break;
	default:
		break;
	}
}
