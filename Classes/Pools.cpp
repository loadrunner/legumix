#include "Pools.h"

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
	item->setVisible(false);
	item->pause();
}

void ObjectPool::init(int capacity, Object* sample, cocos2d::Node* parent)
{
	if (getAvailableItemCount() > 0)
		clearPool();
	
	mParent = parent;
	mSample = sample;
	mSample->setVisible(false);
	mSample->pause();
	mSample->setPosition(-10, -10);
	mParent->addChild(mSample);
	//TODO: enable retain, and implement release
//	mSample->retain();
	
	initWithCapacity(capacity);
}

Object* ObjectPool::onAllocatePoolItem()
{
	Object* item = mSample->clone();
	item->setVisible(false);
	item->pause();
	item->setPosition(-10, -10);
//	item->getPhysicsBody()->setDynamic(false);
	mParent->addChild(item);
	return item;
}

void ObjectPool::onObtainItem(Object* item)
{
	item->setVisible(true);
	item->resume();
}

void ObjectPool::onRecycleItem(Object* item)
{
	item->stopAllActions();
	item->setPosition(-10, -10);
	item->setScale(1);
	item->setVisible(false);
	item->pause();
	item->reset();
}

void BulletPool::init(int capacity, cocos2d::Node* parent)
{
	if (getAvailableItemCount() > 0)
		clearPool();
	
	mParent = parent;
	
	initWithCapacity(capacity);
}

Bullet* BulletPool::onAllocatePoolItem()
{
	Bullet* bullet = Bullet::create();
	bullet->setVisible(false);
	bullet->pause();
	bullet->setPosition(-10, -10);
	bullet->getPhysicsBody()->setDynamic(false);
	mParent->addChild(bullet);
	return bullet;
}

void BulletPool::onObtainItem(Bullet* item)
{
	item->setVisible(true);
	item->resume();
	item->getPhysicsBody()->setDynamic(true);
}

void BulletPool::onRecycleItem(Bullet* item)
{
	item->stopAllActions();
	item->setRotation(0);
	item->setPosition(-10, -10);
	item->setScale(1);
	item->setVisible(false);
	item->pause();
	item->getPhysicsBody()->setDynamic(false);
}
