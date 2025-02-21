#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "cocos2d.h"
#include "SpaceObject.h"

class Projectile : public SpaceObject
{
public:
    virtual bool init() override;

    CREATE_FUNC(Projectile);
protected:
    virtual void update(float deltaTime) override;
};
#endif // __PROJECTILE_H__