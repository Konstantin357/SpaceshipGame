#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__

#include "cocos2d.h"

class GameOverScene : public cocos2d::Scene
{
public:
    virtual bool init() override;
    
    CREATE_FUNC(GameOverScene);

private:
    void update(float deltaTime);

    float SceneShowTime = 5.f;
    float SceneShowTimer = 0.f;
};
#endif // __GAMEOVER_SCENE_H__
