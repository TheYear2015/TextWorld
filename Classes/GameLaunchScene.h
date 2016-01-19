#ifndef __GAME_LAUNCH_SCENE_H__
#define __GAME_LAUNCH_SCENE_H__

#include "BaseScene.h"

class GameLaunch : public XUtility::BaseSceneImpl<GameLaunch>
{
protected:
	virtual const char* GetSceneCsb() const override;

	virtual void OnSceneInited() override;

	virtual void onEnterTransitionDidFinish() override;

private:
	void EnterMainMenu(float dt);

};


#endif // __GAME_LAUNCH_SCENE_H__
