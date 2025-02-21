#include "GameplayScene.h"
#include "Spaceship.h"
#include "Asteroid.h"
#include "DangerZone.h"
#include "GameUtils.h"

bool GameplayScene::init()
{
    if ( !cocos2d::Scene::initWithPhysics() )
    {
        return false;
    }

    AsteroidSpawnTimer = GameUtils::RandF(AsteroidSpawnTime.x, AsteroidSpawnTime.y);
    DangerZonesInfo.push_back(DangerZoneInfo(cocos2d::Vec2(50.f, 50.f), 400.f, 10.f));
    DangerZonesInfo.push_back(DangerZoneInfo(cocos2d::Vec2(50.f, 500.f), 400.f, 12.f));
    DangerZonesInfo.push_back(DangerZoneInfo(cocos2d::Vec2(1000.f, 250.f), 500.f, 15.f));

    initGameplay();
    scheduleUpdate();

    return true;
}

void GameplayScene::initGameplay()
{
    if (cocos2d::Director* director = cocos2d::Director::getInstance())
    {
        cocos2d::Size visibleSize = director->getVisibleSize();
        cocos2d::Vec2 origin = director->getVisibleOrigin();

        if (SpaceshipPtr = Spaceship::create())
        {
            addChild(SpaceshipPtr);
            SpaceshipPtr->setPosition(cocos2d::Vec2(visibleSize.width / 2.f + origin.x, visibleSize.height / 2.f + origin.y));
        }

        if (SpritePartiallyTransparent = cocos2d::Sprite::create("PartiallyTransparent.png"))
        {
            SpritePartiallyTransparent->setPosition(cocos2d::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
            cocos2d::Size spritePartiallyTransparentSize = SpritePartiallyTransparent->getContentSize();
            SpritePartiallyTransparent->setScale(visibleSize.width / spritePartiallyTransparentSize.width, visibleSize.height / spritePartiallyTransparentSize.height);
            addChild(SpritePartiallyTransparent, 1);
            SpritePartiallyTransparent->setVisible(false);
        }

        if (LabelGamePaused = cocos2d::Label::createWithTTF("Game paused", "fonts/Marker Felt.ttf", 60))
        {
            LabelGamePaused->setPosition(cocos2d::Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - LabelGamePaused->getContentSize().height - 300.f));
            addChild(LabelGamePaused, 2);
            LabelGamePaused->setVisible(false);
        }

        if (LabelScore = cocos2d::Label::createWithTTF("0", "fonts/Marker Felt.ttf", 60))
        {
            LabelScore->setPosition(cocos2d::Vec2(origin.x + visibleSize.width / 2, origin.y + 20.f));
            addChild(LabelScore, 2);
        }

        if (EventListenerKeyboard = cocos2d::EventListenerKeyboard::create())
        {
            EventListenerKeyboard->onKeyPressed = CC_CALLBACK_2(GameplayScene::onKeyPressed, this);
            EventListenerKeyboard->onKeyReleased = CC_CALLBACK_2(GameplayScene::onKeyReleased, this);
            _eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerKeyboard, this);
        }

        for (DangerZoneInfo& dangerZoneInfo : DangerZonesInfo)
        {
            if (DangerZone* dangerZone = DangerZone::create())
            {
                DangerZonesOnScene.push_back(dangerZone);
                addChild(dangerZone, 0);
                dangerZone->initDangerZone(dangerZoneInfo.Position + cocos2d::Vec2(dangerZoneInfo.Radius, dangerZoneInfo.Radius) / 2.f, dangerZoneInfo.Radius, dangerZoneInfo.Time);
            }
        }

        if (cocos2d::UserDefault* userDefault = cocos2d::UserDefault::getInstance())
        {
            userDefault->setIntegerForKey(LAST_SCORE_STRING, 0);
            userDefault->setIntegerForKey(GAME_TIME_STRING, 0);
        }

        GameStartTime = std::chrono::system_clock::now();

        //getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
    }
}

class GameOverScene;
void GameplayScene::GameOverProcedure()
{
    if (cocos2d::UserDefault* userDefault = cocos2d::UserDefault::getInstance())
    {
        const int lastScore = userDefault->getIntegerForKey(LAST_SCORE_STRING, 0);
        const int bestScore = userDefault->getIntegerForKey(BEST_SCORE_STRING, 0);
        if (lastScore > bestScore)
        {
            userDefault->setIntegerForKey(BEST_SCORE_STRING, lastScore);
        }
        const std::chrono::system_clock::duration gameTime = std::chrono::system_clock::now() - GameStartTime;
        const std::chrono::seconds gameTimeSec = std::chrono::duration_cast<std::chrono::seconds>(gameTime);
        const int gameTimeSecInt = gameTimeSec.count();
        userDefault->setIntegerForKey(GAME_TIME_STRING, gameTimeSecInt);
        userDefault->flush();
    }
    GameUtils::GoToGameScene<GameOverScene>(this);
}

Asteroid* GameplayScene::SpawnAsteroid(SpaceObject* aimTarget, int inSplitsLeft, cocos2d::Vec2 inPosition/* = cocos2d::Vec2::ZERO*/)
{
    Asteroid* asteroid = Asteroid::create();
    addChild(asteroid);
    asteroid->initAsteroid(aimTarget, inSplitsLeft, inPosition);
    asteroid->SetFuturePosition(inPosition);
    return asteroid;
}

void GameplayScene::IncrementScore()
{
    if (cocos2d::UserDefault* userDefault = cocos2d::UserDefault::getInstance())
    {
        int score = userDefault->getIntegerForKey(LAST_SCORE_STRING, 0) + 1;
        userDefault->setIntegerForKey(LAST_SCORE_STRING, score);
        LabelScore->setString(std::to_string(score));
    }
}

void GameplayScene::DangerZoneCompleteProcedure(DangerZone* inDangerZone)
{
    DangerZonesOnScene.erase(std::remove(DangerZonesOnScene.begin(), DangerZonesOnScene.end(), inDangerZone), DangerZonesOnScene.end());

    if (DangerZonesOnScene.size() == 0)
    {
        GameOverProcedure();
    }
}

void GameplayScene::update(float deltaTime)
{
    AsteroidSpawnTimer -= deltaTime;

    if (AsteroidSpawnTimer < 0.f)
    {
        AsteroidSpawnTimer = GameUtils::RandF(AsteroidSpawnTime.x, AsteroidSpawnTime.y);

        SpawnAsteroid(nullptr, STARTING_ASTEROID_SPLITS);
    }
}

void GameplayScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
}

void GameplayScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
    {
        bGameIsPaused = !bGameIsPaused;
        const cocos2d::Vector<cocos2d::Node*>& children = getChildren();
        for (cocos2d::Node* child : children)
        {
            if (bGameIsPaused == true)
                child->pause();
            else
                child->resume();
        }

        if (bGameIsPaused == true)
        {
            SpritePartiallyTransparent->setVisible(true);
            LabelGamePaused->setVisible(true);
            unscheduleUpdate();
        }
        else
        {
            SpritePartiallyTransparent->setVisible(false);
            LabelGamePaused->setVisible(false);
            scheduleUpdate();
        }
    }
}