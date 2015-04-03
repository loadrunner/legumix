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

void SpritePool::init(int capacity, cocos2d::SpriteFrame* spriteFrame, cocos2d::Node* parent)
{
	if (getAvailableItemCount() > 0)
		clearPool();
	
	mSpriteFrame = spriteFrame;
	mParent = parent;
	
	initWithCapacity(capacity);
}

cocos2d::Sprite* SpritePool::onAllocatePoolItem()
{
	cocos2d::Sprite* sprite = cocos2d::Sprite::createWithSpriteFrame(mSpriteFrame);
	sprite->setVisible(false);
	sprite->pause();
	mParent->addChild(sprite);
	return sprite;
}

void SpritePool::onRecycleItem(cocos2d::Sprite* item)
{
	item->setScale(1);
	item->stopAllActions();
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
	shape->setTag(110);
	shape->setContactTestBitmask(0xFFFFFFFF);
	body->addShape(shape);
	
	wall->setPhysicsBody(body);
	
	return wall;
}

void WallPool::init(int capacity, cocos2d::Node* parent)
{
	if (getAvailableItemCount() > 0)
		clearPool();
	
	mParent = parent;
	
	initWithCapacity(capacity);
}

Wall* WallPool::onAllocatePoolItem()
{
	Wall* wall = Wall::create();
	wall->setVisible(false);
	wall->pause();
	wall->setPosition(-10, -10);
	mParent->addChild(wall);
	return wall;
}

void WallPool::onRecycleItem(Wall* item)
{
	item->setScale(1);
	item->setPosition(-10, -10);
	item->setVisible(false);
	item->pause();
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
	shape->setTag(210);
	shape->setContactTestBitmask(0xFFFFFFFF);
	body->addShape(shape);
	
	obstacle->setPhysicsBody(body);
	
	return obstacle;
}

void ObstaclePool::init(int capacity, cocos2d::Node* parent)
{
	if (getAvailableItemCount() > 0)
		clearPool();
	
	mParent = parent;
	
	initWithCapacity(capacity);
}

Obstacle* ObstaclePool::onAllocatePoolItem()
{
	Obstacle* obstacle = Obstacle::create();
	obstacle->setVisible(false);
	obstacle->pause();
	obstacle->setPosition(-10, -10);
	mParent->addChild(obstacle);
	return obstacle;
}

void ObstaclePool::onRecycleItem(Obstacle* item)
{
	item->setScale(1);
	item->setPosition(-10, -10);
	item->setVisible(false);
	item->pause();
}

