#include "DangerZone.h"
#include "Spaceship.h"
#include "GameUtils.h"
#include "GameplayScene.h"

bool DangerZone::init()
{
    SpriteName = "PartiallyTransparentRedCircle.png";
    SpaceObjectPhysicsBodyType = PhysicsBodyType::Circle;
    SpaceObjectScreenBoundsType = ScreenBoundsType::Inner;
    SpaceObjectMaxSpeed = 0.f;

    if (!SpaceObject::init())
    {
        return false;
    }

    PhysicsBody->setCategoryBitmask(DANGER_ZONE_COLLISION_CHANNEL);
    PhysicsBody->setCollisionBitmask(SPACESHIP_COLLISION_CHANNEL);
    PhysicsBody->setContactTestBitmask(SPACESHIP_COLLISION_CHANNEL);

    return true;
}

void DangerZone::initDangerZone(cocos2d::Vec2 inPosition, float inRadius, float inTime)
{
    DangerZoneTime = inTime;

    if (cocos2d::Director* director = cocos2d::Director::getInstance())
    {
        const cocos2d::Size visibleSize = director->getVisibleSize();
        const cocos2d::Vec2 origin = director->getVisibleOrigin();
        const cocos2d::Size spriteSize = Sprite->getContentSize();

        setPosition(inPosition);
        setScale(inRadius / spriteSize.width);

        if (LabelTimer = cocos2d::Label::createWithTTF("0", "fonts/Marker Felt.ttf", 60))
        {
            addChild(LabelTimer, 1);
            LabelTimer->setScale(spriteSize.width / inRadius);
            LabelTimer->setVisible(false);
        }
    }
    DangerZoneTimer = DangerZoneTime;
}

void DangerZone::OnCollisionBeginVerified(SpaceObject* otherSpaceObject)
{
    SpaceObject::OnCollisionBeginVerified(otherSpaceObject);
    if (Spaceship* spaceship = dynamic_cast<Spaceship*>(otherSpaceObject))
    {
        SpaceshipInsideDangerZone = spaceship;
        if (DangerZoneTimer >= 0.f)
        {
            DangerZoneTimer = DangerZoneTime;
            AsteroidSpawnTimer = 0.f;
            LabelTimer->setVisible(true);
        }
    }
}

void DangerZone::OnCollisionEndVerified(SpaceObject* otherSpaceObject)
{
    SpaceObject::OnCollisionEndVerified(otherSpaceObject);
    if (Spaceship* spaceship = dynamic_cast<Spaceship*>(otherSpaceObject))
    {
        SpaceshipInsideDangerZone = nullptr;
        LabelTimer->setVisible(false);
    }
}

void DangerZone::update(float deltaTime)
{
    SpaceObject::update(deltaTime);

    if (SpaceshipInsideDangerZone != nullptr)
    {
        if (DangerZoneTimer >= 0.f)
        {
            DangerZoneTimer -= deltaTime;
            LabelTimer->setString(std::to_string(static_cast<int>(DangerZoneTimer)));

            if (AsteroidSpawnTimer <= 0.f)
            {
                AsteroidSpawnTimer = AsteroidSpawnTime;

                if (cocos2d::Node* parent = getParent())
                {
                    if (GameplayScene* parentScene = dynamic_cast<GameplayScene*>(parent))
                    {
                        parentScene->SpawnAsteroid(SpaceshipInsideDangerZone, STARTING_ASTEROID_SPLITS);
                    }
                }
            }

            AsteroidSpawnTimer -= deltaTime;
        }
        else
        {
            if (cocos2d::Node* parent = getParent())
            {
                if (GameplayScene* parentScene = dynamic_cast<GameplayScene*>(parent))
                {
                    parentScene->DangerZoneCompleteProcedure(this);
                }
            }
            removeFromParent();
        }
    }
}