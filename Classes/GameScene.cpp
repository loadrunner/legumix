#include "GameScene.h"

const float MAX_FORCE = 1000.0f;

BulletPool GameScene::mBulletPool;

GameScene::GameScene()
{
	cocos2d::log("game scene constructed");
}

GameScene::~GameScene()
{
	cocos2d::log("game scene destructed");
	
	//TODO: empty pools
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
	
	mWorldLayer = WorldLayer::create();
	mWorldLayer->setPosition(cocos2d::Vec2::ZERO);
	mWorldLayer->setAnchorPoint(cocos2d::Vec2::ZERO);
	this->addChild(mWorldLayer, 100);
	
	mHero = TomatoHero::create();
	mHero->setPosition(cocos2d::Vec2(mWorldLayer->getContentSize().width/2, mWorldLayer->getContentSize().height * 0.15f));
	mWorldLayer->addChild(mHero);
	
	mUILayer = UILayer::create(mVisibleSize);
	mUILayer->setPosition(mOrigin);
	mUILayer->setAnchorPoint(cocos2d::Vec2::ZERO);
	this->addChild(mUILayer, 200);
	
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
	
	cocos2d::EventListener* listener6;
//	if (cocos2d::Device::hasGyroscope())
//		listener6 = cocos2d::EventListenerGyroscope::create(CC_CALLBACK_2(GameScene::onGyroscope, this));
//	else
		listener6 = cocos2d::EventListenerAcceleration::create(CC_CALLBACK_2(GameScene::onAcceleration, this));
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
	mHaystackPool.init(10, Haystack::create(), mWorldLayer->getScrollContainer());
	mTowerPool.init(10, Tower::create(), mWorldLayer->getScrollContainer());
	mBulletPool.init(20, mWorldLayer->getScrollContainer());
	mCoinPool.init(50, Coin::create(), mWorldLayer->getScrollContainer());
	mTomatoPool.init(50, Tomato::create(), mWorldLayer->getScrollContainer());
	mBroccoliPool.init(50, Broccoli::create(), mWorldLayer->getScrollContainer());
}

void GameScene::setParent(Node* child)
{
	Node::setParent(child);
	
}

float timeFromLastCollectable = 0;
float timeFromLastObstacle = 0;

void GameScene::update(float dt)
{
	if (!mGameStarted)
		return;
	
	//mHero->getPhysicsBody()->resetForces();
	if (mCurrentAcceleration.x != 0)
		mHero->getPhysicsBody()->applyImpulse(cocos2d::Vec2(mCurrentAcceleration.x, 0));
	
	timeFromLastCollectable += dt;
	timeFromLastObstacle += dt;
	
	if (timeFromLastCollectable >= 1.0f)
	{
		timeFromLastCollectable = 0;
		
		Object* obj;
		switch (rand()%2)
		{
			case 0:
				obj = mTomatoPool.obtainPoolItem();
				obj->setPosition(cocos2d::Vec2((mScreenSize.width - mWorldLayer->getContentSize().width) / 2 + rand() % (int) mWorldLayer->getContentSize().width, -mWorldLayer->getScrollContainer()->getPositionY() + mScreenSize.height));
				obj->setVisible(true);
				mObjects.pushBack(obj);
				break;
			case 1:
				obj = mBroccoliPool.obtainPoolItem();
				obj->setPosition(cocos2d::Vec2((mScreenSize.width - mWorldLayer->getContentSize().width) / 2 + rand() % (int) mWorldLayer->getContentSize().width, -mWorldLayer->getScrollContainer()->getPositionY() + mScreenSize.height));
				obj->setVisible(true);
				mObjects.pushBack(obj);
				break;
			default:
				;
		}
	}
	
	if (timeFromLastObstacle >= 1.0f)
	{
		timeFromLastObstacle = 0;
		
		Object* obj;
		switch (rand()%4)
		{
			case 0:
				obj = mTowerPool.obtainPoolItem();
				obj->setPosition(cocos2d::Vec2((mScreenSize.width - mWorldLayer->getContentSize().width) / 2 + rand() % (int) mWorldLayer->getContentSize().width, -mWorldLayer->getScrollContainer()->getPositionY() + mScreenSize.height));
				obj->setVisible(true);
				mObjects.pushBack(obj);
				mTowers.pushBack((Tower*) obj);
				break;
			case 1:
			case 2:
			case 3:
				obj = mHaystackPool.obtainPoolItem();
				obj->setPosition(cocos2d::Vec2((mScreenSize.width - mWorldLayer->getContentSize().width) / 2 + rand() % (int) mWorldLayer->getContentSize().width, -mWorldLayer->getScrollContainer()->getPositionY() + mScreenSize.height));
				obj->setVisible(true);
				mObjects.pushBack(obj);
				break;
			default:
				;
		}
	}
	
	mWorldLayer->update(dt);
}

void GameScene::updateSlow(float dt)
{
	if (mObjects.size() > 0)
	{
		while (!mObjects.empty())
		{
			Object* obj = mObjects.front();
			if (obj->getPositionY() < -mWorldLayer->getScrollContainer()->getPositionY())
			{
				mObjects.eraseObject(obj);
				
				Haystack* haystack = dynamic_cast<Haystack*>(obj);
				if (haystack != nullptr)
				{
					cocos2d::log("recycle hay");
					mHaystackPool.recyclePoolItem(haystack);
					continue;
				}
				
				Tomato* tomato = dynamic_cast<Tomato*>(obj);
				if (tomato != nullptr)
				{
					cocos2d::log("recycle tom");
					mTomatoPool.recyclePoolItem(tomato);
					continue;
				}
				
				Broccoli* broccoli = dynamic_cast<Broccoli*>(obj);
				if (broccoli != nullptr)
				{
					cocos2d::log("recycle bro");
					mBroccoliPool.recyclePoolItem(broccoli);
					continue;
				}
				
				Tower* tower = dynamic_cast<Tower*>(obj);
				if (tower != nullptr)
				{
					cocos2d::log("recycle tower");
					mTowerPool.recyclePoolItem(tower);
					mTowers.eraseObject(tower);
					continue;
				}
			}
			else
				break;
		}
	}
	
	if (mGameStarted)
	{
		mProgress += dt * 0.6f;
		mUILayer->updateProgress(mProgress);
		
		cocos2d::Vec2 heroPos = cocos2d::Vec2(mHero->getPositionX(), mHero->getPositionY() - mWorldLayer->getScrollContainer()->getPositionY());
		for (auto it = mTowers.begin(); it != mTowers.end(); it++)
		{
			(*it)->update(dt, heroPos);
		}
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
	mProgress = 0;
	mHero->reset();
	
	mUILayer->updateScore(mScore);
	mUILayer->updateLife(mHero->getLife());
	mUILayer->updateProgress(mProgress);
	
	mGameStarted = true;
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	//cocos2d::log("You touched id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	
	if (!mGameStarted)
	{
		startGame();
	}
	else
	{
		Bullet* bullet = mBulletPool.obtainPoolItem();
		bullet->setLauncher(mHero);
		bullet->setPosition(cocos2d::Vec2(mHero->getPositionX(), -mWorldLayer->getScrollContainer()->getPositionY() + mHero->getPositionY() + mHero->getContentSize().height * 0.7f));
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
	//cocos2d::log("You moved id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	/*
	if (mHero->getPhysicsBody()->getVelocity() == cocos2d::Vec2::ZERO)
	{
		float angle = helpers::Custom::getNormalizedAngle(mHero->getPosition(), touch->getLocation());
		cocos2d::log("normalized angle %f", angle);
		prastie->setRotation(angle + 90);
	}
	*/
}

void GameScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	//cocos2d::log("You ended move id %d - %f, %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
	/*
	if (mHero->getPhysicsBody()->getVelocity() == cocos2d::Vec2::ZERO)
	{
		startGame(helpers::Custom::getNormalizedAngle(mHero->getPosition(), touch->getLocation()));
		
		prastie->removeFromParentAndCleanup(true);
		prastie = nullptr;
	}
	*/
}

void GameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	//cocos2d::log("button press %d", (int) keyCode);
	
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
			bullet->setLauncher(mHero);
			bullet->setPosition(cocos2d::Vec2(mHero->getPositionX(), -mWorldLayer->getScrollContainer()->getPositionY() + mHero->getPositionY() + mHero->getContentSize().height * 0.7f));
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
	//cocos2d::log("button release %d", (int) keyCode);
	
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
	cocos2d::log("contact %ld with %ld", contact.getShapeA()->getTag(), contact.getShapeB()->getTag());
	/*
	if (contact.getShapeA()->getBody()->getTag() == Dot::PHYSICS_TAG
	 || contact.getShapeB()->getBody()->getTag() == Dot::PHYSICS_TAG)
	{
		endGame();
	}
	*/
	
	cocos2d::Vec2 v = mHero->getPhysicsBody()->getVelocity();
	cocos2d::Vec2 newV = v;
	
//	if (std::abs(v.x) < 150 || std::abs(v.y) < 150)
//	{
//		newV = v * 1.01f;
//		cocos2d::log("speed old %f/%f -> new %f/%f", v.x, v.y, newV.x, newV.y);
//	}
	
//	newV = helpers::Custom::normalizeVelocity(newV);
	
//	if (v != newV)
//		mHero->getPhysicsBody()->setVelocity(newV);
	
	if (helpers::Custom::isContactBetweenAB(contact, Hero::PHYSICS_TAG_BODY, WorldLayer::PHYSICS_TAG_EDGE))
	{
		cocos2d::Vec2 v = mHero->getPhysicsBody()->getVelocity();
		mHero->getPhysicsBody()->setVelocity(cocos2d::Vec2(v.x * -0.5f, 0));
	}
	else if (helpers::Custom::isContactBetweenAB(contact, Hero::PHYSICS_TAG_BODY, Obstacle::PHYSICS_TAG))
	{
		mHero->loseLife();
		mUILayer->updateLife(mHero->getLife());
		//TODO: implement death
	}
	else if (helpers::PhysicsCollisions::getShape(contact, Bullet::PHYSICS_TAG))
	{
		cocos2d::Node* node = helpers::PhysicsCollisions::getShapeContactedBy(contact, Bullet::PHYSICS_TAG)->getBody()->getNode();
		Bullet* bullet = (Bullet*) helpers::PhysicsCollisions::getShape(contact, Bullet::PHYSICS_TAG)->getBody()->getNode();
		
		if (node != nullptr && bullet->getLauncher() == node)
			return false; // ignore own bullet
		
		if (node == mHero)
		{
			mHero->hit(Bullet::PHYSICS_TAG, 1);
			mUILayer->updateLife(mHero->getLife());
			mBulletPool.recyclePoolItem(bullet);
		}
		else
		{
			Object* object = dynamic_cast<Object*>(node);
			if (object == nullptr || !object->canBeShotBy(Bullet::PHYSICS_TAG))
				return false;
			
			mBulletPool.recyclePoolItem(bullet);
			
			bool dead = object->hit(Bullet::PHYSICS_TAG, 1);
			if (dead)
				mObjects.eraseObject(object);
			
			cocos2d::Vec2 pos = object->getPosition();
			
			Obstacle* obstacle = dynamic_cast<Obstacle*>(object);
			if (obstacle != nullptr)
			{
				Haystack* haystack = dynamic_cast<Haystack*>(object);
				if (haystack != nullptr)
					if (dead)
						mHaystackPool.recyclePoolItem(haystack);
				
				if (dead)
				{
					runAction(cocos2d::CallFunc::create([this, pos]()
							{
								int n = 2 + rand() % 8;
								for (int i = 0; i < n; i++)
								{
									Coin* coin = (Coin*) mCoinPool.obtainPoolItem();
									coin->setPosition(pos);
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
			}
			
			Collectable* collectable = dynamic_cast<Collectable*>(object);
			if (collectable != nullptr)
			{
				Collectable* collectable = (Collectable*) helpers::PhysicsCollisions::getShape(contact, Collectable::PHYSICS_TAG)->getBody()->getNode();
				
				Coin* coin = dynamic_cast<Coin*>(collectable);
				if (coin != nullptr)
				{
					if (dead)
						mCoinPool.recyclePoolItem(coin);
					
					return false;
				}
				
				Tomato* tomato = dynamic_cast<Tomato*>(collectable);
				if (tomato != nullptr)
					if (dead)
						mTomatoPool.recyclePoolItem(tomato);
				
				Broccoli* broccoli = dynamic_cast<Broccoli*>(collectable);
				if (broccoli != nullptr)
					if (dead)
						mBroccoliPool.recyclePoolItem(broccoli);
				
				if (dead)
				{
					runAction(cocos2d::CallFunc::create([this, pos]()
							{
								int n = 2 + rand() % 8;
								for (int i = 0; i < n; i++)
								{
									Coin* coin = (Coin*) mCoinPool.obtainPoolItem();
									coin->setPosition(pos);
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
			}
			
			Enemy* enemy = dynamic_cast<Enemy*>(object);
			if (enemy != nullptr)
			{
				Tower* tower = dynamic_cast<Tower*>(enemy);
				if (tower != nullptr)
					if (dead)
						mTowerPool.recyclePoolItem(tower);
				
				if (dead)
				{
					runAction(cocos2d::CallFunc::create([this, pos]()
							{
								int n = 2 + rand() % 8;
								for (int i = 0; i < n; i++)
								{
									Coin* coin = (Coin*) mCoinPool.obtainPoolItem();
									coin->setPosition(pos);
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
			}
		}
	}
	else if (helpers::Custom::isContactBetweenAB(contact, Hero::PHYSICS_TAG_BODY, Coin::PHYSICS_TAG))
	{
		Coin* coin = (Coin*) helpers::PhysicsCollisions::getShape(contact, Coin::PHYSICS_TAG)->getBody()->getNode();
		mCoinPool.recyclePoolItem(coin);
		
		mScore++;
		mUILayer->updateScore(mScore);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("collect.wav");
	}
	else if (helpers::Custom::isContactBetweenAB(contact, Hero::PHYSICS_TAG_BODY, Collectable::PHYSICS_TAG))
	{
		Collectable* collectable = (Collectable*) helpers::PhysicsCollisions::getShape(contact, Collectable::PHYSICS_TAG)->getBody()->getNode();
		cocos2d::Vec2 pos = collectable->getPosition();
		mObjects.eraseObject(collectable);
		
		Tomato* tomato = dynamic_cast<Tomato*>(collectable);
		if (tomato != nullptr)
		{
			mTomatoPool.recyclePoolItem(tomato);
			
			mHero->increaseLife();
			mUILayer->updateLife(mHero->getLife());
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("collect.wav");
		}
		
		Broccoli* broccoli = dynamic_cast<Broccoli*>(collectable);
		if (broccoli != nullptr)
			mBroccoliPool.recyclePoolItem(broccoli);
	}
	
	return false;
}

void GameScene::onAcceleration(cocos2d::Acceleration* acc, cocos2d::Event* unused_event)
{
	std::function<float(float, float, float)> fix = [](float v, float min, float max)
			{
				if (v > max)
					return 1.0f;
				
				if (v < -max)
					return -1.0f;
				
				if (v < min && v > -min)
					return 0.0f;
				
				return (v - (v > 0 ? min : -min)) / max;
			};
	
	float x = fix(acc->x, 0.02f, 0.3f);
	float y = fix(acc->y, 0.02f, 0.3f);
	
	mCurrentAcceleration.x = x * MAX_FORCE;
	mCurrentAcceleration.y = y * MAX_FORCE;
	
	//cocos2d::log("new acc %f %f (%f %f)", acc->x, acc->y, mCurrentAcceleration.x, mCurrentAcceleration.y);
}

void GameScene::onGyroscope(cocos2d::Gyroscope* gyro, cocos2d::Event* unused_event)
{
	std::function<float(float, float, float)> fix = [](float v, float min, float max)
			{
				if (v > max)
					return 1.0f;
				
				if (v < -max)
					return -1.0f;
				
				if (v < min && v > -min)
					return 0.0f;
				
				return (v - (v > 0 ? min : -min)) / max;
			};
	
	float x = fix(gyro->x, 0.0f, 0.1f);
	float y = fix(gyro->y, 0.0f, 0.1f);
	
	mCurrentAcceleration.x = x * MAX_FORCE;
	mCurrentAcceleration.y = y * MAX_FORCE;
	
	//cocos2d::log("new gyro %f %f, %f %f (%f %f)", x, y, gyro->x, gyro->y, mCurrentAcceleration.x, mCurrentAcceleration.y);
}

void GameScene::onComeToForeground()
{
	
}

void GameScene::onComeToBackground()
{
	
}
