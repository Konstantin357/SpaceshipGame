#ifndef __DANGERZONE_H__
#define __DANGERZONE_H__

#include "cocos2d.h"
#include "SpaceObject.h"

class Spaceship;

class DangerZone : public SpaceObject
{
public:
    virtual bool init() override;
    void initDangerZone(cocos2d::Vec2 inPosition, float inRadius, float inTime);

    CREATE_FUNC(DangerZone);
    
private:
    cocos2d::Vec2 SpeedRange;

    float AsteroidSpawnTime = 1.f;
    float AsteroidSpawnTimer = 0.f;

    float DangerZoneTime = 10.f;
    float DangerZoneTimer = 0.f;

    Spaceship* SpaceshipInsideDangerZone = nullptr;

    cocos2d::Label* LabelTimer = nullptr;

protected:
    virtual void OnCollisionBeginVerified(SpaceObject* otherSpaceObject) override;
    virtual void OnCollisionEndVerified(SpaceObject* otherSpaceObject) override;
    virtual void update(float deltaTime) override;
};
#endif // __DANGERZONE_H__