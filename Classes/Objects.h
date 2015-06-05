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

class Wall : public cocos2d::Sprite
{
public:
	static Wall* create();
	static const int PHYSICS_TAG = 110;
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

