#pragma once

#include "cocos2d.h"

class Hero : public cocos2d::Sprite
{
public:
	static const long PHYSICS_TAG = 1 << 0;
	static const long PHYSICS_TAG_BODY = PHYSICS_TAG | 1 << 1;
	static const long PHYSICS_TAG_HEAD = PHYSICS_TAG | 1 << 2;
	static const long PHYSICS_TAG_WING = PHYSICS_TAG | 1 << 3;
	
	bool init(const std::string& spriteFrameName);
	virtual bool init() = 0;
	virtual void reset();
	
	virtual inline int getMaxLife() { return 5; };
	inline int getLife() { return mLife; };
	inline void loseLife() { mLife--; /* TODO: check available lives */};
	inline void increaseLife() { if (mLife < getMaxLife()) mLife++; };
protected:
	int mLife;
	
};

class TomatoHero : public Hero
{
public:
	bool init() override;
	CREATE_FUNC(TomatoHero);
};
