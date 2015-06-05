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
	item->setPosition(-10, -10);
	item->setScale(1);
	item->setVisible(false);
	item->pause();
	item->getPhysicsBody()->setDynamic(false);
}

void CoinPool::init(int capacity, cocos2d::Node* parent)
{
	if (getAvailableItemCount() > 0)
		clearPool();
	
	mParent = parent;
	
	initWithCapacity(capacity);
}

Coin* CoinPool::onAllocatePoolItem()
{
	Coin* coin = Coin::create();
	coin->setVisible(false);
	coin->pause();
	coin->setPosition(-10, -10);
//	coin->getPhysicsBody()->setDynamic(false);
	mParent->addChild(coin);
	return coin;
}

void CoinPool::onObtainItem(Coin* item)
{
	item->setVisible(true);
	item->resume();
}

void CoinPool::onRecycleItem(Coin* item)
{
	item->stopAllActions();
	item->setPosition(-10, -10);
	item->setScale(1);
	item->setVisible(false);
	item->pause();
}

