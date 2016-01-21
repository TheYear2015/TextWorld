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
	GameLogic::GameCore::Instance().SetInterface(this);

	//�����û����ݣ�����actionList
	m_actionCellArray.clear();
	ActionCell actionCell;
	float beginY = 0;
	auto stageList = GameLogic::GameCore::Instance().GetPlayedStageList();
	for (auto sd : stageList)
	{
		actionCell.m_stageId = sd->Id();
		for (int i = 0; i < sd->ActionList().size(); ++i)
		{
			actionCell.m_y = beginY;
			actionCell.m_actionIndex = i;
			m_actionCellArray.push_back(actionCell);
			//if (sd->ActionList[i].Type() == 1)
			{
				beginY += m_normalTextNodeSize.height;
			}
		}
		if (sd->IsHaveChooseAtEnd())
		{
			actionCell.m_y = beginY;
			actionCell.m_actionIndex = -1;
			m_actionCellArray.push_back(actionCell);
			beginY += m_chooseNodeSize.height;
		}
	}

	//���ؼ�
	cocos2d::Vec2 pos;
	for (auto& c : m_actionCellArray)
	{
		if (c.m_actionIndex < 0)
		{
			auto n = CreateChooseNode();
			if (n)
			{
				pos.y = c.m_y;
				n->setPosition(pos);
			}
		}
		else
		{
			auto n = CreateNormalTextNode();
			if (n)
			{
				pos.y = c.m_y;
				n->setPosition(pos);
			}
		}
	}



	////test
	//if (m_normalTextNodeTmpl)
	//{

	//	cocos2d::Vec2 pos;
	//	pos.x = 0;;
	//	pos.y = 0;
	//	for (int i = 0; i < 1000; ++i)
	//	{
	//		auto nTextN = CreateNormalTextNode();
	//		nTextN->setPosition(pos);
	//		pos.y += 200;
	//	}

	//	auto size = m_actionScrollView->getInnerContainerSize();
	//	size.height = pos.y + 200;
	//	m_actionScrollView->setInnerContainerSize(size);
	//}
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

void PlayGame::OnEnterAction(const GameLogic::StageActionData* actData)
{
	if (actData)
	{
		CCLOG("PlayGame::OnEnterAction %s.", actData->Text());
		//�����µĿؼ�
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
		CCLOG("PlayGame::OnNeedChoose %d.", stageData->Id());
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

void PlayGame::update(float delta)
{
	BaseScene::update(delta);
	GameLogic::GameCore::Instance().Update(delta);
}
