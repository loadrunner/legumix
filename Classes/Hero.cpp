#include "Hero.h"

bool Hero::init(const std::string& spriteFrameName)
{
	if (!cocos2d::Sprite::initWithSpriteFrameName(spriteFrameName))
		return false;
	
	reset();
	
	return true;
}

void Hero::reset()
{
	mLife = getMaxLife();
}

bool TomatoHero::init()
{
	if (!Hero::init("plane"))
		return false;
	
	cocos2d::PhysicsBody* body = cocos2d::PhysicsBody::create();
	body->setVelocity(cocos2d::Vec2::ZERO);
	body->setRotationEnable(false);
	body->setLinearDamping(7.5f);
	
	cocos2d::PhysicsShape* bodyy = cocos2d::PhysicsShapeBox::create(cocos2d::Size(4, 13), cocos2d::PhysicsMaterial(1, 1, 0), cocos2d::Vec2(0, -2));
	bodyy->setTag(PHYSICS_TAG_BODY);
	bodyy->setSensor(true);
	body->addShape(bodyy);
	
	cocos2d::PhysicsShape* bot = cocos2d::PhysicsShapeBox::create(cocos2d::Size(3, 2), cocos2d::PhysicsMaterial(1, 1, 0), cocos2d::Vec2(0, 8));
	bot->setTag(PHYSICS_TAG_HEAD);
	bot->setSensor(true);
	body->addShape(bot);
	
	cocos2d::PhysicsShape* leftWing = cocos2d::PhysicsShapeBox::create(cocos2d::Size(4, 2), cocos2d::PhysicsMaterial(1, 1, 0), cocos2d::Vec2(-6, -1));
	leftWing->setTag(PHYSICS_TAG_WING);
	leftWing->setSensor(true);
	body->addShape(leftWing);
	
	cocos2d::PhysicsShape* rightWing = cocos2d::PhysicsShapeBox::create(cocos2d::Size(4, 2), cocos2d::PhysicsMaterial(1, 1, 0), cocos2d::Vec2(6, -1));
	rightWing->setTag(PHYSICS_TAG_WING);
	rightWing->setSensor(true);
	body->addShape(rightWing);
	
	body->setContactTestBitmask(0xFFFFFFFF);
	setPhysicsBody(body);
	
	return true;
}
