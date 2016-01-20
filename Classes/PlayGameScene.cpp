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
	//»ñµÃ

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
		}
		if (m_chooseNodeTmpl)
		{
			m_chooseNodeTmpl->setVisible(false);
		}
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
		auto n = m_normalTextNodeTmpl->clone();
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

	//test
	if (m_normalTextNodeTmpl)
	{

		cocos2d::Vec2 pos;
		pos.x = 0;;
		pos.y = 0;
		for (int i = 0; i < 10; ++i)
		{
			auto nTextN = CreateNormalTextNode();
			nTextN->setPosition(pos);
			pos.y += 200;
		}

		auto size = m_actionScrollView->getInnerContainerSize();
		size.height = pos.y + 200;
		m_actionScrollView->setInnerContainerSize(size);
	}
}
