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

using namespace cocos2d;

class HelloWorldLayer : public CCLayer {
    
public:
    
    float screenW;
	float screenH;
    
    CC_SYNTHESIZE_RETAIN(CCSprite *, background, Background);
    CC_SYNTHESIZE_RETAIN(Terrain *, terrain, Terrain);
    LAYER_NODE_FUNC(HelloWorldLayer);
    static CCScene * scene();
    
    bool init();
    
    ~HelloWorldLayer();
    
    void registerWithTouchDispatcher();
    bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

};

#endif
