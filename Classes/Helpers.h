#ifndef __HELPERS_H__
#define __HELPERS_H__

#include "cocos2d.h"

namespace helpers {

#define CLASS_NAME "org/cocos2dx/lib/Cocos2dxHelper"

class Vibrator
{
public:
	static void vibrate(int time);
	static void cancelVibrate();
};

class URL
{
public:
	static void open(const char* url);
	static const std::string encode(const std::string& src);
};

class Locale
{
public:
	static const std::string getCountry();
};

class Custom
{
public:
	static cocos2d::Vec2 translatePositionToCenter(const cocos2d::Vec2& pos, const cocos2d::Size& containerSize);
	static cocos2d::Vec2 getRealPosition(cocos2d::Node* node);
	static cocos2d::Vec2 normalizeVelocity(cocos2d::Vec2 v);
	static bool containsPoint(cocos2d::Node* node, const cocos2d::Vec2& point);
	static float getNormalizedAngle(const cocos2d::Vec2& v1, const cocos2d::Vec2& v2);
	static cocos2d::Node* getNodeByShapeTag(const cocos2d::PhysicsContact& contact, int tag);
	static cocos2d::Node* getNodeByBodyTag(const cocos2d::PhysicsContact& contact, int tag);
	static bool isContactBetweenAB(const cocos2d::PhysicsContact& contact, int tagA, int tagB);
};

class PhysicsCollisions
{
public:
	static cocos2d::PhysicsShape* getShape(const cocos2d::PhysicsContact& contact, int tag);
	static cocos2d::PhysicsShape* getShapeContactedBy(const cocos2d::PhysicsContact& contact, int tag);
};

}; // namespace helpers {
#endif // __HELPERS_H__
