#include "GameUtils.h"
#include "HelloWorldScene.h"
#include "GameplayScene.h"
#include "MainMenuScene.h"
#include "GameOverScene.h"

template<typename T>
inline void GameUtils::GoToGameScene(cocos2d::Ref* sender)
{
	T* nextScene = T::create();

	cocos2d::Director* director = cocos2d::Director::getInstance();
	if (nextScene != nullptr && director != nullptr)
	{
		director->replaceScene(nextScene);
	}
}
template void GameUtils::GoToGameScene<GameplayScene>(cocos2d::Ref* sender);
template void GameUtils::GoToGameScene<GameOverScene>(cocos2d::Ref* sender);
template void GameUtils::GoToGameScene<MainMenu>(cocos2d::Ref* sender);

void GameUtils::ExitGame(cocos2d::Ref* sender)
{
	cocos2d::Director* director = cocos2d::Director::getInstance();
	if (director != nullptr)
	{
		director->end();
	}
}

float GameUtils::RandF(float min, float max)
{
	return (min + (max - min) * static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX)));
}

int GameUtils::RandI(int min, int max)
{
	return (min + (std::rand() % (max + 1 - min)));
}