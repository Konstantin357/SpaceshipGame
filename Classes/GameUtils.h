#ifndef __GAMEUTILS_H__
#define __GAMEUTILS_H__

#include "cocos2d.h"

class GameUtils
{
public:
	template <typename T>
	static void GoToGameScene(cocos2d::Ref* sender);

	static void ExitGame(cocos2d::Ref* sender);

	static float RandF(float min, float max);
	static int RandI(int min, int max);
};
#endif // __GAMEUTILS_H__