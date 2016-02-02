#include "BaseScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

namespace XUtility
{

	void PlayNodeAction(cocos2d::Node* node, bool isLoop)
	{
		if (node)
		{
			cocostudio::timeline::ActionTimeline* action = nullptr;
			auto tt = node->getTag();
			action = dynamic_cast<cocostudio::timeline::ActionTimeline*>(node->getActionByTag(node->getTag()));
			if (action)
				action->gotoFrameAndPlay(0, isLoop);
		}
	}

	void PlayNodeAction(cocos2d::Node* node, const std::string& name, bool isLoop)
	{
		if (node)
		{
			cocostudio::timeline::ActionTimeline* action = nullptr;
			auto tt = node->getTag();
			action = dynamic_cast<cocostudio::timeline::ActionTimeline*>(node->getActionByTag(node->getTag()));
			if (action)
				action->play(name, isLoop);
		}
	}
}