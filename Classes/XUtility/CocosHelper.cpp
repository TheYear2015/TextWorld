#include "BaseScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

namespace XUtility
{

	void PlayNodeAndChlidrenAction(cocos2d::Node* node, bool isLoop)
	{
		if (node)
		{
			auto c = node->getActionManager()->getNumberOfRunningActionsInTarget(node);
			if (c > 0)
			{
				auto anm = node->getActionManager();
				//anm->resumeTarget(node);
				CCLOG("sdfdsfdsfsd");
			}
			cocostudio::timeline::ActionTimeline* action = nullptr;
			auto tt = node->getTag();
			action = dynamic_cast<cocostudio::timeline::ActionTimeline*>(node->getActionByTag(node->getTag()));
			if (!action)
			{
				for (int t = 0; t < 1000; ++t)
				{
					action = dynamic_cast<cocostudio::timeline::ActionTimeline*>(node->getActionByTag(t));
					if (action)
						break;
				}
			}
			if (action)
				action->gotoFrameAndPlay(0, isLoop);
			auto children = node->getChildren();
			for (auto cn : children)
			{
				PlayNodeAndChlidrenAction(cn, isLoop);
			}
		}
	}

}