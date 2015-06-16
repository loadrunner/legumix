#pragma once

#include "cocos2d.h"

class UILayer : public cocos2d::Layer
{
public:
	bool init(const cocos2d::Size size);
	static UILayer* create(const cocos2d::Size size);
	
	void updateScore(int score);
	void updateLife(int lives);
	void updateProgress(float progress);
private:
	cocos2d::Label* mScoreView;
	cocos2d::Label* mLifeView;
	cocos2d::Label* mProgressView;
};
