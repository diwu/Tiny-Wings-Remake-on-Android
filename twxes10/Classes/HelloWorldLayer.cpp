//
//  HelloWorldLayer.cpp
//  twxes10
//
//  Created by diwwu on 5/17/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "HelloWorldLayer.h"

CCScene * HelloWorldLayer::scene() {
    CCScene *scene = CCScene::node();
    
    HelloWorldLayer *layer = HelloWorldLayer::node();
    
    scene->addChild(layer);
    
    return scene;
}


bool HelloWorldLayer::init() {
    
    CCLayer::init();
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    screenW = size.width;
    screenH = size.height;
    
    setBackground(CCSprite::spriteWithFile("background.png"));
    getBackground()->setPosition(ccp(screenW/2, screenH/2));
    
    getBackground()->setScaleX(screenW/getBackground()->getTextureRect().size.width);
    getBackground()->setScaleY(screenH/getBackground()->getTextureRect().size.height);
    
    //CCLog("sw = %f, sh = %f, bw = %f, bh = %f", screenW, screenH, getBackground()->getTextureRect().size.width, getBackground()->getTextureRect().size.height);
    
    addChild(getBackground());
    
    setTerrain(new Terrain());
    getTerrain()->init();
    
    addChild(getTerrain());
    
    setIsTouchEnabled(true);

    return true;
}

HelloWorldLayer::~HelloWorldLayer() {
    
    getBackground()->release();
    getTerrain()->release();
    
    setBackground(NULL);
    setTerrain(NULL);
    
}

void HelloWorldLayer::registerWithTouchDispatcher() {
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
}

bool HelloWorldLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint location = pTouch->locationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    getTerrain()->toggleScrolling();
    
    return true;
}

