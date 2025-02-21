#include "Asteroid.h"
#include "GameUtils.h"
#include "GameplayScene.h"
#include "Projectile.h"

bool Asteroid::init()
{
    SpeedRange = cocos2d::Vec2(60.f, 120.f);
    SpriteName = "Asteroid.png";
    SpaceObjectPhysicsBodyType = PhysicsBodyType::Circle;
    SpaceObjectScreenBoundsType = ScreenBoundsType::OuterDouble;
    SpaceObjectMaxSpeed = GameUtils::RandF(SpeedRange.x, SpeedRange.y);

    if (!SpaceObject::init())
    {
        return false;
    }

    PhysicsBody->setCategoryBitmask(ASTEROID_COLLISION_CHANNEL);
    PhysicsBody->setCollisionBitmask(SPACESHIP_COLLISION_CHANNEL + PROJECTILE_COLLISION_CHANNEL);
    PhysicsBody->setContactTestBitmask(SPACESHIP_COLLISION_CHANNEL + PROJECTILE_COLLISION_CHANNEL);

    return true;
}

void Asteroid::initAsteroid(SpaceObject* aimTarget, int inSplitsLeft, cocos2d::Vec2 inPosition/* = cocos2d::Vec2::ZERO*/)
{
    if (cocos2d::Director* director = cocos2d::Director::getInstance())
    {
        const cocos2d::Size visibleSize = director->getVisibleSize();
        const cocos2d::Vec2 origin = director->getVisibleOrigin();

        const ScreenEdgeType screenEdge = static_cast<ScreenEdgeType>(GameUtils::RandI(static_cast<int>(ScreenEdgeType::Left), static_cast<int>(ScreenEdgeType::Down)));

        cocos2d::Vec2 asteroidPosition = inPosition;
        if (inSplitsLeft == STARTING_ASTEROID_SPLITS)
        {
            if (screenEdge == ScreenEdgeType::Left || screenEdge == ScreenEdgeType::Right)
            {
                asteroidPosition.x = screenEdge == ScreenEdgeType::Left ? 0.f : visibleSize.width;
                asteroidPosition.y = GameUtils::RandF(0.f, visibleSize.height);
            }
            else
            {
                asteroidPosition.y = screenEdge == ScreenEdgeType::Up ? 0.f : visibleSize.height;
                asteroidPosition.x = GameUtils::RandF(0.f, visibleSize.width);
            }
        }
        setPosition(asteroidPosition);
        PhysicsBody->setPositionOffset(cocos2d::Vec2::ZERO);

        cocos2d::Vec2 asteroidDirection = cocos2d::Vec2::ZERO;
        if (inSplitsLeft != STARTING_ASTEROID_SPLITS)
        {
            asteroidDirection = cocos2d::Vec2(GameUtils::RandF(-1.f, 1.f), GameUtils::RandF(-1.f, 1.f));
        }
        else if (aimTarget != nullptr)
        {
            cocos2d::Vec2 aimPosition = aimTarget->getPosition();
            asteroidDirection = aimPosition - asteroidPosition;
            asteroidDirection.rotate(cocos2d::Vec2::ZERO, GameUtils::RandF(-0.1f, 0.1f));
        }
        else
        {
            if (screenEdge == ScreenEdgeType::Left || screenEdge == ScreenEdgeType::Right)
            {
                asteroidDirection.x = screenEdge == ScreenEdgeType::Left ? 1.f : -1.f;
                asteroidDirection.y = GameUtils::RandF(-2.f, 2.f);
            }
            else
            {
                asteroidDirection.y = screenEdge == ScreenEdgeType::Up ? 1.f : -1.f;
                asteroidDirection.x = GameUtils::RandF(-2.f, 2.f);
            }
        }

        SetVelocityDirection(asteroidDirection);

        SplitsLeft = inSplitsLeft;
        float scale = std::powf(0.5f, static_cast<float>(StartingSplits - SplitsLeft));
        setScale(scale);
    }
}

void Asteroid::SetFuturePosition(cocos2d::Vec2 inFuturePosition)
{
    FuturePosition = inFuturePosition;
}

void Asteroid::Split()
{
    if (SplitsLeft > 0)
    {
        if (cocos2d::Node* parent = getParent())
        {
            if (GameplayScene* parentScene = dynamic_cast<GameplayScene*>(parent))
            {
                parentScene->IncrementScore();
                const cocos2d::Vec2 asteroidPosition = getPosition();
                parentScene->SpawnAsteroid(nullptr, SplitsLeft - 1, asteroidPosition);
                parentScene->SpawnAsteroid(nullptr, SplitsLeft - 1, asteroidPosition);
            }
        }
    }
    removeFromParent();
}

void Asteroid::update(float deltaTime)
{
    SpaceObject::update(deltaTime);

    if (!FuturePosition.isZero())
    {
        setPosition(FuturePosition);
        FuturePosition.setZero();
    }

    const cocos2d::Vec2 currentPosition = getPosition();
    const cocos2d::Vec2 deltaPosition = GetVelocityDirection() * SpaceObjectMaxSpeed * deltaTime;
    cocos2d::Vec2 nextPosition = currentPosition + deltaPosition;
    nextPosition = ClampPosition(nextPosition);
    setPosition(nextPosition);

    if (LastScreenBumpType != ScreenEdgeType::None)
    {
        removeFromParent();
    }
}

void Asteroid::OnCollisionBeginVerified(SpaceObject* otherSpaceObject)
{
    SpaceObject::OnCollisionBeginVerified(otherSpaceObject);
    if (Projectile* projectile = dynamic_cast<Projectile*>(otherSpaceObject))
    {
        projectile->removeFromParent();
        Split();
    }
}