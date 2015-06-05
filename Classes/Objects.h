#pragma once

#include "cocos2d.h"

class MyMenuItem : public cocos2d::MenuItemSprite
{
public:
	static MyMenuItem* create(cocos2d::Node* sprite, const cocos2d::ccMenuCallback& callback);
	static MyMenuItem* create(cocos2d::Node* enabledSprite, cocos2d::Node* disabledSprite, const cocos2d::ccMenuCallback& callback);
	
	void selected() override;
	void unselected() override;
	void activate() override;
	
	bool getEnabled();
	void setEnabled(bool state);
private:
	bool mEnabled;
};

class Obstacle : public cocos2d::Sprite
{
public:
	static Obstacle* create();
	static const int PHYSICS_TAG = 210;
};

class Bullet : public cocos2d::Sprite
{
public:
	static Bullet* create();
	static const int PHYSICS_TAG = 400;
};

class Collectable : public cocos2d::Sprite
{
public:
	
};

class Coin : public Collectable
{
public:
	static Coin* create();
	static const int PHYSICS_TAG = 500;
};

