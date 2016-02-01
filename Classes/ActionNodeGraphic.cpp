#include <algorithm>
#include "ActionNodeGraphic.h"
#include "GameLogic.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

ActionNodeGraphicMgr& ActionNodeGraphicMgr::Instance()
{
	static ActionNodeGraphicMgr _inst;
	return _inst;
}

ActionNodeGraphicMgr::ActionNodeGraphicMgr()
{
}


ActionNodeGraphic* ActionNodeGraphicMgr::CreateActionNode(GameLogic::ActionNodeType type, cocos2d::Node* parent)
{
	int t = (int)type;
	if (m_unusedNodeList[t].empty())
	{
		auto a = NewCreateActionNode(type);
		if (a)
		{

			parent->addChild(a->GetNode());
			a->GetNode()->setVisible(true);
		}
		return a;
	}
	auto a = m_unusedNodeList[t].front();
	a->GetNode()->setVisible(true);
	m_unusedNodeList[t].pop_front();
	return a;
}

ActionNodeGraphic* ActionNodeGraphicMgr::CreateActionNodeByData(const GameLogic::ActionNode* action, cocos2d::Node* parent)
{
	auto a = CreateActionNode(action->GetType(), parent);
	if (a)
	{
		a->SetData(action);
	}
	return a;
}

void ActionNodeGraphicMgr::ReleaseActionNode(ActionNodeGraphic* node)
{
	if (node)
	{
		node->GetNode()->setVisible(false);
		m_unusedNodeList[node->GetType()].push_back(node);
	}
}

ActionNodeGraphic* ActionNodeGraphicMgr::NewCreateActionNode(GameLogic::ActionNodeType type)
{
	ActionNodeGraphic* an = nullptr;
	int t = (int)type;
	switch (type)
	{
	case GameLogic::ActionNodeType::NormalText:
		an = new NormalActionNode();
		break;
	case GameLogic::ActionNodeType::Choosing:
		an = new ChoosingActionNode();
		break;
	case GameLogic::ActionNodeType::Choosed:
		an = new ChoosedActionNode();
		break;
	}
	if (an)
	{
		if (!an->Init())
		{
			delete an;
			an = nullptr;
		}
	}
	return an;
}


cocos2d::Node* NormalActionNode::GetNode()
{
	return m_root;
}

cocos2d::Size NormalActionNode::GetSize() const
{
	return m_node->getContentSize();
}

int NormalActionNode::GetType() const
{
	return (int)GameLogic::ActionNodeType::NormalText;
}

bool NormalActionNode::Init()
{
	const std::string nodeName = "NormalNode.csb";
	const std::string loadingName = "NormalTextLoading.csb";
	m_root = CSLoader::createNode(nodeName);
	if (m_root)
	{
		m_node = m_root->getChildByName("Node");
		//创建指定的动画
		if (!loadingName.empty())
		{
			if (m_node)
			{
				m_content = m_node->getChildByName("Content");
				m_loading = CSLoader::createNode(loadingName);
				if (m_loading)
				{
					m_node->addChild(m_loading);
					auto parentSize = m_node->getContentSize();
					auto pos = cocos2d::Vec2{ parentSize.width * 0.5f, parentSize.height * 0.5f };
					m_loading->setPosition(pos);
					auto timeLine = CSLoader::createTimeline(loadingName);
					m_loading->runAction(timeLine);
					if (timeLine)
						timeLine->play("Bring", true);
				}
			}
		}
	}
	return m_root != nullptr;
}

void NormalActionNode::SetData(const GameLogic::ActionNode* action)
{

	if (m_content)
	{
		auto text = dynamic_cast<cocos2d::ui::Text*>(m_content->getChildByName("Text"));
		if (text)
		{
			text->setString(action->m_action->Text());
		}
	}


	if (m_loading)
	{
		m_loading->setVisible(false);
	}

}

void NormalActionNode::ShowLoading(bool show)
{
	if (m_content)
	{
		m_content->setVisible(!show);
	}
	if (m_loading)
	{
		if (m_loading->isVisible() != show)
		{
			m_loading->setVisible(show);
		}
	}
}


cocos2d::Node* TipsActionNode::GetNode()
{
	return m_root;
}

cocos2d::Size TipsActionNode::GetSize() const
{
	return m_node->getContentSize();
}

int TipsActionNode::GetType() const
{
	return (int)GameLogic::ActionNodeType::Tips;
}

bool TipsActionNode::Init()
{
	const std::string nodeName = "NormalNode.csb";
	const std::string loadingName = "NormalTextLoading.csb";
	m_root = CSLoader::createNode(nodeName);
	if (m_root)
	{
		m_node = m_root->getChildByName("Node");
		//创建指定的动画
		if (!loadingName.empty())
		{
			if (m_node)
			{
				m_content = m_node->getChildByName("Content");
				m_loading = CSLoader::createNode(loadingName);
				if (m_loading)
				{
					m_node->addChild(m_loading);
					auto parentSize = m_node->getContentSize();
					auto pos = cocos2d::Vec2{ parentSize.width * 0.5f, parentSize.height * 0.5f };
					m_loading->setPosition(pos);
					auto timeLine = CSLoader::createTimeline(loadingName);
					m_loading->runAction(timeLine);
					if (timeLine)
						timeLine->play("Bring", true);
				}
			}
		}
	}
	return m_root != nullptr;
}

void TipsActionNode::SetData(const GameLogic::ActionNode* action)
{

	if (m_content)
	{
		auto text = dynamic_cast<cocos2d::ui::Text*>(m_content->getChildByName("Text"));
		if (text)
		{
			text->setString(action->m_action->Text());
		}
	}


	if (m_loading)
	{
		m_loading->setVisible(false);
	}

}

void TipsActionNode::ShowLoading(bool show)
{
	if (m_content)
	{
		m_content->setVisible(!show);
	}
	if (m_loading)
	{
		if (m_loading->isVisible() != show)
		{
			m_loading->setVisible(show);
		}
	}
}

cocos2d::Node* ChoosingActionNode::GetNode()
{
	return m_root;
}

cocos2d::Size ChoosingActionNode::GetSize() const
{
	return m_node->getContentSize();
}

int ChoosingActionNode::GetType() const
{
	return (int)GameLogic::ActionNodeType::Choosing;
}

bool ChoosingActionNode::Init()
{
	const std::string nodeName = "ChoosingNode.csb";
	m_root = CSLoader::createNode(nodeName);
	if (m_root)
	{
		m_node = m_root->getChildByName("Node");
	}
	return m_root != nullptr;
}

void ChoosingActionNode::SetData(const GameLogic::ActionNode* action)
{
	if (m_node)
	{
		auto btn1 = dynamic_cast<cocos2d::ui::Button*>(m_node->getChildByName("ChooseBtn1"));
		if (btn1)
		{
			btn1->setTitleText(action->m_stage->ToStage()[0].second);
			btn1->addTouchEventListener(CC_CALLBACK_2(ChoosingActionNode::ChooseAction, this));
			btn1->setTag(0);
		}
		auto btn2 = dynamic_cast<cocos2d::ui::Button*>(m_node->getChildByName("ChooseBtn2"));
		if (btn2)
		{
			btn2->setTitleText(action->m_stage->ToStage()[1].second);
			btn2->addTouchEventListener(CC_CALLBACK_2(ChoosingActionNode::ChooseAction, this));
			btn2->setTag(1);
		}
	}
}

void ChoosingActionNode::ChooseAction(cocos2d::Ref* target, cocos2d::ui::Widget::TouchEventType type)
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

void ChoosingActionNode::ShowLoading(bool show)
{
}

cocos2d::Node* ChoosedActionNode::GetNode()
{
	return m_root;
}

cocos2d::Size ChoosedActionNode::GetSize() const
{
	return m_node->getContentSize();
}

int ChoosedActionNode::GetType() const
{
	return (int)GameLogic::ActionNodeType::Choosed;
}

bool ChoosedActionNode::Init()
{
	const std::string nodeName = "ChoosedNode.csb";
	m_root = CSLoader::createNode(nodeName);
	if (m_root)
	{
		m_node = m_root->getChildByName("Node");
	}
	return m_root != nullptr;
}

void ChoosedActionNode::SetData(const GameLogic::ActionNode* action)
{
	int chooseIndex = action->m_chooseIndex;
	if (m_node)
	{
		auto node1 = m_node->getChildByName("Node1");
		if (node1)
		{
			auto image0 = node1->getChildByName("Image0");
			auto image1 = node1->getChildByName("Image1");
			image0->setVisible(chooseIndex == 0);
			image1->setVisible(chooseIndex != 0);
			auto text = dynamic_cast<cocos2d::ui::Text*>(node1->getChildByName("Text"));
			if (text)
			{
				text->setString(action->m_stage->ToStage().at(0).second);
			}
		}
		auto node2 = m_node->getChildByName("Node2");
		if (node2)
		{
			auto image0 = node2->getChildByName("Image0");
			auto image1 = node2->getChildByName("Image1");
			image0->setVisible(chooseIndex == 1);
			image1->setVisible(chooseIndex != 1);
			auto text = dynamic_cast<cocos2d::ui::Text*>(node2->getChildByName("Text"));
			if (text)
			{
				text->setString(action->m_stage->ToStage().at(1).second);
			}
		}
	}

}

void ChoosedActionNode::ShowLoading(bool show)
{
}
