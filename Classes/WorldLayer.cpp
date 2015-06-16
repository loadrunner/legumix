#include "WorldLayer.h"

bool WorldLayer::init()
{
	if (!cocos2d::LayerColor::initWithColor(cocos2d::Color4B(255, 255, 255, 255)))
		return false;
	
//	mGameArea = cocos2d::ClippingRectangleNode::create(cocos2d::Rect(0, 0, mScreenSize.width, 120));
	ignoreAnchorPointForPosition(false);
	
	mScrollContainer = cocos2d::Layer::create();
	addChild(mScrollContainer);
	
	mBg1 = cocos2d::LayerColor::create(cocos2d::Color4B(89, 148, 54, 255));
	mBg1->ignoreAnchorPointForPosition(false);
	mBg1->setPosition(cocos2d::Vec2(getContentSize().width/2, getContentSize().height/2));
	mScrollContainer->addChild(mBg1);
	
	cocos2d::Sprite* decor = cocos2d::Sprite::createWithSpriteFrameName("line");
	decor->setAnchorPoint(cocos2d::Vec2(0.5f, 1));
	decor->setPosition(cocos2d::Vec2(mBg1->getContentSize().width/2, mBg1->getContentSize().height));
	mBg1->addChild(decor);
	
	decor = cocos2d::Sprite::createWithSpriteFrameName("side-clouds");
	decor->setPosition(cocos2d::Vec2(0, mBg1->getContentSize().height * 0.25f));
	mBg1->addChild(decor);
	
	decor = cocos2d::Sprite::createWithSpriteFrameName("side-clouds");
	decor->setPosition(cocos2d::Vec2(0, mBg1->getContentSize().height * 0.75f));
	mBg1->addChild(decor);
	
	decor = cocos2d::Sprite::createWithSpriteFrameName("side-clouds");
	decor->setPosition(cocos2d::Vec2(mBg1->getContentSize().width, mBg1->getContentSize().height * 0.25f));
	mBg1->addChild(decor);
	
	decor = cocos2d::Sprite::createWithSpriteFrameName("side-clouds");
	decor->setPosition(cocos2d::Vec2(mBg1->getContentSize().width, mBg1->getContentSize().height * 0.75f));
	mBg1->addChild(decor);
	
	decor = cocos2d::Sprite::createWithSpriteFrameName("tree");
	decor->setPosition(cocos2d::Vec2(rand() % (int) mBg1->getContentSize().width, rand() % (int) mBg1->getContentSize().height));
	mBg1->addChild(decor);
	
	decor = cocos2d::Sprite::createWithSpriteFrameName("tree");
	decor->setPosition(cocos2d::Vec2(rand() % (int) mBg1->getContentSize().width, rand() % (int) mBg1->getContentSize().height));
	mBg1->addChild(decor);
	
	decor = cocos2d::Sprite::createWithSpriteFrameName("tree");
	decor->setPosition(cocos2d::Vec2(rand() % (int) mBg1->getContentSize().width, rand() % (int) mBg1->getContentSize().height));
	mBg1->addChild(decor);
	
	decor = cocos2d::Sprite::createWithSpriteFrameName("random-house");
	decor->setPosition(cocos2d::Vec2(rand() % (int) mBg1->getContentSize().width, rand() % (int) mBg1->getContentSize().height));
	mBg1->addChild(decor);
	
	decor = cocos2d::Sprite::createWithSpriteFrameName("random-house-2");
	decor->setPosition(cocos2d::Vec2(rand() % (int) mBg1->getContentSize().width, rand() % (int) mBg1->getContentSize().height));
	mBg1->addChild(decor);
	
	cocos2d::PhysicsMaterial material(0, 1, 0);
	cocos2d::PhysicsBody* body = cocos2d::PhysicsBody::create();
	cocos2d::PhysicsShape* edge = cocos2d::PhysicsShapeEdgeSegment::create(cocos2d::Vec2(-mBg1->getContentSize().width/2, -mBg1->getContentSize().height/2), cocos2d::Vec2(-mBg1->getContentSize().width/2, mBg1->getContentSize().height/2), material);
	edge->setTag(PHYSICS_TAG_EDGE_LEFT);
	body->addShape(edge, true);
	
	edge = cocos2d::PhysicsShapeEdgeSegment::create(cocos2d::Vec2(mBg1->getContentSize().width/2, mBg1->getContentSize().height/2), cocos2d::Vec2(mBg1->getContentSize().width/2, -mBg1->getContentSize().height/2), material);
	edge->setTag(PHYSICS_TAG_EDGE_RIGHT);
	body->addShape(edge, true);
	
	body->setDynamic(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	mBg1->setPhysicsBody(body);
	
	mBg2 = cocos2d::LayerColor::create(cocos2d::Color4B(89, 148, 54, 255));
	mBg2->ignoreAnchorPointForPosition(false);
	mBg2->setPosition(mBg1->getPosition() + cocos2d::Vec2(0, mBg1->getContentSize().height));
	mScrollContainer->addChild(mBg2);
	
	decor = cocos2d::Sprite::createWithSpriteFrameName("line");
	decor->setAnchorPoint(cocos2d::Vec2(0.5f, 1));
	decor->setPosition(cocos2d::Vec2(mBg2->getContentSize().width/2, mBg2->getContentSize().height));
	mBg2->addChild(decor);
	
	decor = cocos2d::Sprite::createWithSpriteFrameName("side-clouds");
	decor->setPosition(cocos2d::Vec2(0, mBg2->getContentSize().height * 0.25f));
	mBg2->addChild(decor);
	
	decor = cocos2d::Sprite::createWithSpriteFrameName("side-clouds");
	decor->setPosition(cocos2d::Vec2(0, mBg2->getContentSize().height * 0.75f));
	mBg2->addChild(decor);
	
	decor = cocos2d::Sprite::createWithSpriteFrameName("side-clouds");
	decor->setPosition(cocos2d::Vec2(mBg2->getContentSize().width, mBg2->getContentSize().height * 0.25f));
	mBg2->addChild(decor);
	
	decor = cocos2d::Sprite::createWithSpriteFrameName("side-clouds");
	decor->setPosition(cocos2d::Vec2(mBg2->getContentSize().width, mBg2->getContentSize().height * 0.75f));
	mBg2->addChild(decor);
	
	decor = cocos2d::Sprite::createWithSpriteFrameName("tree");
	decor->setPosition(cocos2d::Vec2(rand() % (int) mBg2->getContentSize().width, rand() % (int) mBg2->getContentSize().height));
	mBg2->addChild(decor);
	
	decor = cocos2d::Sprite::createWithSpriteFrameName("tree");
	decor->setPosition(cocos2d::Vec2(rand() % (int) mBg2->getContentSize().width, rand() % (int) mBg2->getContentSize().height));
	mBg2->addChild(decor);
	
	decor = cocos2d::Sprite::createWithSpriteFrameName("tree");
	decor->setPosition(cocos2d::Vec2(rand() % (int) mBg2->getContentSize().width, rand() % (int) mBg2->getContentSize().height));
	mBg2->addChild(decor);
	
	decor = cocos2d::Sprite::createWithSpriteFrameName("random-house");
	decor->setPosition(cocos2d::Vec2(rand() % (int) mBg2->getContentSize().width, rand() % (int) mBg2->getContentSize().height));
	mBg2->addChild(decor);
	
	decor = cocos2d::Sprite::createWithSpriteFrameName("random-house-2");
	decor->setPosition(cocos2d::Vec2(rand() % (int) mBg2->getContentSize().width, rand() % (int) mBg2->getContentSize().height));
	mBg2->addChild(decor);
	
	body = cocos2d::PhysicsBody::create();
	edge = cocos2d::PhysicsShapeEdgeSegment::create(cocos2d::Vec2(-mBg2->getContentSize().width/2, -mBg2->getContentSize().height/2), cocos2d::Vec2(-mBg2->getContentSize().width/2, mBg2->getContentSize().height/2), material);
	edge->setTag(PHYSICS_TAG_EDGE_LEFT);
	body->addShape(edge, true);
	
	edge = cocos2d::PhysicsShapeEdgeSegment::create(cocos2d::Vec2(mBg2->getContentSize().width/2, mBg2->getContentSize().height/2), cocos2d::Vec2(mBg2->getContentSize().width/2, -mBg2->getContentSize().height/2), material);
	edge->setTag(PHYSICS_TAG_EDGE_RIGHT);
	body->addShape(edge, true);
	
	body->setDynamic(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	mBg2->setPhysicsBody(body);
	
	return true;
}

void WorldLayer::update(float dt)
{
	mScrollContainer->setPositionY(mScrollContainer->getPositionY() - dt * 60);
	
	if (mBg2->getPositionY() - mBg2->getContentSize().height/2 <= -mScrollContainer->getPositionY())
	{
		mBg1->setPositionY(mBg2->getPositionY() + mBg2->getContentSize().height);
		cocos2d::Node* tmp = mBg1;
		mBg1 = mBg2;
		mBg2 = tmp;
	}
}
