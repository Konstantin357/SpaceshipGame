#include "Spaceship.h"
#include "Projectile.h"
#include "GameplayScene.h"
#include "Asteroid.h"

bool Spaceship::init()
{
    SpriteName = "Spaceship.png";
    SpaceObjectPhysicsBodyType = PhysicsBodyType::Circle;
    SpaceObjectScreenBoundsType = ScreenBoundsType::Inner;
    SpaceObjectMaxSpeed = 100.f;

    if (!SpaceObject::init())
    {
        return false;
    }

    PhysicsBody->setCategoryBitmask(SPACESHIP_COLLISION_CHANNEL);
    PhysicsBody->setCollisionBitmask(ASTEROID_COLLISION_CHANNEL + DANGER_ZONE_COLLISION_CHANNEL);
    PhysicsBody->setContactTestBitmask(ASTEROID_COLLISION_CHANNEL + DANGER_ZONE_COLLISION_CHANNEL);

    EventListenerKeyboard = cocos2d::EventListenerKeyboard::create();
    EventListenerKeyboard->onKeyPressed = CC_CALLBACK_2(Spaceship::onKeyPressed, this);
    EventListenerKeyboard->onKeyReleased = CC_CALLBACK_2(Spaceship::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerKeyboard, this);

    EventListenerMouse = cocos2d::EventListenerMouse::create();
    EventListenerMouse->onMouseDown = CC_CALLBACK_1(Spaceship::onMousePressed, this);
    EventListenerMouse->onMouseUp = CC_CALLBACK_1(Spaceship::onMouseReleased, this);
    EventListenerMouse->onMouseMove = CC_CALLBACK_1(Spaceship::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerMouse, this);

    return true;
}

void Spaceship::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    if (KeysPressed.find(keyCode) == KeysPressed.end())
    {
        KeysPressed[keyCode] = std::chrono::system_clock::now();
    }
}

void Spaceship::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    if (KeysPressed.find(keyCode) != KeysPressed.end())
    {
        std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - KeysPressed[keyCode];
    }
    KeysPressed.erase(keyCode);
}

void Spaceship::onMousePressed(cocos2d::EventMouse* event)
{
    if (cocos2d::EventMouse* eventMouse = (cocos2d::EventMouse*)event)
    {
        cocos2d::EventMouse::MouseButton mouseButton = eventMouse->getMouseButton();
        if (mouseButton == cocos2d::EventMouse::MouseButton::BUTTON_LEFT)
        {
            bFireState = true;
        }
    }
}

void Spaceship::onMouseReleased(cocos2d::EventMouse* event)
{
    if (cocos2d::EventMouse* eventMouse = (cocos2d::EventMouse*)event)
    {
        cocos2d::EventMouse::MouseButton mouseButton = eventMouse->getMouseButton();
        if (mouseButton == cocos2d::EventMouse::MouseButton::BUTTON_LEFT)
        {
            bFireState = false;
        }
    }
}

void Spaceship::onMouseMove(cocos2d::EventMouse* event)
{
    MouseLocation = event->getLocationInView();
}

void Spaceship::OnCollisionBeginVerified(SpaceObject* otherSpaceObject)
{
    SpaceObject::OnCollisionBeginVerified(otherSpaceObject);
    if (Asteroid* asteroid = dynamic_cast<Asteroid*>(otherSpaceObject))
    {
        if (cocos2d::Node* parent = getParent())
        {
            if (GameplayScene* parentScene = dynamic_cast<GameplayScene*>(parent))
            {
                parentScene->GameOverProcedure();
            }
        }
    }
}

void Spaceship::update(float deltaTime)
{
    SpaceObject::update(deltaTime);

    const cocos2d::Vec2 currentPosition = getPosition();
    cocos2d::Vec2 deltaPosition = cocos2d::Vec2(0.f, 0.f);

    if (KeysPressed.find(cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW) != KeysPressed.end() || KeysPressed.find(cocos2d::EventKeyboard::KeyCode::KEY_A) != KeysPressed.end())
        deltaPosition.x -= SpaceObjectMaxSpeed * deltaTime;
    if (KeysPressed.find(cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW) != KeysPressed.end() || KeysPressed.find(cocos2d::EventKeyboard::KeyCode::KEY_D) != KeysPressed.end())
        deltaPosition.x += SpaceObjectMaxSpeed * deltaTime;
    if (KeysPressed.find(cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW) != KeysPressed.end() || KeysPressed.find(cocos2d::EventKeyboard::KeyCode::KEY_W) != KeysPressed.end())
        deltaPosition.y += SpaceObjectMaxSpeed * deltaTime;
    if (KeysPressed.find(cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW) != KeysPressed.end() || KeysPressed.find(cocos2d::EventKeyboard::KeyCode::KEY_S) != KeysPressed.end())
        deltaPosition.y -= SpaceObjectMaxSpeed * deltaTime;

    cocos2d::Vec2 nextPosition = currentPosition + deltaPosition;
    nextPosition = ClampPosition(nextPosition);
    setPosition(nextPosition);

    if (FireTimer > 0.f)
    {
        FireTimer -= deltaTime;
    }

    if (bFireState == true && FireTimer <= 0.f)
    {
        FireTimer = FireTime;

        const cocos2d::Vec2 spaceshipPosition = getPosition();
        const cocos2d::Vec2 shotDirection = MouseLocation - spaceshipPosition;

        cocos2d::Scene* scene = getScene();
        Projectile* projectile = Projectile::create();
        scene->addChild(projectile);
        projectile->setPosition(spaceshipPosition);
        projectile->SetVelocityDirection(shotDirection);
    }
}