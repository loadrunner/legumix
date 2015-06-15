#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "Helpers.h"
#include "Hero.h"
#include "Objects.h"
#include "Pools.h"
#include "AppDelegate.h"

class GameScene : public cocos2d::LayerColor
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init() override;
	
	const long PHYSICS_TAG_EDGE = 1 << 4;
	const long PHYSICS_TAG_EDGE_LEFT = PHYSICS_TAG_EDGE | 1 << 5;
	const long PHYSICS_TAG_EDGE_RIGHT = PHYSICS_TAG_EDGE | 1 << 6;
	
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
	
	cocos2d::Node* mGameArea;
	cocos2d::Layer* mScrollContainer;
	cocos2d::Node* mBg1;
	cocos2d::Node* mBg2;
	cocos2d::Layer* mUILayer;
	
	Hero* mBox;
	
	int mScore;
	cocos2d::Label* mScoreView;
	cocos2d::Label* mLifeView;
	
	HaystackPool mHaystackPool;
	cocos2d::Vector<Obstacle*> mObstacles;
	
	BulletPool mBulletPool;
	CoinPool mCoinPool;
	
	float mProgress;
	cocos2d::Label* mProgressView;
	
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
