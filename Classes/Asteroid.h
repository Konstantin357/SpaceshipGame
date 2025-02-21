#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "cocos2d.h"
#include "SpaceObject.h"

class Asteroid : public SpaceObject
{
public:
    virtual bool init() override;
    void initAsteroid(SpaceObject* aimTarget, int inSplitsLeft, cocos2d::Vec2 inPosition = cocos2d::Vec2::ZERO);

    CREATE_FUNC(Asteroid);

    void SetFuturePosition(cocos2d::Vec2 inFuturePosition);
    
private:
    cocos2d::Vec2 SpeedRange;
    int SplitsLeft;
    const int StartingSplits = STARTING_ASTEROID_SPLITS;

    cocos2d::Vec2 FuturePosition = cocos2d::Vec2::ZERO;

    void Split();

protected:
    virtual void update(float deltaTime) override;

    virtual void OnCollisionBeginVerified(SpaceObject* otherSpaceObject) override;
};
#endif // __ASTEROID_H__