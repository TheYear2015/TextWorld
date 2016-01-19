#include "GameLaunchScene.h"
#include "GameSceneManager.h"
#include "GameLogic.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

const char* GameLaunch::GetSceneCsb() const
{
	return "LaunchScene.csb";
}

void GameLaunch::OnSceneInited()
{
	//throw std::logic_error("The method or operation is not implemented.");
}

void GameLaunch::onEnterTransitionDidFinish()
{
	BaseScene::onEnterTransitionDidFinish();

	GameLogic::DataManager::Instance().ReadGameDataFromFile("ContentStage.pck");

	XUtility::GameSceneManager::Instance().EnterScene(XUtility::ScenenType::MainMenu);
}
