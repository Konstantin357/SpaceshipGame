#include "Projectile.h"

bool Projectile::init()
{
    SpriteName = "Projectile.png";
    SpaceObjectPhysicsBodyType = PhysicsBodyType::Circle;
    SpaceObjectScreenBoundsType = ScreenBoundsType::Outer;
    SpaceObjectMaxSpeed = 800.f;

    if (!SpaceObject::init())
    {
        return false;
    }

    PhysicsBody->setCategoryBitmask(PROJECTILE_COLLISION_CHANNEL);
    PhysicsBody->setCollisionBitmask(ASTEROID_COLLISION_CHANNEL);
    PhysicsBody->setContactTestBitmask(ASTEROID_COLLISION_CHANNEL);

    return true;
}

void Projectile::update(float deltaTime)
{
    SpaceObject::update(deltaTime);

    const cocos2d::Vec2 currentPosition = getPosition();
    cocos2d::Vec2 deltaPosition = GetVelocityDirection() * SpaceObjectMaxSpeed * deltaTime;
    cocos2d::Vec2 nextPosition = currentPosition + deltaPosition;
    nextPosition = ClampPosition(nextPosition);
    setPosition(nextPosition);

    if (LastScreenBumpType != ScreenEdgeType::None)
    {
        removeFromParent();
    }
}