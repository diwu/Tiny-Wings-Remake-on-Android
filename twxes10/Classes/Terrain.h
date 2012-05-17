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
#define kMaxHillVertices 2000
#define kHillSegmentWidth 3
#define kMaxBorderVertices 400

using namespace cocos2d;

class Terrain : public cocos2d::CCNode {
    
    CCPoint hillKeyPoints[kMaxHillKeyPoints];
	int nHillKeyPoints;
    int fromKeyPointI;
	int toKeyPointI;
	CCPoint hillVertices[kMaxHillVertices];
	CCPoint hillTexCoords[kMaxHillVertices];
	int nHillVertices;
    
    CCPoint borderVertices[kMaxBorderVertices];
	int nBorderVertices;
	//CCSprite *stripes_;
	bool scrolling;
	//float offsetX;
    
public:
    CC_SYNTHESIZE_RETAIN(CCSprite *, stripes, Stripes);
    CC_SYNTHESIZE_READONLY(float, offsetX, OffsetX);
    bool init(void);
    ~Terrain();
    void draw(void);
    void toggleScrolling();
    void update(ccTime dt);
    
private:
    void generateHills();
    void updateHillVertices();
    void offsetChanged();
    void generateStripes();
};

#endif
