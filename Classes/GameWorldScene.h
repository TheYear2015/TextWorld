#ifndef __GAME_WORLD_SCENE_H__
#define __GAME_WORLD_SCENE_H__

#include "cocos2d.h"
#include "BaseScene.h"

class GameWorld : public XUtility::BaseSceneImpl<GameWorld>
{
protected:
	virtual const char* GetSceneCsb() const override;
};

#endif // __GAME_WORLD_SCENE_H__
