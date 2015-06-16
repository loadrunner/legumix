#pragma once

#include "cocos2d.h"

class WorldLayer : public cocos2d::LayerColor
{
public:
	static const long PHYSICS_TAG_EDGE = 1 << 4;
	static const long PHYSICS_TAG_EDGE_LEFT = PHYSICS_TAG_EDGE | 1 << 5;
	static const long PHYSICS_TAG_EDGE_RIGHT = PHYSICS_TAG_EDGE | 1 << 6;
	
	bool init();
	void update(float dt);
	CREATE_FUNC(WorldLayer);
	
	inline cocos2d::Node* getScrollContainer() { return mScrollContainer; }
private:
	cocos2d::Layer* mScrollContainer;
	cocos2d::Node* mBg1;
	cocos2d::Node* mBg2;
};
