#include "Helpers.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include <android/log.h>
#include "platform/android/jni/JniHelper.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <AudioToolbox/AudioServices.h>
#endif

namespace helpers {

void Vibrator::vibrate(int time)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "vibrate", "(I)V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID, time);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
#endif
}
void Vibrator::cancelVibrate()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "cancelVibrate", "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
#endif
}

void URL::open(const char* url)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "openUrl", "(Ljava/lang/String;)V"))
	{
		jstring arg1 = t.env->NewStringUTF(url);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, arg1);
		t.env->DeleteLocalRef(t.classID);
		t.env->DeleteLocalRef(arg1);
	}
#endif
}

const char SAFE[256] =
{
	/*	  0 1 2 3  4 5 6 7  8 9 A B  C D E F */
	/* 0 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	/* 1 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	/* 2 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	/* 3 */ 1,1,1,1, 1,1,1,1, 1,1,0,0, 0,0,0,0,
	
	/* 4 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
	/* 5 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,
	/* 6 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
	/* 7 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,
	
	/* 8 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	/* 9 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	/* A */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	/* B */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	
	/* C */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	/* D */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	/* E */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	/* F */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
};

const std::string URL::encode(const std::string & sSrc)
{
	const char DEC2HEX[16 + 1] = "0123456789ABCDEF";
	const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
	const int SRC_LEN = sSrc.length();
	unsigned char * const pStart = new unsigned char[SRC_LEN * 3];
	unsigned char * pEnd = pStart;
	const unsigned char * const SRC_END = pSrc + SRC_LEN;
	
	for (; pSrc < SRC_END; ++pSrc)
	{
		if (SAFE[*pSrc]) 
			*pEnd++ = *pSrc;
		else
		{
			// escape this char
			*pEnd++ = '%';
			*pEnd++ = DEC2HEX[*pSrc >> 4];
			*pEnd++ = DEC2HEX[*pSrc & 0x0F];
		}
	}
	
	std::string sResult((char *)pStart, (char *)pEnd);
	delete [] pStart;
	return sResult;
}

const std::string Locale::getCountry()
{
	std::string country("USA");
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getCurrentCountry", "()Ljava/lang/String;"))
	{
		jstring str = (jstring) t.env->CallStaticObjectMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
		country = cocos2d::JniHelper::jstring2string(str);
		t.env->DeleteLocalRef(str);
	}
#endif
	
	return country;
}

cocos2d::Vec2 Custom::translatePositionToCenter(const cocos2d::Vec2& pos, const cocos2d::Size& containerSize)
{
	return cocos2d::Vec2(pos.x - containerSize.width/2, pos.y - containerSize.height/2);
}

cocos2d::Vec2 Custom::getRealPosition(cocos2d::Node* node)
{
	return cocos2d::Vec2(
			node->getPositionX() - node->getContentSize().width * node->getAnchorPoint().x,
			node->getPositionY() - node->getContentSize().height * node->getAnchorPoint().y);
}

cocos2d::Vec2 Custom::normalizeVelocity(cocos2d::Vec2 v)
{
	int signX = v.x > 0 ? 1 : -1;
	int signY = v.y > 0 ? 1 : -1;
	
	if (std::abs(v.x) != std::abs(v.y))
	{
		int max = std::max(std::abs(v.x), std::abs(v.y));
		v.x = max * signX;
		v.y = max * signY;
	}
	
	return v;
}

bool Custom::containsPoint(cocos2d::Node* node, const cocos2d::Vec2& point)
{
	cocos2d::Vec2 local = node->convertToNodeSpace(point);
	cocos2d::Rect r = node->getBoundingBox();
	r.origin = cocos2d::Vec2::ZERO;
	return r.containsPoint(local);
}

float Custom::getNormalizedAngle(const cocos2d::Vec2& v1, const cocos2d::Vec2& v2)
{
	float dX = v2.x - v1.x;
	float dY = v2.y - v1.y;
	
	float angle = -CC_RADIANS_TO_DEGREES(atan2f(dY, dX));
	if (angle < -90)
		angle += 360;
	angle -= 90;
	
	float sign = angle > 0 ? 1 : -1;
	
	return sign * std::min(75.0f, std::max(15.0f, std::abs(angle)));
}

cocos2d::Node* Custom::getNodeByShapeTag(const cocos2d::PhysicsContact& contact, int tag)
{
	cocos2d::PhysicsShape* shape = nullptr;
	
	if (contact.getShapeA()->getTag() == tag)
		shape = contact.getShapeA();
	else if (contact.getShapeB()->getTag() == tag)
		shape = contact.getShapeB();
	
	if (shape)
		return shape->getBody()->getNode();
	
	return nullptr;
}

bool Custom::isContactBetweenAB(const cocos2d::PhysicsContact& contact, int tagA, int tagB)
{
	return ((contact.getShapeA()->getTag() & tagA) == tagA && (contact.getShapeB()->getTag() & tagB) == tagB)
	    || ((contact.getShapeA()->getTag() & tagB) == tagB  && (contact.getShapeB()->getTag() & tagA) == tagA);
}

cocos2d::Node* Custom::getNodeByBodyTag(const cocos2d::PhysicsContact& contact, int tag)
{
	cocos2d::PhysicsBody* body = nullptr;
	
	if (contact.getShapeA()->getBody()->getTag() == tag)
		body = contact.getShapeA()->getBody();
	else if (contact.getShapeB()->getBody()->getTag() == tag)
		body = contact.getShapeB()->getBody();
	
	if (body)
		return body->getNode();
	
	return nullptr;
}

cocos2d::PhysicsShape* PhysicsCollisions::getShape(const cocos2d::PhysicsContact& contact, int tag)
{
	if ((contact.getShapeA()->getTag() & tag) == tag)
		return contact.getShapeA();
	
	if ((contact.getShapeB()->getTag() & tag) == tag)
		return contact.getShapeB();
	
	return nullptr;
}

cocos2d::PhysicsShape* PhysicsCollisions::getShapeContactedBy(const cocos2d::PhysicsContact& contact, int tag)
{
	if (contact.getShapeA()->getTag() == tag)
		return contact.getShapeB();
	
	if (contact.getShapeB()->getTag() == tag)
		return contact.getShapeA();
	
	return nullptr;
}

}; // namespace helpers {

