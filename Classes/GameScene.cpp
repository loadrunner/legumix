#include "GameScene.h"

GameScene::GameScene() : mTempPoint(nullptr), mTempWallShape1(nullptr), mTempWallShape2(nullptr)
{
	cocos2d::log("game scene constructed");
}

GameScene::~GameScene()
{
	cocos2d::log("game scene destructed");
	
	if (mTempPoint != nullptr)
		delete mTempPoint;
}

cocos2d::Scene* GameScene::createScene()
{
	auto scene = cocos2d::Scene::createWithPhysics();
	
	auto layer = new GameScene();
	scene->addChild(layer);
	layer->init();
	layer->autorelease();
	
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!cocos2d::LayerColor::initWithColor(cocos2d::Color4B(44, 44, 44, 255)))
	{
		return false;
	}
	
	cocos2d::UserDefault* ud = cocos2d::UserDefault::getInstance();
	
	mIsGameServicesAvailable = false;
	
	mScreenSize = cocos2d::Director::getInstance()->getWinSize();
	mVisibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	mOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
	
	cocos2d::log("size %f, %f", mScreenSize.width, mScreenSize.height);
	cocos2d::log("visible size %f, %f", mVisibleSize.width, mVisibleSize.height);
	cocos2d::log("offset %f, %f", mOrigin.x, mOrigin.y);
	
	mGameLayer = cocos2d::LayerColor::create(cocos2d::Color4B(15, 50, 15, 200));
	mGameLayer->ignoreAnchorPointForPosition(false);
	mGameLayer->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	mGameLayer->setContentSize(cocos2d::Size(90, 120));
	mGameLayer->setPosition(cocos2d::Vec2(mScreenSize.width/2, 5 + mGameLayer->getContentSize().height/2));
	this->addChild(mGameLayer, 100);
	
	mUILayer = cocos2d::Layer::create();
	this->addChild(mUILayer, 200);
	
	initPools();
	
	getScene()->getPhysicsWorld()->setGravity(cocos2d::Vec2::ZERO);
	getScene()->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
	
	cocos2d::PhysicsMaterial material(0, 1, 0);
	cocos2d::PhysicsBody* body = cocos2d::PhysicsBody::create();
	body->addShape(cocos2d::PhysicsShapeEdgeSegment::create(cocos2d::Vec2(-mGameLayer->getContentSize().width/2, -mGameLayer->getContentSize().height/2), cocos2d::Vec2(-mGameLayer->getContentSize().width/2, mGameLayer->getContentSize().height/2), material), true);
	body->addShape(cocos2d::PhysicsShapeEdgeSegment::create(cocos2d::Vec2(-mGameLayer->getContentSize().width/2, -mGameLayer->getContentSize().height/2), cocos2d::Vec2(mGameLayer->getContentSize().width/2, -mGameLayer->getContentSize().height/2), material), true);
	body->addShape(cocos2d::PhysicsShapeEdgeSegment::create(cocos2d::Vec2(mGameLayer->getContentSize().width/2, mGameLayer->getContentSize().height/2), cocos2d::Vec2(mGameLayer->getContentSize().width/2, -mGameLayer->getContentSize().height/2), material), true);
	body->addShape(cocos2d::PhysicsShapeEdgeSegment::create(cocos2d::Vec2(-mGameLayer->getContentSize().width/2, mGameLayer->getContentSize().height/2), cocos2d::Vec2(mGameLayer->getContentSize().width/2, mGameLayer->getContentSize().height/2), material), true);
	
	body->setDynamic(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	mGameLayer->setPhysicsBody(body);
	
	mTempPoint = nullptr;
	mTempWallShape1 = nullptr;
	mTempWallShape2 = nullptr;
	
	mTempWallLayer = cocos2d::LayerColor::create(cocos2d::Color4B(95, 10, 15, 200));
	mTempWallLayer->ignoreAnchorPointForPosition(false);
	mTempWallLayer->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
	mTempWallLayer->setContentSize(mGameLayer->getContentSize());
	mTempWallLayer->setPosition(cocos2d::Vec2(mGameLayer->getContentSize().width/2, mGameLayer->getContentSize().height/2));
	mGameLayer->addChild(mTempWallLayer);
	
	mBox = cocos2d::LayerColor::create(cocos2d::Color4B::BLUE);
	mBox->setContentSize(cocos2d::Size(15, 15));
	mBox->ignoreAnchorPointForPosition(false);
	mBox->setPosition(cocos2d::Vec2(mGameLayer->getContentSize().width/2, mGameLayer->getContentSize().height/2));
	mGameLayer->addChild(mBox);
	
	body = cocos2d::PhysicsBody::createBox(mBox->getContentSize(), cocos2d::PhysicsMaterial(1, 1, 0), cocos2d::Vec2::ZERO);
	body->setVelocity(cocos2d::Vec2::ZERO);
	body->setRotationEnable(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	mBox->setPhysicsBody(body);
	
	// Register Touch Event
	auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	auto listener2 = cocos2d::EventListenerKeyboard::create();
	listener2->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	listener2->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener2, this);
	
	auto listener3 = cocos2d::EventListenerCustom::create(EVENT_COME_TO_FOREGROUND, CC_CALLBACK_0(GameScene::onComeToForeground, this));
	dispatcher->addEventListenerWithSceneGraphPriority(listener3, this);
	
	auto listener4 = cocos2d::EventListenerCustom::create(EVENT_COME_TO_BACKGROUND, CC_CALLBACK_0(GameScene::onComeToBackground, this));
	dispatcher->addEventListenerWithSceneGraphPriority(listener4, this);
	
	auto listener5 = cocos2d::EventListenerPhysicsContact::create();
	listener5->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener5, this);
	
	this->schedule(schedule_selector(GameScene::update), 0.02f);
	this->schedule(schedule_selector(GameScene::updateSlow), 0.5f);
	
	if (AppDelegate::pluginAnalytics != nullptr)
	{
		std::vector<cocos2d::plugin::PluginParam*> params;
		params.push_back(new cocos2d::plugin::PluginParam("game"));
		AppDelegate::pluginAnalytics->callFuncWithParam("logPageView", params);
	}
	
	return true;
}

void GameScene::initPools()
{

}

void GameScene::setParent(Node* child)
{
	Node::setParent(child);
	
}

void GameScene::update(float dt)
{
//	timeFromLastMoney += dt;
	
//	if (timeFromLastMoney >= 0.5f)
//	{
//		timeFromLastMoney = 0;
//		
//
//	}
}

void GameScene::updateSlow(float dt)
{
	if (AppDelegate::pluginGameServices->isSignedIn() != mIsGameServicesAvailable)
	{
		mIsGameServicesAvailable = AppDelegate::pluginGameServices->isSignedIn();
		
		if (mIsGameServicesAvailable)
		{
		//	if (mScore > 0)
		//		AppDelegate::pluginGameServices->publishScore(mScore);
			
		//	if (mScore >= 10000)
		//		AppDelegate::pluginGameServices->unlockAchievement(1);
		}
	}
}

void GameScene::updateBoxDirection()
{
	cocos2d::Vec2 dir = mBox->getPhysicsBody()->getVelocity();
	
	if (dir == cocos2d::Vec2::ZERO)
		dir = cocos2d::Vec2(1, 1) * 30;
	else dir.x *= -1;/*if (dir.x > 0)
		if (dir.y > 0)
			dir.y *= -1;
		else
			dir.x *= -1;
	else if (dir.y > 0)
		dir.x *= -1;
	else
		dir.y *= -1;
	*/
	/*
	switch (mBoxDirection)
	{
		case 1:
			dir = cocos2d::Vec2(-1, -1);
			break;
		case 2:
			dir = cocos2d::Vec2(1, -1);
			break;
		case 3:
			dir = cocos2d::Vec2(1, 1);
			break;
		case 4:
			dir = cocos2d::Vec2(-1, 1);
			break;
		default:
			return;
	}
	*/
	//mBox->getPhysicsBody()->setVelocity(cocos2d::Vec2::ZERO);
	mBox->getPhysicsBody()->setVelocity(dir * 1.05f);
}

void GameScene::updateTempPoint(const cocos2d::Vec2& point)
{
	if (mTempPoint != nullptr)
	{
		removeTempPoint();
	}
	
	mTempPoint = new cocos2d::Vec2(point - helpers::Custom::getRealPosition(mGameLayer));
	
	cocos2d::Vec2 posTouch = helpers::Custom::translatePositionToCenter(*mTempPoint, mGameLayer->getContentSize());
	cocos2d::Vec2 posBox = helpers::Custom::translatePositionToCenter(mBox->getPosition(), mGameLayer->getContentSize());
	cocos2d::Vec2 a, b;
	
	cocos2d::log("comparing touch pos vs box pos %f/%f to %f/%f", posTouch.x, posTouch.y, posBox.x, posBox.y);
	
	a.x = posTouch.x;
	a.y = posTouch.y > posBox.y ? mGameLayer->getContentSize().height/2 : -mGameLayer->getContentSize().height/2;
	b.x = posTouch.x > posBox.x ? mGameLayer->getContentSize().width/2 : -mGameLayer->getContentSize().width/2;
	b.y = posTouch.y;
	
	/*
	if (dir.x > 0)
		if (dir.y > 0)
		{//top, right
			a.y = mOrigin.y + mVisibleSize.height;
			b.x = mOrigin.x + mVisibleSize.width;
		}
		else
		{//bottom, right
			a.y = mOrigin.y;
			b.x = mOrigin.x + mVisibleSize.width;
		}
	else if (dir.y > 0)
	{//top, left
		a.y = mOrigin.y + mVisibleSize.height;
		b.x = mOrigin.x;
	}
	else
	{//bottom, left
		a.y = mOrigin.y;
		b.x = mOrigin.x;
	}
	*/
	
	cocos2d::PhysicsMaterial material(0, 1, 0);
	
	mTempWallShape1 = cocos2d::PhysicsShapeEdgeSegment::create(posTouch, a, material);
	mTempWallShape2 = cocos2d::PhysicsShapeEdgeSegment::create(posTouch, b, material);
	
	cocos2d::PhysicsBody* body = cocos2d::PhysicsBody::create();
	body->addShape(mTempWallShape1, true);
	body->addShape(mTempWallShape2, true);
	body->setContactTestBitmask(0xFFFFFFFF);
	body->setTag(102);
	mTempWallLayer->setPhysicsBody(body);
	
	cocos2d::log("added 2 shapes from %f/%f to %f/%f and %f/%f", mTempPoint->x, mTempPoint->y, a.x, a.y, b.x, b.y);
}

void GameScene::removeTempPoint()
{
	if (mTempPoint == nullptr)
		return;
	
	delete mTempPoint;
	mTempPoint = nullptr;
	mTempWallLayer->setPhysicsBody(nullptr);
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("You touched id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
//	updateBoxDirection();
	
	if (mBox->getPhysicsBody()->getVelocity() == cocos2d::Vec2::ZERO)
		mBox->getPhysicsBody()->setVelocity(cocos2d::Vec2(1, 1) * 30);
	else
		updateTempPoint(touch->getLocation());
	
	return true;
}

void GameScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("You moved id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
}

void GameScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("You ended move id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
}

void GameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	cocos2d::log("button press %d", (int) keyCode);
	
}

void GameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	cocos2d::log("button release %d", (int) keyCode);
	
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		if (AppDelegate::pluginAnalytics != nullptr)
			AppDelegate::pluginAnalytics->logEvent("click_back_btn");
		
		AppDelegate::closeApp();
	}
	else if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_VOLUME_DOWN)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->decreaseVolume();
	}
	else if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_VOLUME_UP)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->increaseVolume();
	}
}


bool GameScene::onContactBegin(const cocos2d::PhysicsContact& contact)
{
	cocos2d::log("contact %d with %d", contact.getShapeA()->getBody()->getTag(), contact.getShapeB()->getBody()->getTag());
	/*
	if (contact.getShapeA()->getBody()->getTag() == Dot::PHYSICS_TAG
	 || contact.getShapeB()->getBody()->getTag() == Dot::PHYSICS_TAG)
	{
		endGame();
	}
	*/
	
	cocos2d::Vec2 v = mBox->getPhysicsBody()->getVelocity();
	
	cocos2d::log("speed old %f/%f -> new %f/%f", v.x, v.y, (v*1.05f).x, (v*1.05f).y);
	
	if (std::abs(v.x) < 150 && std::abs(v.y) < 150)
	{
		mBox->getPhysicsBody()->setVelocity(v * 1.05f);
	}
	
	
	if (contact.getShapeA()->getBody()->getTag() == 102 || contact.getShapeB()->getBody()->getTag() == 102)
	{
		removeTempPoint();
	}
	
	return true;
}

void GameScene::onComeToForeground()
{
	
}

void GameScene::onComeToBackground()
{
	
}

