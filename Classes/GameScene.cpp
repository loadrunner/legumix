#include "GameScene.h"

const float MAX_FORCE = 150.0f;

GameScene::GameScene()
{
	cocos2d::log("game scene constructed");
}

GameScene::~GameScene()
{
	cocos2d::log("game scene destructed");
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
	if (!cocos2d::LayerColor::initWithColor(cocos2d::Color4B(0, 0, 0, 255)))
	{
		return false;
	}
	
	cocos2d::UserDefault* ud = cocos2d::UserDefault::getInstance();
	
	mGameStarted = false;
	mIsGameServicesAvailable = false;
	
	mScreenSize = cocos2d::Director::getInstance()->getWinSize();
	mVisibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	mOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
	
	cocos2d::log("size %f, %f", mScreenSize.width, mScreenSize.height);
	cocos2d::log("visible size %f, %f", mVisibleSize.width, mVisibleSize.height);
	cocos2d::log("offset %f, %f", mOrigin.x, mOrigin.y);
	
	getScene()->getPhysicsWorld()->setGravity(cocos2d::Vec2::ZERO);
//	getScene()->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
	
	mGameArea = cocos2d::LayerColor::create(cocos2d::Color4B(255, 255, 255, 255));
//	mGameArea = cocos2d::ClippingRectangleNode::create(cocos2d::Rect(0, 0, mScreenSize.width, 120));
	mGameArea->ignoreAnchorPointForPosition(false);
	mGameArea->setPosition(cocos2d::Vec2::ZERO);
	mGameArea->setAnchorPoint(cocos2d::Vec2::ZERO);
	this->addChild(mGameArea, 100);
	
	mScrollContainer = cocos2d::Layer::create();
	mGameArea->addChild(mScrollContainer);
	
	mBg1 = cocos2d::LayerColor::create(cocos2d::Color4B(89, 148, 54, 255));
	mBg1->ignoreAnchorPointForPosition(false);
	mBg1->setPosition(cocos2d::Vec2(mGameArea->getContentSize().width/2, mBg1->getContentSize().height/2));
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
	
	mBox = cocos2d::Sprite::createWithSpriteFrameName("plane");
	mBox->setPosition(cocos2d::Vec2(mGameArea->getContentSize().width/2, mGameArea->getContentSize().height * 0.15f));
	mGameArea->addChild(mBox);
	
	body = cocos2d::PhysicsBody::create();
	body->setVelocity(cocos2d::Vec2::ZERO);
	body->setRotationEnable(false);
	
	cocos2d::PhysicsShape* bodyy = cocos2d::PhysicsShapeBox::create(cocos2d::Size(4, 13), cocos2d::PhysicsMaterial(1, 1, 0), cocos2d::Vec2(0, -2));
	bodyy->setTag(PHYSICS_TAG_BOX_BODY);
	bodyy->setSensor(true);
	body->addShape(bodyy);
	
	cocos2d::PhysicsShape* bot = cocos2d::PhysicsShapeBox::create(cocos2d::Size(3, 2), cocos2d::PhysicsMaterial(1, 1, 0), cocos2d::Vec2(0, 8));
	bot->setTag(PHYSICS_TAG_BOX_HEAD);
	bot->setSensor(true);
	body->addShape(bot);
	
	cocos2d::PhysicsShape* leftWing = cocos2d::PhysicsShapeBox::create(cocos2d::Size(4, 2), cocos2d::PhysicsMaterial(1, 1, 0), cocos2d::Vec2(-6, -1));
	leftWing->setTag(PHYSICS_TAG_BOX_WING);
	leftWing->setSensor(true);
	body->addShape(leftWing);
	
	cocos2d::PhysicsShape* rightWing = cocos2d::PhysicsShapeBox::create(cocos2d::Size(4, 2), cocos2d::PhysicsMaterial(1, 1, 0), cocos2d::Vec2(6, -1));
	rightWing->setTag(PHYSICS_TAG_BOX_WING);
	rightWing->setSensor(true);
	body->addShape(rightWing);
	
	body->setContactTestBitmask(0xFFFFFFFF);
	mBox->setPhysicsBody(body);
	
	mUILayer = cocos2d::Layer::create();
	this->addChild(mUILayer, 200);
	
	mScoreView = cocos2d::Label::createWithTTF("", "fonts/default.ttf", 10);
	mScoreView->setPosition(cocos2d::Vec2(mOrigin.x + 10, mOrigin.y + mVisibleSize.height - 10));
	mScoreView->setAnchorPoint(cocos2d::Vec2(0, 1));
	mUILayer->addChild(mScoreView);
	
	mProgress = 0;
	
	mProgressView = cocos2d::Label::createWithTTF("0m", "fonts/default.ttf", 10);
	mProgressView->setColor(cocos2d::Color3B::GREEN);
	mProgressView->setPosition(cocos2d::Vec2(mOrigin.x + mVisibleSize.width - 10, mOrigin.y + mVisibleSize.height - 10));
	mProgressView->setAnchorPoint(cocos2d::Vec2(1, 1));
	mUILayer->addChild(mProgressView);
	
	initPools();
	
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
	
	auto listener6 = cocos2d::EventListenerAcceleration::create(CC_CALLBACK_2(GameScene::onAcceleration, this));
	dispatcher->addEventListenerWithSceneGraphPriority(listener6, this);
	
	this->schedule(schedule_selector(GameScene::update));
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
	mObstaclePool.init(10, mScrollContainer);
	mBulletPool.init(20, mScrollContainer);
	mCoinPool.init(50, mScrollContainer);
}

void GameScene::setParent(Node* child)
{
	Node::setParent(child);
	
}

float timeFromLastObstacle = 0;

void GameScene::update(float dt)
{
	if (!mGameStarted)
		return;
	
	if (mCurrentAcceleration.x != 0)
		mBox->getPhysicsBody()->applyImpulse(cocos2d::Vec2(mCurrentAcceleration.x, 0));
	
	timeFromLastObstacle += dt;
	
	if (timeFromLastObstacle >= 1.2f)
	{
		cocos2d::log("add obs");
		timeFromLastObstacle = 0;
		
		Obstacle* obs = mObstaclePool.obtainPoolItem();
		obs->setPosition(cocos2d::Vec2((mScreenSize.width - mBg1->getContentSize().width) / 2 + rand() % (int) mBg1->getContentSize().width, -mScrollContainer->getPositionY() + mScreenSize.height));
		obs->setVisible(true);
		mObstacles.pushBack(obs);
	}
	
	mScrollContainer->setPositionY(mScrollContainer->getPositionY() - dt * 60);
	
	if (mBg2->getPositionY() - mBg2->getContentSize().height/2 <= -mScrollContainer->getPositionY())
	{
		mBg1->setPositionY(mBg2->getPositionY() + mBg2->getContentSize().height);
		cocos2d::Node* tmp = mBg1;
		mBg1 = mBg2;
		mBg2 = tmp;
	}
}

void GameScene::updateSlow(float dt)
{
	if (mObstacles.size() > 0)
	{
		Obstacle* obs = mObstacles.front();
		if (obs->getPositionY() < -mScrollContainer->getPositionY())
		{
			cocos2d::log("remove obs");
			mObstacles.eraseObject(obs);
			mObstaclePool.recyclePoolItem(obs);
		}
	}
	
	if (mGameStarted)
	{
		mProgress += dt * 0.6f;
		mProgressView->setString(cocos2d::__String::createWithFormat("%.1fm", mProgress)->_string);
	}
	
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

void GameScene::startGame()
{
	mScore = 0;
	mScoreView->setString("0");
	
	mGameStarted = true;
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("You touched id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
	if (!mGameStarted)
	{
		startGame();
	}
	else
	{
		Bullet* bullet = mBulletPool.obtainPoolItem();
		bullet->setPosition(cocos2d::Vec2(mBox->getPositionX(), -mScrollContainer->getPositionY() + mBox->getPositionY() + mBox->getContentSize().height * 0.7f));
		bullet->runAction(cocos2d::Sequence::create(
				cocos2d::MoveBy::create(0.5f, cocos2d::Vec2(0, 200)),
				cocos2d::CallFuncN::create([this](cocos2d::Node* node)
				{
					mBulletPool.recyclePoolItem((Bullet*) node);
				}),
				nullptr));
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("shoot.mp3");
	}
	
	return true;
}

void GameScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("You moved id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	/*
	if (mBox->getPhysicsBody()->getVelocity() == cocos2d::Vec2::ZERO)
	{
		float angle = helpers::Custom::getNormalizedAngle(mBox->getPosition(), touch->getLocation());
		cocos2d::log("normalized angle %f", angle);
		prastie->setRotation(angle + 90);
	}
	*/
}

void GameScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("You ended move id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	/*
	if (mBox->getPhysicsBody()->getVelocity() == cocos2d::Vec2::ZERO)
	{
		startGame(helpers::Custom::getNormalizedAngle(mBox->getPosition(), touch->getLocation()));
		
		prastie->removeFromParentAndCleanup(true);
		prastie = nullptr;
	}
	*/
}

void GameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	cocos2d::log("button press %d", (int) keyCode);
	
	if (!mGameStarted)
	{
		startGame();
		return;
	}
	
	float force = MAX_FORCE;
	
	switch (keyCode)
	{
		case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_LEFT:
			mCurrentAcceleration.x = -force;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_RIGHT:
			mCurrentAcceleration.x = force;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
		case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_UP:
			mCurrentAcceleration.y = force;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_DOWN:
			mCurrentAcceleration.y = -force;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_CENTER:
		case cocos2d::EventKeyboard::KeyCode::KEY_ENTER:
		case cocos2d::EventKeyboard::KeyCode::KEY_KP_ENTER:
		case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:
		{
			Bullet* bullet = mBulletPool.obtainPoolItem();
			bullet->setPosition(cocos2d::Vec2(mBox->getPositionX(), -mScrollContainer->getPositionY() + mBox->getPositionY() + mBox->getContentSize().height * 0.7f));
			bullet->runAction(cocos2d::Sequence::create(
					cocos2d::MoveBy::create(0.5f, cocos2d::Vec2(0, 200)),
					cocos2d::CallFuncN::create([this](cocos2d::Node* node)
					{
						mBulletPool.recyclePoolItem((Bullet*) node);
					}),
					nullptr));
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("shoot.mp3");
			break;
		}
		default:
			return;
	}
	
	mPressedKeys |= 1 << (int) keyCode;
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
	
	if (mPressedKeys != 0)
	{
		switch (keyCode)
		{
			case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_LEFT:
				if (mCurrentAcceleration.x < 0)
					mCurrentAcceleration.x = 0;
				break;
			case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_RIGHT:
				if (mCurrentAcceleration.x > 0)
					mCurrentAcceleration.x = 0;
				break;
			case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
			case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_UP:
				if (mCurrentAcceleration.y > 0)
					mCurrentAcceleration.y = 0;
				break;
			case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			case cocos2d::EventKeyboard::KeyCode::KEY_DPAD_DOWN:
				if (mCurrentAcceleration.y < 0)
					mCurrentAcceleration.y = 0;
				break;
			default:
				return;
		}
		
		mPressedKeys &= ~(1 << (int) keyCode);
	}
}

bool GameScene::onContactBegin(const cocos2d::PhysicsContact& contact)
{
	cocos2d::log("contact %d with %d", contact.getShapeA()->getTag(), contact.getShapeB()->getTag());
	/*
	if (contact.getShapeA()->getBody()->getTag() == Dot::PHYSICS_TAG
	 || contact.getShapeB()->getBody()->getTag() == Dot::PHYSICS_TAG)
	{
		endGame();
	}
	*/
	
	cocos2d::Vec2 v = mBox->getPhysicsBody()->getVelocity();
	cocos2d::Vec2 newV = v;
	
//	if (std::abs(v.x) < 150 || std::abs(v.y) < 150)
//	{
//		newV = v * 1.01f;
//		cocos2d::log("speed old %f/%f -> new %f/%f", v.x, v.y, newV.x, newV.y);
//	}
	
//	newV = helpers::Custom::normalizeVelocity(newV);
	
//	if (v != newV)
//		mBox->getPhysicsBody()->setVelocity(newV);
	
	if (helpers::Custom::isContactBetweenAB(contact, PHYSICS_TAG_BOX_BODY, PHYSICS_TAG_EDGE_LEFT))
	{
		cocos2d::Vec2 v = mBox->getPhysicsBody()->getVelocity();
		mBox->getPhysicsBody()->setVelocity(cocos2d::Vec2(std::abs(v.x) * 0.5f, 0));
	}
	else if (helpers::Custom::isContactBetweenAB(contact, PHYSICS_TAG_BOX_BODY, PHYSICS_TAG_EDGE_RIGHT))
	{
		cocos2d::Vec2 v = mBox->getPhysicsBody()->getVelocity();
		mBox->getPhysicsBody()->setVelocity(cocos2d::Vec2(-std::abs(v.x) * 0.5f, 0));
	}
	else if (helpers::Custom::isContactBetweenAB(contact, PHYSICS_TAG_BOX_BODY, Obstacle::PHYSICS_TAG))
	{
		cocos2d::Scene* scene = GameScene::createScene();
		scene->retain();
		mBox->getPhysicsBody()->setVelocity(cocos2d::Vec2::ZERO);
		mBox->setColor(cocos2d::Color3B::GRAY);
		mGameStarted = false;
		runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(1.5f),
				cocos2d::CallFunc::create(CC_CALLBACK_0(cocos2d::Director::replaceScene, cocos2d::Director::getInstance(), scene)),
				cocos2d::CallFunc::create(CC_CALLBACK_0(cocos2d::Ref::release, scene)), nullptr));
	}
	else if (helpers::Custom::isContactBetweenAB(contact, Obstacle::PHYSICS_TAG, Bullet::PHYSICS_TAG))
	{
		Obstacle* obstacle = (Obstacle*) helpers::PhysicsCollisions::getShape(contact, Obstacle::PHYSICS_TAG)->getBody()->getNode();
		cocos2d::Vec2 pos = obstacle->getPosition();
		mObstacles.eraseObject(obstacle);
		mObstaclePool.recyclePoolItem(obstacle);
		
		Bullet* bullet = (Bullet*) helpers::PhysicsCollisions::getShape(contact, Bullet::PHYSICS_TAG)->getBody()->getNode();
		mBulletPool.recyclePoolItem(bullet);
		
		runAction(cocos2d::CallFunc::create([this, pos]()
				{
					int n = 2 + rand() % 8;
					for (int i = 0; i < n; i++)
					{
						Coin* coin = mCoinPool.obtainPoolItem();
						coin->setPosition(pos);
						cocos2d::log("add coin to %f %f", pos.x, pos.y);
						coin->runAction(cocos2d::Sequence::create(
								cocos2d::MoveBy::create(0.15f, cocos2d::Vec2(-15 + rand() % 30, -15 + rand() % 30)),//(i % 2 ? 5 : 1) * ((i+1) % 2 ? -1 : 1), (i % 2 ? 5 : 1) * ((i+1) % 2 ? -1 : 1))),
								cocos2d::DelayTime::create(4.0f),
								cocos2d::CallFuncN::create([this](cocos2d::Node* node)
								{
									mCoinPool.recyclePoolItem((Coin*) node);
								}),
								nullptr));
					}
				}));
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("kill.wav");
	}
	else if (helpers::Custom::isContactBetweenAB(contact, PHYSICS_TAG_BOX_BODY, Coin::PHYSICS_TAG))
	{
		Coin* coin = (Coin*) helpers::PhysicsCollisions::getShape(contact, Coin::PHYSICS_TAG)->getBody()->getNode();
		mCoinPool.recyclePoolItem(coin);
		
		mScore++;
		mScoreView->setString(cocos2d::__String::createWithFormat("%d", mScore)->_string);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("collect.wav");
	}
	
	return false;
}

void GameScene::onAcceleration(cocos2d::Acceleration* acc, cocos2d::Event* unused_event)
{
	mCurrentAcceleration.x = acc->x * MAX_FORCE;
	mCurrentAcceleration.y = acc->y * MAX_FORCE;
	
	cocos2d::log("new acc %f %f (%f %f)", mCurrentAcceleration.x, mCurrentAcceleration.y, acc->x* MAX_FORCE, acc->y* MAX_FORCE);
}

void GameScene::onComeToForeground()
{
	
}

void GameScene::onComeToBackground()
{
	
}

