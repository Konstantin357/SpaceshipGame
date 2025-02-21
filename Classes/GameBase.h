#ifndef __GAMEBASE_H__
#define __GAMEBASE_H__

#include "cocos2d.h"

enum class ScreenBoundsType
{
    Inner,
    Middle,
    Outer,
    OuterDouble
};

enum class PhysicsBodyType
{
    Circle,
    Box
};

enum class ScreenEdgeType
{
    None = 0,
    Left = 1,
    Right = 2,
    Up = 3,
    Down = 4
};

struct DangerZoneInfo
{
    DangerZoneInfo()
    {
        Position = cocos2d::Vec2::ZERO;
        Radius = 0.f;
        Time = 0.f;
    }

    DangerZoneInfo(cocos2d::Vec2 inPosition, float inRadius, float inTime)
    {
        Position = inPosition;
        Radius = inRadius;
        Time = inTime;
    }

    cocos2d::Vec2 Position;
    float Radius;
    float Time;
};

#define SPACESHIP_COLLISION_CHANNEL 0b0001
#define PROJECTILE_COLLISION_CHANNEL 0b0010
#define ASTEROID_COLLISION_CHANNEL 0b0100
#define DANGER_ZONE_COLLISION_CHANNEL 0b1000

#define STARTING_ASTEROID_SPLITS 2

#define LAST_SCORE_STRING "LastScore"
#define BEST_SCORE_STRING "BestScore"
#define GAME_TIME_STRING "GameTime"

#endif // __GAMEBASE_H__