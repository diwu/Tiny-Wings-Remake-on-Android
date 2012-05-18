//
//  HelloWorldLayer.h
//  twxes10
//
//  Created by diwwu on 5/17/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef twxes10_HelloWorldLayer_h
#define twxes10_HelloWorldLayer_h

#include "Terrain.h"
#include "cocos2d.h"
#include "Hero.h"
#include "Box2D.h"

using namespace cocos2d;

class HelloWorldLayer : public CCLayer {
    
public:
    
    float screenW;
	float screenH;
    
    b2World *world;    
    bool tapDown;
    CC_SYNTHESIZE_RETAIN(Hero *, hero, Hero);
    
    CC_SYNTHESIZE_RETAIN(CCSprite *, background, Background);
    CC_SYNTHESIZE_RETAIN(Terrain *, terrain, Terrain);
    LAYER_NODE_FUNC(HelloWorldLayer);
    static CCScene * scene();
    
    bool init();
    
    ~HelloWorldLayer();
    
    void registerWithTouchDispatcher();
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

    CCSprite * generateBackground();
    
    void update(ccTime dt);
    
    void createBox2DWorld();
};

#endif
