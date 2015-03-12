#include "GameScene.h"

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
	
	getScene()->getPhysicsWorld()->setGravity(cocos2d::Vec2::ZERO);
	getScene()->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
	
	mGameArea = cocos2d::LayerColor::create(cocos2d::Color4B(15, 50, 15, 200));
//	mGameArea = cocos2d::ClippingRectangleNode::create(cocos2d::Rect(0, 0, mScreenSize.width, 120));
	this->addChild(mGameArea, 100);
	
	mScrollContainer = cocos2d::Layer::create();
	mGameArea->addChild(mScrollContainer);
	
	mBg1 = cocos2d::Sprite::createWithSpriteFrameName("sheet");
	mBg1->setPosition(cocos2d::Vec2(mGameArea->getContentSize().width/2, mBg1->getContentSize().height/2));
	mScrollContainer->addChild(mBg1);
	
	cocos2d::PhysicsMaterial material(0, 1, 0);
	cocos2d::PhysicsBody* body = cocos2d::PhysicsBody::create();
	body->addShape(cocos2d::PhysicsShapeEdgeSegment::create(cocos2d::Vec2(-mBg1->getContentSize().width/2, -mBg1->getContentSize().height/2), cocos2d::Vec2(-mBg1->getContentSize().width/2, mBg1->getContentSize().height/2), material), true);
	body->addShape(cocos2d::PhysicsShapeEdgeSegment::create(cocos2d::Vec2(mBg1->getContentSize().width/2, mBg1->getContentSize().height/2), cocos2d::Vec2(mBg1->getContentSize().width/2, -mBg1->getContentSize().height/2), material), true);
	
	body->setDynamic(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	mBg1->setPhysicsBody(body);
	
	mBg2 = cocos2d::Sprite::createWithSpriteFrameName("sheet");
	mBg2->setPosition(mBg1->getPosition() + cocos2d::Vec2(0, mBg1->getContentSize().height));
	mScrollContainer->addChild(mBg2);
	
	body = cocos2d::PhysicsBody::create();
	body->addShape(cocos2d::PhysicsShapeEdgeSegment::create(cocos2d::Vec2(-mBg1->getContentSize().width/2, -mBg1->getContentSize().height/2), cocos2d::Vec2(-mBg1->getContentSize().width/2, mBg1->getContentSize().height/2), material), true);
	body->addShape(cocos2d::PhysicsShapeEdgeSegment::create(cocos2d::Vec2(mBg1->getContentSize().width/2, mBg1->getContentSize().height/2), cocos2d::Vec2(mBg1->getContentSize().width/2, -mBg1->getContentSize().height/2), material), true);
	
	body->setDynamic(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	mBg2->setPhysicsBody(body);
	
	mBox = cocos2d::LayerColor::create(cocos2d::Color4B::BLUE);
	mBox->setContentSize(cocos2d::Size(8, 8));
	mBox->ignoreAnchorPointForPosition(false);
	mBox->setPosition(cocos2d::Vec2(mGameArea->getContentSize().width/2, mGameArea->getContentSize().height * 0.15f));
	mScrollContainer->addChild(mBox);
	
	body = cocos2d::PhysicsBody::createBox(mBox->getContentSize(), cocos2d::PhysicsMaterial(1, 1, 0), cocos2d::Vec2::ZERO);
	body->setVelocity(cocos2d::Vec2::ZERO);
	body->setRotationEnable(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	mBox->setPhysicsBody(body);
	
	mGoodie = nullptr;
	
	mUILayer = cocos2d::Layer::create();
	this->addChild(mUILayer, 200);
	
	mWallCounterView = cocos2d::Label::createWithTTF("", "fonts/default.ttf", 10);
	mWallCounterView->setPosition(cocos2d::Vec2(mOrigin.x + 10, mOrigin.y + mVisibleSize.height - 10));
	mWallCounterView->setAnchorPoint(cocos2d::Vec2(0, 1));
	mUILayer->addChild(mWallCounterView);
	
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
	mWallPool.init(5, mScrollContainer);
	mWallCounter = 5;
	mWallCounterView->setString("5");
	
	mObstaclePool.init(10, mScrollContainer);
}

void GameScene::setParent(Node* child)
{
	Node::setParent(child);
	
}

float timeFromLastObstacle = 0;

void GameScene::update(float dt)
{
	if (mBox->getPhysicsBody()->getVelocity() != cocos2d::Vec2::ZERO)
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
	
	mScrollContainer->setPositionY(-mBox->getPositionY() + mGameArea->getContentSize().height * 0.15f);
	
	if (mBg2->getPositionY() - mBg2->getContentSize().height/2 <= -mScrollContainer->getPositionY())
	{
		mBg1->setPositionY(mBg2->getPositionY() + mBg2->getContentSize().height);
		cocos2d::Sprite* tmp = mBg1;
		mBg1 = mBg2;
		mBg2 = tmp;
	}
}

void GameScene::updateSlow(float dt)
{
	if (mManualWalls.size() > 0)
	{
		Wall* wall = mManualWalls.front();
		if (wall->getPositionY() < -mScrollContainer->getPositionY())
			recycleWall(wall);
	}
	
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
	
	if (mBox->getPhysicsBody()->getVelocity() != cocos2d::Vec2::ZERO)
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

void GameScene::startGame(float angle)
{
	angle = CC_DEGREES_TO_RADIANS(angle);
	cocos2d::log("xxxxxxxxxxxxx %f %f", sin(angle), cos(angle));
	
	mBox->getPhysicsBody()->setVelocity(cocos2d::Vec2(sin(angle), cos(angle)) * 70);
	
	moveGoodie();
}

void GameScene::moveGoodie()
{
	if (mGoodie != nullptr)
	{
		mGoodie->removeFromParentAndCleanup(true);
	}
	
	cocos2d::Vec2 newPos = cocos2d::Vec2(rand() % (int) mGameArea->getContentSize().width, rand() % (int) mGameArea->getContentSize().height);
	cocos2d::log("put goodie to new pos %f/%f", newPos.x, newPos.y);
	
	mGoodie = cocos2d::Node::create();
	mGoodie->setContentSize(cocos2d::Size(3, 3));
	mGoodie->setVisible(true);
	mGoodie->setPosition(newPos);
	mScrollContainer->addChild(mGoodie);
	
	cocos2d::PhysicsShape* shape = cocos2d::PhysicsShapeCircle::create(mGoodie->getContentSize().width/2);
	shape->setSensor(true);
	shape->setTag(200);
	
	cocos2d::PhysicsBody* body = cocos2d::PhysicsBody::create();
	body->addShape(shape);
	body->setDynamic(false);
	body->setContactTestBitmask(0xFFFFFFFF);
	
	mGoodie->setPhysicsBody(body);
}

void GameScene::setManualWall(const cocos2d::Vec2& point)
{
	if (mWallCounter <= 0)
		return;
	
	cocos2d::Node* node;
	cocos2d::Vec2 a, b;
	/*
	if (helpers::Custom::containsPoint(mBg1, point))
		node = mBg1;
	else if (helpers::Custom::containsPoint(mBg2, point))
		node = mBg2;
	else
		return;
	*/
	
	Wall* wall = mWallPool.obtainPoolItem();
	wall->setPosition(point);
	wall->setVisible(true);
	mManualWalls.pushBack(wall);
	
	mWallCounter--;
	mWallCounterView->setString(cocos2d::__String::createWithFormat("%d", mWallCounter)->_string);
	
	/*
	mTempPoint = new cocos2d::Vec2(point - helpers::Custom::getRealPosition(mSc));
	
	cocos2d::Vec2 posTouch = helpers::Custom::translatePositionToCenter(*mTempPoint, mGameLayer->getContentSize());
	cocos2d::Vec2 posBox = helpers::Custom::translatePositionToCenter(mBox->getPosition(), mGameLayer->getContentSize());
	cocos2d::Vec2 a, b;
	
	cocos2d::log("comparing touch pos vs box pos %f/%f to %f/%f", posTouch.x, posTouch.y, posBox.x, posBox.y);
	
	a.x = posTouch.x;
	a.y = posTouch.y > posBox.y ? mGameLayer->getContentSize().height/2 : -mGameLayer->getContentSize().height/2;
	b.x = posTouch.x > posBox.x ? mGameLayer->getContentSize().width/2 : -mGameLayer->getContentSize().width/2;
	b.y = posTouch.y;
	
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
	*/
}

void GameScene::recycleWall(Wall* wall)
{
	mManualWalls.eraseObject(wall);
	mWallPool.recyclePoolItem(wall);
	
	mWallCounter++;
	mWallCounterView->setString(cocos2d::__String::createWithFormat("%d", mWallCounter)->_string);
}

cocos2d::Sprite* prastie = nullptr;

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("You touched id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
	if (mBox->getPhysicsBody()->getVelocity() == cocos2d::Vec2::ZERO)
	{
		prastie = cocos2d::Sprite::createWithSpriteFrameName("line");
		prastie->setPosition(mBox->getPosition());
		prastie->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
		mGameArea->addChild(prastie);
	}
	else if (mWallCounter > 0)
	{
		cocos2d::Vec2 local = mScrollContainer->convertToNodeSpace(touch->getLocation());
		runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.3f), cocos2d::CallFunc::create(CC_CALLBACK_0(GameScene::setManualWall, this, local)), nullptr));
		
		//tmp
		auto sprite = cocos2d::Sprite::createWithSpriteFrameName("wall");
		sprite->setPosition(local);
		mScrollContainer->addChild(sprite);
		sprite->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.4f), cocos2d::RemoveSelf::create(true), nullptr));
	}
	
	return true;
}

void GameScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("You moved id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
	if (mBox->getPhysicsBody()->getVelocity() == cocos2d::Vec2::ZERO)
	{
		float angle = helpers::Custom::getNormalizedAngle(mBox->getPosition(), touch->getLocation());
		cocos2d::log("normalized angle %f", angle);
		prastie->setRotation(angle);
	}
}

void GameScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::log("You ended move id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
	if (mBox->getPhysicsBody()->getVelocity() == cocos2d::Vec2::ZERO)
	{
		startGame(helpers::Custom::getNormalizedAngle(mBox->getPosition(), touch->getLocation()));
		
		prastie->removeFromParentAndCleanup(true);
		prastie = nullptr;
	}
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
	
	cocos2d::Node* node = nullptr;
	
	if (node = helpers::Custom::getNodeByShapeTag(contact, 110))
	{
		float boxY = mBox->getPositionY() + mBox->getContentSize().height * (1.0f - mBox->getAnchorPoint().y);
		float wallY = node->getPositionY() - node->getContentSize().height * node->getAnchorPoint().y;
		if (boxY < wallY)
		{
			recycleWall(dynamic_cast<Wall*>(node));
			
			return false;
		}
	}
	else if (node = helpers::Custom::getNodeByShapeTag(contact, 210))
	{
		cocos2d::Director::getInstance()->replaceScene(GameScene::createScene());
	}
	else if (contact.getShapeA()->getTag() == 200 || contact.getShapeB()->getTag() == 200)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("collect.mp3");
		moveGoodie();
	}
	
	return true;
}

void GameScene::onComeToForeground()
{
	
}

void GameScene::onComeToBackground()
{
	
}

