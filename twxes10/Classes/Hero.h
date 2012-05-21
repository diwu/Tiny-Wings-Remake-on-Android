//
//  Hero.h
//  twxes10
//
//  Created by diwwu on 5/17/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef twxes10_Hero_h
#define twxes10_Hero_h

#include "cocos2d.h"
#include "Box2D.h"

using namespace cocos2d;

class Hero : public CCNode {
    
public:
    b2World *world;
	b2Body *body;
	float radius;
    
    static Hero * heroWithWorld(b2World * w);
    bool initWithWorld(b2World * w);
    
    void sleep();
    void wake();
    void dive();
    void limitVelocity();
    void updateNodePosition();
    
    //void draw();
    
    CC_SYNTHESIZE_READONLY(bool, awake, Awake);
    CC_SYNTHESIZE_RETAIN(CCSprite *, sprite, Sprite);
    
    void createBox2DBody();
    
    ~Hero();
};

#endif
