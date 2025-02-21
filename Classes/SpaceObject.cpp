#include "SpaceObject.h"

bool SpaceObject::init()
{
    if (!cocos2d::Node::init())
    {
        return false;
    }

    cocos2d::Director* director = cocos2d::Director::getInstance();
    if (director != nullptr)
    {
        Sprite = cocos2d::Sprite::create(SpriteName);
        addChild(Sprite);
        Sprite->setPosition(cocos2d::Vec2(0, 0));

        const cocos2d::Size& spriteSize = Sprite->getContentSize();
        if (SpaceObjectPhysicsBodyType == PhysicsBodyType::Circle)
        {
            PhysicsBody = cocos2d::PhysicsBody::createCircle(spriteSize.width / 2.f);
        }
        else
        {
            PhysicsBody = cocos2d::PhysicsBody::createBox(spriteSize);
        }
        setPhysicsBody(PhysicsBody);
        PhysicsBody->setGravityEnable(false);
        PhysicsBody->setDynamic(false);
        PhysicsBody->setRotationEnable(false);
        if (cocos2d::PhysicsShape* physicsShape = PhysicsBody->getShape(0))
        {
            physicsShape->setSensor(true);
        }

        cocos2d::Size visibleSize = director->getVisibleSize();
        cocos2d::Vec2 origin = director->getVisibleOrigin();

        cocos2d::Vec2 screenMargins = cocos2d::Vec2(0.f, 0.f);
        if (SpaceObjectScreenBoundsType == ScreenBoundsType::Inner)
            screenMargins = cocos2d::Vec2(spriteSize.width / 2.f, spriteSize.height / 2.f);
        else if (SpaceObjectScreenBoundsType == ScreenBoundsType::Middle)
            screenMargins = cocos2d::Vec2(0.f, 0.f);
        else if (SpaceObjectScreenBoundsType == ScreenBoundsType::Outer)
            screenMargins = cocos2d::Vec2(-spriteSize.width / 2.f, -spriteSize.height / 2.f);
        else if (SpaceObjectScreenBoundsType == ScreenBoundsType::OuterDouble)
            screenMargins = cocos2d::Vec2(-spriteSize.width, -spriteSize.height);

        BoundsMin = cocos2d::Vec2(screenMargins.x + origin.x, screenMargins.y + origin.y);
        BoundsMax = cocos2d::Vec2(visibleSize.width - screenMargins.x + origin.x, visibleSize.height - screenMargins.y + origin.y);

        ListenerPhysicsContact = cocos2d::EventListenerPhysicsContact::create();
        ListenerPhysicsContact->onContactBegin = CC_CALLBACK_1(SpaceObject::OnPhysicsContactBegin, this);
        ListenerPhysicsContact->onContactSeparate = CC_CALLBACK_1(SpaceObject::OnPhysicsContactEnd, this);
        if (cocos2d::EventDispatcher* eventDispatcher = getEventDispatcher())
        {
            eventDispatcher->addEventListenerWithSceneGraphPriority(ListenerPhysicsContact, this);
        }
    }

    scheduleUpdate();

    return true;
}

void SpaceObject::update(float deltaTime)
{
}

void SpaceObject::SetVelocityDirection(cocos2d::Vec2 inVelocityDirection)
{
    const float velocityDirectionAbs = std::sqrtf(inVelocityDirection.x * inVelocityDirection.x + inVelocityDirection.y * inVelocityDirection.y);
    if (velocityDirectionAbs != 0.f)
    {
        VelocityDirection = inVelocityDirection / velocityDirectionAbs;
    }
    else
    {
        VelocityDirection = cocos2d::Vec2(1.f, 0.f);
    }
}

cocos2d::Vec2 SpaceObject::GetVelocityDirection()
{
    return VelocityDirection;
}

void SpaceObject::OnScreenEdgeBumped()
{
    bScreenEdgeBumped = true;
}

cocos2d::Vec2 SpaceObject::ClampPosition(const cocos2d::Vec2 inPosition)
{
    cocos2d::Vec2 returnPosition = inPosition;
    if (returnPosition.x < BoundsMin.x)
    {
        returnPosition.x = BoundsMin.x;
        LastScreenBumpType = ScreenEdgeType::Left;
    }
    else if (returnPosition.x > BoundsMax.x)
    {
        returnPosition.x = BoundsMax.x;
        LastScreenBumpType = ScreenEdgeType::Right;
    }
    if (returnPosition.y < BoundsMin.y)
    {
        returnPosition.y = BoundsMin.y;
        LastScreenBumpType = ScreenEdgeType::Up;
    }
    else if (returnPosition.y > BoundsMax.y)
    {
        returnPosition.y = BoundsMax.y;
        LastScreenBumpType = ScreenEdgeType::Down;
    }

    if (returnPosition != inPosition)
        OnScreenEdgeBumped();

    return returnPosition;
}

bool SpaceObject::OnPhysicsContactBegin(cocos2d::PhysicsContact& contact)
{
    SpaceObject* otherSpaceObject = FindOtherSpaceObjectOnPhysicsContact(contact);
    if (otherSpaceObject != nullptr)
    {
        OnCollisionBeginVerified(otherSpaceObject);
    }

    return otherSpaceObject != nullptr;
}

bool SpaceObject::OnPhysicsContactEnd(cocos2d::PhysicsContact& contact)
{
    SpaceObject* otherSpaceObject = FindOtherSpaceObjectOnPhysicsContact(contact);
    if (otherSpaceObject != nullptr)
    {
        OnCollisionEndVerified(otherSpaceObject);
    }

    return otherSpaceObject != nullptr;
}

SpaceObject* SpaceObject::FindOtherSpaceObjectOnPhysicsContact(cocos2d::PhysicsContact& contact)
{
    const cocos2d::PhysicsShape* shapeA = contact.getShapeA();
    const cocos2d::PhysicsShape* shapeB = contact.getShapeB();
    const cocos2d::PhysicsBody* bodyA = shapeA != nullptr ? shapeA->getBody() : nullptr;
    const cocos2d::PhysicsBody* bodyB = shapeB != nullptr ? shapeB->getBody() : nullptr;

    if (bodyA != nullptr && bodyB != nullptr && (bodyA == PhysicsBody || bodyB == PhysicsBody))
    {
        if (cocos2d::Node* otherNode = bodyA == PhysicsBody ? bodyB->getOwner() : bodyA->getOwner())
        {
            if (SpaceObject* otherSpaceObject = dynamic_cast<SpaceObject*>(otherNode))
            {
                return otherSpaceObject;
            }
        }
    }
    return nullptr;
}

void SpaceObject::OnCollisionBeginVerified(SpaceObject* otherSpaceObject)
{
}
void SpaceObject::OnCollisionEndVerified(SpaceObject* otherSpaceObject)
{
}