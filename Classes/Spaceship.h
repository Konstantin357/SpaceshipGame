#ifndef __SPACESHIP_H__
#define __SPACESHIP_H__

#include "cocos2d.h"
#include "SpaceObject.h"

class Spaceship : public SpaceObject
{
public:
    virtual bool init() override;

    CREATE_FUNC(Spaceship);
private:
    cocos2d::EventListenerKeyboard* EventListenerKeyboard = nullptr;
    cocos2d::EventListenerMouse* EventListenerMouse = nullptr;

    virtual void update(float deltaTime) override;

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    std::map<cocos2d::EventKeyboard::KeyCode, std::chrono::system_clock::time_point> KeysPressed;
    
    void onMousePressed(cocos2d::EventMouse* event);
    void onMouseReleased(cocos2d::EventMouse* event);
    void onMouseMove(cocos2d::EventMouse* event);
    cocos2d::Vec2 MouseLocation = cocos2d::Vec2::ZERO;
    bool bFireState = false;
    float FireTime = 0.5f;
    float FireTimer = 0.f;

protected:
    virtual void OnCollisionBeginVerified(SpaceObject* otherSpaceObject) override;
};
#endif // __SPACESHIP_H__