#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "Helpers.h"
#include "Loader.h"
#include "Objects.h"
#include "AppDelegate.h"

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
	
	bool mIsGameServicesAvailable;
	
	cocos2d::Layer* mGameLayer;
	cocos2d::Layer* mUILayer;
	
	cocos2d::Node* mBox;
	
	cocos2d::Vec2* mTempPoint;
	cocos2d::Layer* mTempWallLayer;
	cocos2d::PhysicsShape* mTempWallShape1;
	cocos2d::PhysicsShape* mTempWallShape2;
	
	void updateBoxDirection();
	void updateTempPoint(const cocos2d::Vec2& point);
	void removeTempPoint();
	
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	bool onContactBegin(const cocos2d::PhysicsContact& contact);
	void onComeToForeground();
	void onComeToBackground();
	
	void setParent(Node* child) override;
	
	void update(float dt);
	void updateSlow(float dt);
	
	void initPools();
};

#endif // __GAMESCENE_H__
