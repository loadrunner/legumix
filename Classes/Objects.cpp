#include "Objects.h"
#include "GameScene.h"
#include "audio/include/SimpleAudioEngine.h"

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

bool Bullet::init()
{
	if (!cocos2d::Sprite::initWithSpriteFrameName("bullet"))
		return false;
	
	cocos2d::PhysicsBody* body = cocos2d::PhysicsBody::create();
	
	cocos2d::PhysicsMaterial material(0, 1, 0);
	
	cocos2d::PhysicsShape* shape = cocos2d::PhysicsShapeBox::create(getContentSize(), material);
	shape->setTag(PHYSICS_TAG);
	shape->setContactTestBitmask(0xFFFFFFFF);
	shape->setSensor(true);
	body->addShape(shape);
	
	setPhysicsBody(body);
	
	return true;
}

bool Object::init(const std::string& spriteFrameName)
{
	if (!cocos2d::Sprite::initWithSpriteFrameName(spriteFrameName))
		return false;
	
	return true;
}

bool Obstacle::init(const std::string& spriteFrameName)
{
	return Object::init(spriteFrameName);
}

bool Haystack::init()
{
	if (!Obstacle::init("haystack"))
		return false;
	
	cocos2d::PhysicsBody* body = cocos2d::PhysicsBody::create();
	body->setDynamic(false);
	
	cocos2d::PhysicsMaterial material(0, 1, 0);
	
	cocos2d::PhysicsShape* shape = cocos2d::PhysicsShapeBox::create(
			cocos2d::Size(getContentSize().width, getContentSize().height * 0.7f), material);
	shape->setTag(PHYSICS_TAG);
	shape->setContactTestBitmask(0xFFFFFFFF);
	body->addShape(shape);
	
	setPhysicsBody(body);
	
	return true;
}

bool Collectable::init(const std::string& spriteFrameName)
{
	return Object::init(spriteFrameName);
}

bool Coin::init()
{
	if (!Collectable::init("coin"))
		return false;
	
	cocos2d::PhysicsBody* body = cocos2d::PhysicsBody::create();
	
	cocos2d::PhysicsMaterial material(0, 1, 0);
	
	cocos2d::PhysicsShape* shape = cocos2d::PhysicsShapeBox::create(getContentSize(), material);
	shape->setTag(PHYSICS_TAG);
	shape->setContactTestBitmask(0xFFFFFFFF);
	shape->setSensor(true);
	body->addShape(shape);
	body->setDynamic(false);
	
	setPhysicsBody(body);
	
	return true;
}

bool Tomato::init()
{
	if (!Collectable::init("tomato_small"))
		return false;
	
	cocos2d::PhysicsBody* body = cocos2d::PhysicsBody::create();
	
	cocos2d::PhysicsMaterial material(0, 1, 0);
	
	cocos2d::PhysicsShape* shape = cocos2d::PhysicsShapeBox::create(getContentSize(), material);
	shape->setTag(PHYSICS_TAG);
	shape->setContactTestBitmask(0xFFFFFFFF);
	shape->setSensor(true);
	body->addShape(shape);
	body->setDynamic(false);
	
	setPhysicsBody(body);
	
	return true;
}

bool Broccoli::init()
{
	if (!Collectable::init("broccoli_small"))
		return false;
	
	cocos2d::PhysicsBody* body = cocos2d::PhysicsBody::create();
	
	cocos2d::PhysicsMaterial material(0, 1, 0);
	
	cocos2d::PhysicsShape* shape = cocos2d::PhysicsShapeBox::create(getContentSize(), material);
	shape->setTag(PHYSICS_TAG);
	shape->setContactTestBitmask(0xFFFFFFFF);
	shape->setSensor(true);
	body->addShape(shape);
	body->setDynamic(false);
	
	setPhysicsBody(body);
	
	return true;
}

bool Enemy::init(const std::string& spriteFrameName)
{
	return Object::init(spriteFrameName);
}

bool Tower::init()
{
	if (!Enemy::init("tower"))
		return false;
	
	cocos2d::PhysicsBody* body = cocos2d::PhysicsBody::create();
	body->setDynamic(false);
	
	cocos2d::PhysicsMaterial material(0, 1, 0);
	
	cocos2d::PhysicsShape* shape = cocos2d::PhysicsShapeBox::create(
			cocos2d::Size(getContentSize().width, getContentSize().height * 0.7f), material);
	shape->setTag(PHYSICS_TAG);
	shape->setContactTestBitmask(0xFFFFFFFF);
	body->addShape(shape);
	
	setPhysicsBody(body);
	
	mLife = MAX_LIVES;
	
	return true;
}

void Tower::update(cocos2d::Vec2 heroPos)
{
	float dist = getPosition().distanceSquared(heroPos);
	float ang = atan2(heroPos.y - getPositionY(), heroPos.x - getPositionX()) * 180 / M_PI;
	
	if (dist < 2500)
	{
		Bullet* bullet = GameScene::mBulletPool.obtainPoolItem();
		bullet->setRotation(90 - ang);
		bullet->setPosition(cocos2d::Vec2(getPosition()));
		bullet->runAction(cocos2d::Sequence::create(
				cocos2d::MoveTo::create(0.2f, heroPos),
				cocos2d::CallFuncN::create([this](cocos2d::Node* node)
				{
					GameScene::mBulletPool.recyclePoolItem((Bullet*) node);
				}),
				nullptr));
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("shoot.mp3");
	}
}
