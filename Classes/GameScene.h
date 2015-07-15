#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "Helpers.h"
#include "Hero.h"
#include "Objects.h"
#include "Pools.h"
#include "AppDelegate.h"
#include "WorldLayer.h"
#include "UILayer.h"

class GameScene : public cocos2d::LayerColor
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init() override;
	
private:
	GameScene();
	~GameScene();
	
	cocos2d::Size mScreenSize;
	cocos2d::Size mVisibleSize;
	cocos2d::Vec2 mOrigin;
	
	bool mGameStarted;
	bool mIsGameServicesAvailable;
	
	int mPressedKeys = 0;
	cocos2d::Vec2 mCurrentAcceleration;
	
	WorldLayer* mWorldLayer;
	UILayer* mUILayer;
	
	Hero* mHero;
	
	ObjectPool mHaystackPool;
	cocos2d::Vector<Object*> mObjects;
	cocos2d::Vector<Tower*> mTowers;
	
	BulletPool mBulletPool;
	ObjectPool mCoinPool;
	
	ObjectPool mTomatoPool;
	ObjectPool mBroccoliPool;
	ObjectPool mTowerPool;
	
	int mScore;
	float mProgress;
	
	void startGame();
	
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	bool onContactBegin(const cocos2d::PhysicsContact& contact);
	void onAcceleration(cocos2d::Acceleration* acc, cocos2d::Event* unused_event);
	void onGyroscope(cocos2d::Gyroscope* gyro, cocos2d::Event* unused_event);
	void onComeToForeground();
	void onComeToBackground();
	
	void setParent(Node* child) override;
	
	void update(float dt);
	void updateSlow(float dt);
	
	void initPools();
};

#endif // __GAMESCENE_H__
