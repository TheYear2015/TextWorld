#ifndef __XUTILITY_COCOS_HELPER_H__
#define __XUTILITY_COCOS_HELPER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

namespace XUtility
{
	void PlayNodeAction(cocos2d::Node* node, bool isLoop);
	void PlayNodeAction(cocos2d::Node* node, const std::string& name, bool isLoop);
}

#endif // __XUTILITY_COCOS_HELPER_H__
