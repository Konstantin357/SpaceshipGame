#include "GameOverScene.h"
#include "Spaceship.h"
#include "Asteroid.h"
#include "DangerZone.h"
#include "GameUtils.h"

bool GameOverScene::init()
{
    if ( !cocos2d::Scene::init() )
    {
        return false;
    }

    if (cocos2d::Director* director = cocos2d::Director::getInstance())
    {
        cocos2d::Size visibleSize = director->getVisibleSize();
        cocos2d::Vec2 origin = director->getVisibleOrigin();

        if (cocos2d::Label* labelGameOver = cocos2d::Label::createWithTTF("Game over", "fonts/Marker Felt.ttf", 60))
        {
            labelGameOver->setPosition(cocos2d::Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - labelGameOver->getContentSize().height));
            addChild(labelGameOver, 1);
        }

        if (cocos2d::UserDefault* userDefault = cocos2d::UserDefault::getInstance())
        {
            int bestScore = userDefault->getIntegerForKey(BEST_SCORE_STRING, 0);
            if (cocos2d::Label* labelBestScore = cocos2d::Label::createWithTTF("Best score: " + std::to_string(bestScore), "fonts/Marker Felt.ttf", 60))
            {
                labelBestScore->setPosition(cocos2d::Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - labelBestScore->getContentSize().height - 200.f));
                addChild(labelBestScore, 1);
            }

            int lastScore = userDefault->getIntegerForKey(LAST_SCORE_STRING, 0);
            if (cocos2d::Label* labelLastScore = cocos2d::Label::createWithTTF("Last score: " + std::to_string(lastScore), "fonts/Marker Felt.ttf", 60))
            {
                labelLastScore->setPosition(cocos2d::Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - labelLastScore->getContentSize().height - 300.f));
                addChild(labelLastScore, 1);
            }

            int gameTime = userDefault->getIntegerForKey(GAME_TIME_STRING, 0);
            if (cocos2d::Label* labelGameTime = cocos2d::Label::createWithTTF("Game time: " + std::to_string(gameTime), "fonts/Marker Felt.ttf", 60))
            {
                labelGameTime->setPosition(cocos2d::Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - labelGameTime->getContentSize().height - 450.f));
                addChild(labelGameTime, 1);
            }
        }
    }

    SceneShowTimer = SceneShowTime;
    scheduleUpdate();

    return true;
}

class MainMenu;
void GameOverScene::update(float deltaTime)
{
    SceneShowTime -= deltaTime;

    if (SceneShowTime < 0.f)
    {
        GameUtils::GoToGameScene<MainMenu>(this);
    }
}