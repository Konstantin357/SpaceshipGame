#ifndef __GAMEPLAY_SCENE_H__
#define __GAMEPLAY_SCENE_H__

#include "cocos2d.h"
#include "GameBase.h"

class Spaceship;
class Asteroid;
class SpaceObject;
class DangerZone;

class GameplayScene : public cocos2d::Scene
{
public:
    virtual bool init() override;
    void initGameplay();
    
    CREATE_FUNC(GameplayScene);

    void GameOverProcedure();

    Asteroid* SpawnAsteroid(SpaceObject* aimTarget, int inSplitsLeft, cocos2d::Vec2 inPosition = cocos2d::Vec2::ZERO);

    void IncrementScore();

    void DangerZoneCompleteProcedure(DangerZone* inDangerZone);

private:
    Spaceship* SpaceshipPtr = nullptr;
    cocos2d::Sprite* SpritePartiallyTransparent = nullptr;
    cocos2d::Label* LabelGamePaused = nullptr;
    
    cocos2d::Label* LabelScore = nullptr;

    std::vector<DangerZone*> DangerZonesOnScene;

    virtual void update(float deltaTime);

    cocos2d::Vec2 AsteroidSpawnTime = cocos2d::Vec2(2.f, 4.f);
    float AsteroidSpawnTimer = 0.f;

    std::vector<DangerZoneInfo> DangerZonesInfo;

    cocos2d::EventListenerKeyboard* EventListenerKeyboard = nullptr;
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    bool bGameIsPaused = false;

    std::chrono::system_clock::time_point GameStartTime;
};
#endif // __GAMEPLAY_SCENE_H__
