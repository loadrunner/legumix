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

class Bullet : public cocos2d::Sprite
{
public:
	static const unsigned long PHYSICS_TAG = 1 << 8;
	
	bool init();
	CREATE_FUNC(Bullet);
};

class Object : public cocos2d::Sprite
{
public:
	static const unsigned long PHYSICS_TAG = 1 << 12;
	
	virtual Object* clone() const = 0;
	virtual bool init(const std::string& spriteFrameName);
	virtual bool init() = 0;
	virtual bool canBeShotBy(const long tag) = 0;
};

class Obstacle : public Object
{
public:
	static const unsigned long PHYSICS_TAG = Object::PHYSICS_TAG | 1 << 13;
	
	bool init(const std::string& spriteFrameName) override;
};

class Haystack : public Obstacle
{
public:
	static const unsigned long PHYSICS_TAG = Obstacle::PHYSICS_TAG | 1 << 14;
	
	virtual bool init() override;
	CREATE_FUNC(Haystack);
	inline virtual Haystack* clone() const { return Haystack::create(); }
	
	inline bool canBeShotBy(const long tag) override { return tag & Bullet::PHYSICS_TAG; };
};

class Collectable : public Object
{
public:
	static const unsigned long PHYSICS_TAG = 1 << 20;
	
	virtual bool init(const std::string& spriteFrameName) override;
};

class Coin : public Collectable
{
public:
	static const unsigned long PHYSICS_TAG = Collectable::PHYSICS_TAG | 1 << 21;
	
	virtual bool init() override;
	CREATE_FUNC(Coin);
	inline virtual Coin* clone() const { return Coin::create(); }
	
	inline bool canBeShotBy(const long tag) override { return false; };
};

class Tomato : public Collectable
{
public:
	static const unsigned long PHYSICS_TAG = Collectable::PHYSICS_TAG | 1 << 22;
	
	virtual bool init() override;
	CREATE_FUNC(Tomato);
	inline virtual Tomato* clone() const { return Tomato::create(); }
	
	inline bool canBeShotBy(const long tag) override { return false; };
};

class Broccoli : public Collectable
{
public:
	static const unsigned long PHYSICS_TAG = Collectable::PHYSICS_TAG | 1 << 23;
	
	virtual bool init() override;
	CREATE_FUNC(Broccoli);
	inline virtual Broccoli* clone() const { return Broccoli::create(); }
	
	inline bool canBeShotBy(const long tag) override { return false; };
};
