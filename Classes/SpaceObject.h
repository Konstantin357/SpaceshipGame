#ifndef __SPACEOBJECT_H__
#define __SPACEOBJECT_H__

#include "cocos2d.h"
#include "GameBase.h"

class SpaceObject : public cocos2d::Node
{
public:
    virtual bool init() override;

    void SetVelocityDirection(cocos2d::Vec2 inVelocityDirection);

    CREATE_FUNC(SpaceObject);
protected:
    cocos2d::Sprite* Sprite = nullptr;
    cocos2d::PhysicsBody* PhysicsBody = nullptr;

    virtual void update(float deltaTime);

    cocos2d::Vec2 BoundsMin;
    cocos2d::Vec2 BoundsMax;

    //Define before init!
    std::string SpriteName;
    ScreenBoundsType SpaceObjectScreenBoundsType;
    PhysicsBodyType SpaceObjectPhysicsBodyType;
    float SpaceObjectMaxSpeed;

    cocos2d::Vec2 GetVelocityDirection();

    cocos2d::Vec2 ClampPosition(const cocos2d::Vec2 inPosition);

    virtual void OnScreenEdgeBumped();
    bool bScreenEdgeBumped = false;
    ScreenEdgeType LastScreenBumpType = ScreenEdgeType::None;

    cocos2d::EventListenerPhysicsContact* ListenerPhysicsContact = nullptr;
    virtual bool OnPhysicsContactBegin(cocos2d::PhysicsContact& contact);
    virtual bool OnPhysicsContactEnd(cocos2d::PhysicsContact& contact);
    virtual void OnCollisionBeginVerified(SpaceObject* otherSpaceObject);
    virtual void OnCollisionEndVerified(SpaceObject* otherSpaceObject);

private:
    cocos2d::Vec2 VelocityDirection;

    SpaceObject* FindOtherSpaceObjectOnPhysicsContact(cocos2d::PhysicsContact& contact);
};
#endif // __SPACEOBJECT_H__