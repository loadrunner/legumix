#include "Objects.h"

MyMenuItem* MyMenuItem::create(cocos2d::Node* sprite, const cocos2d::ccMenuCallback& callback)
{
	return create(sprite, nullptr, callback);
}

MyMenuItem* MyMenuItem::create(cocos2d::Node* enabledSprite, cocos2d::Node* disabledSprite, const cocos2d::ccMenuCallback& callback)
{
	MyMenuItem* ret = new MyMenuItem();
	ret->initWithNormalSprite(enabledSprite, nullptr, disabledSprite, callback);
	ret->mEnabled = true;
	ret->autorelease();
	
	return ret;
}

void MyMenuItem::selected()
{
	MenuItem::selected();
	
	runAction(cocos2d::Sequence::create(
			cocos2d::ScaleTo::create(0.07f, 0.9f),
			cocos2d::ScaleTo::create(0.07f, 1),
			NULL
	));
}

void MyMenuItem::unselected()
{
	MenuItem::unselected();
}

void MyMenuItem::activate()
{
	MenuItemSprite::activate();
}

bool MyMenuItem::getEnabled()
{
	return mEnabled;
}

void MyMenuItem::setEnabled(bool state)
{
	if (state != mEnabled)
	{
		mEnabled = state;
		_normalImage->setVisible(mEnabled);
		if (_disabledImage)
			_disabledImage->setVisible(!mEnabled);
	}
}

Wall* Wall::create()
{
	Wall* wall = new Wall();
	wall->initWithSpriteFrameName("clouds");
	wall->autorelease();
	
	cocos2d::PhysicsBody* body = cocos2d::PhysicsBody::create();
	body->setDynamic(false);
	
	cocos2d::PhysicsMaterial material(0, 1, 0);
	
	cocos2d::PhysicsShape* shape = cocos2d::PhysicsShapeBox::create(wall->getContentSize(), material);
	shape->setTag(PHYSICS_TAG);
	shape->setContactTestBitmask(0xFFFFFFFF);
	body->addShape(shape);
	
	wall->setPhysicsBody(body);
	
	return wall;
}

Obstacle* Obstacle::create()
{
	Obstacle* obstacle = new Obstacle();
	obstacle->initWithSpriteFrameName("obs");
	obstacle->autorelease();
	
	cocos2d::PhysicsBody* body = cocos2d::PhysicsBody::create();
	body->setDynamic(false);
	
	cocos2d::PhysicsMaterial material(0, 1, 0);
	
	cocos2d::PhysicsShape* shape = cocos2d::PhysicsShapeBox::create(obstacle->getContentSize() * 0.7f, material);
	shape->setTag(PHYSICS_TAG);
	shape->setContactTestBitmask(0xFFFFFFFF);
	body->addShape(shape);
	
	obstacle->setPhysicsBody(body);
	
	return obstacle;
}

Bullet* Bullet::create()
{
	Bullet* bullet = new Bullet();
	bullet->initWithSpriteFrameName("bullet");
	bullet->autorelease();
	
	cocos2d::PhysicsBody* body = cocos2d::PhysicsBody::create();
	
	cocos2d::PhysicsMaterial material(0, 1, 0);
	
	cocos2d::PhysicsShape* shape = cocos2d::PhysicsShapeBox::create(bullet->getContentSize(), material);
	shape->setTag(PHYSICS_TAG);
	shape->setContactTestBitmask(0xFFFFFFFF);
	shape->setSensor(true);
	body->addShape(shape);
	
	bullet->setPhysicsBody(body);
	
	return bullet;
}
