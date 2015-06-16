#include "UILayer.h"

UILayer* UILayer::create(const cocos2d::Size size)
{
	UILayer *pRet = new(std::nothrow) UILayer();
	if (pRet && pRet->init(size))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool UILayer::init(const cocos2d::Size size)
{
	if (!cocos2d::Layer::init())
		return false;
	
	ignoreAnchorPointForPosition(false);
	setContentSize(size);
	
	mScoreView = cocos2d::Label::createWithTTF("", "fonts/default.ttf", 10);
	mScoreView->setPosition(cocos2d::Vec2(2, getContentSize().height - 2));
	mScoreView->setAnchorPoint(cocos2d::Vec2(0, 1));
	addChild(mScoreView);
	
	mLifeView = cocos2d::Label::createWithTTF("", "fonts/default.ttf", 10);
	mLifeView->setPosition(cocos2d::Vec2(getContentSize().width * 0.5f, getContentSize().height - 2));
	mLifeView->setAnchorPoint(cocos2d::Vec2(0.5f, 1));
	addChild(mLifeView);
	
	
	mProgressView = cocos2d::Label::createWithTTF("0m", "fonts/default.ttf", 10);
	mProgressView->setColor(cocos2d::Color3B::GREEN);
	mProgressView->setPosition(cocos2d::Vec2(getContentSize().width - 2, getContentSize().height - 2));
	mProgressView->setAnchorPoint(cocos2d::Vec2(1, 1));
	addChild(mProgressView);
	
	return true;
}

void UILayer::updateScore(int score)
{
	mScoreView->setString(cocos2d::__String::createWithFormat("%d", score)->_string);
}

void UILayer::updateLife(int lives)
{
	mLifeView->setString(cocos2d::__String::createWithFormat("%d", lives)->_string);
}

void UILayer::updateProgress(float progress)
{
	mProgressView->setString(cocos2d::__String::createWithFormat("%.1fm", progress)->_string);
}
