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
#include "Box2D.h"

#define kMaxHillKeyPoints 200
#define kMaxHillVertices 1000
#define kHillSegmentWidth 10
#define kMaxBorderVertices 5000

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
    
    b2World *world;
    b2Body *body;
    int screenW;
    int screenH;
    
    int textureSize;
    
protected: float offsetX;
public: virtual float getOffsetX(void) const { return offsetX; }
public: virtual void setOffsetX(float var);
    
public:
    CC_SYNTHESIZE_RETAIN(CCSprite *, stripes, Stripes);
    //CC_SYNTHESIZE(float, offsetX, OffsetX);
    
    //bool init(void);
    ~Terrain();
    void draw(void);
    //void toggleScrolling();
    //void update(ccTime dt);
    
    static Terrain * terrainWithWorld(b2World* w);
    bool initWithWorld(b2World* w);
    
private:
    void generateHillKeyPoints();
    void resetHillVertices();
    //void resetBox2DBody();
    void generateStripes();
    ccColor3B generateDarkColor();
    ccColor3B generateLightColorFrom(ccColor3B c);
    
    void generateBorderVertices();
    void createBox2DBody();

};

#endif
