#ifndef __PLAY_GAME_SCENE_H__
#define __PLAY_GAME_SCENE_H__

#include "cocos2d.h"
#include "BaseScene.h"

class PlayGame : public XUtility::BaseSceneImpl<PlayGame>
{
protected:
	virtual const char* GetSceneCsb() const override;
};

#endif // __PLAY_GAME_SCENE_H__
