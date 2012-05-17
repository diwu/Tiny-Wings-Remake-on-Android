//
//  Terrain.h
//  twx
//
//  Created by diwwu on 5/16/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef twx_Terrain_h
#define twx_Terrain_h

#include "cocos2d.h"

#define kMaxHillKeyPoints 100
#define kMaxHillVertices 1000000
#define kMaxHillVisibleVertices 10000

using namespace cocos2d;

class Terrain : public cocos2d::CCNode {
    
    CCPoint hillKeyPoints[kMaxHillKeyPoints];
	int nHillKeyPoints;
	CCPoint hillVertices[kMaxHillVertices];
	CCPoint hillTexCoords[kMaxHillVertices];
	int nHillVertices;
	CCPoint hillVisibleVertices[kMaxHillVisibleVertices];
	CCPoint hillVisibleTexCoords[kMaxHillVisibleVertices];
	int nHillVisibleVertices;
	//CCSprite *stripes_;
	bool scrolling;
	float offsetX;
    
public:
    CC_SYNTHESIZE_RETAIN(CCSprite *, stripes, Stripes);
    bool init(void);
    ~Terrain();
    void draw(void);
    void toggleScrolling();
    void update(ccTime dt);
    
private:
    void generateHills();
    void updateHillVisibleVertices();
    void offsetChanged();
};

#endif
