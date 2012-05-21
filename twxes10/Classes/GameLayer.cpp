//
//  GameLayer.cpp
//  twxes10
//
//  Created by diwwu on 5/17/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "GameLayer.h"

void GameLayer::draw() {
    
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
    
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);	
}

ccColor3B GameLayer::generateDarkColor() {
    const int maxValue = 200;
    const int minValue = 100;
    const int maxSum = 350;
    int r, g, b;
    while (true) {
        r = arc4random()%(maxValue-minValue)+minValue;
        g = arc4random()%(maxValue-minValue)+minValue;
        b = arc4random()%(maxValue-minValue)+minValue;
        if (r+g+b > maxSum) break;
    }
    return ccc3(r, g, b);
}

void GameLayer::createBox2DWorld() {
	
	b2Vec2 gravity;
	gravity.Set(0.0f, -9.8f);
	
	world = new b2World(gravity, false);
	//world->SetContinuousPhysics(true);
	
    //	debugDraw = new GLESDebugDraw(PTM_RATIO);
    //	world->SetDebugDraw(debugDraw);
	
    //	uint32 flags = 0;
    //	flags += b2DebugDraw::e_shapeBit;
    //	debugDraw->SetFlags(flags);
	
}

CCScene * GameLayer::scene() {
    CCScene *scene = CCScene::node();
    
    GameLayer *layer = GameLayer::node();
    
    scene->addChild(layer);
    
    return scene;
}

void GameLayer::generateBackground() {
	
	//CCSize textureSize = CCSizeMake(screenW, screenH);
    int textureSize = 512;
    
	ccColor3B c = (ccColor3B){140,205,221};
	//ccColor3B c = generateDarkColor();
    ccColor4F cf = ccc4FFromccc3B(c);
    
    //CCRenderTexture *rt = [CCRenderTexture renderTextureWithWidth:textureSize.width height:textureSize.height];
    CCRenderTexture * rt = CCRenderTexture::renderTextureWithWidthAndHeight(textureSize, textureSize);
    //[rt beginWithClear:(float)c.r/256.0f g:(float)c.g/256.0f b:(float)c.b/256.0f a:1];
    rt->beginWithClear(cf.r, cf.g, cf.b, cf.a);
    
	// layer 1: gradient
    
	float gradientAlpha = 0.25f;
    
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
    CCPoint vertices[4];
	ccColor4F colors[4];
    int nVertices = 0;
	
    vertices[nVertices] = ccp(0, 0);
    colors[nVertices++] = (ccColor4F){1, 1, 1, 0};
    vertices[nVertices] = ccp(textureSize, 0);
    colors[nVertices++] = (ccColor4F){1, 1, 1, 0};
    vertices[nVertices] = ccp(0, textureSize);
    colors[nVertices++] = (ccColor4F){1, 1, 1, gradientAlpha};
    vertices[nVertices] = ccp(textureSize, textureSize);
    colors[nVertices++] = (ccColor4F){1, 1, 1, gradientAlpha};
	
    glVertexPointer(2, GL_FLOAT, 0, vertices);
	glColorPointer(4, GL_FLOAT, 0, colors);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);	
    
	// layer 2: noise
	
	//CCSprite *s = [CCSprite spriteWithFile:@"noise.png"];
    CCSprite *s = CCSprite::spriteWithFile("noise.png");
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //s->setScaleX(winSize.width/s->getContentSize().width);
    //s->setScaleY(winSize.height/s->getContentSize().height);

	//[s setBlendFunc:(ccBlendFunc){GL_DST_COLOR, GL_ZERO}];
    s->setBlendFunc((ccBlendFunc){GL_DST_COLOR, GL_ZERO});
	//s.position = ccp(textureSize.width/2, textureSize.height/2);
    s->setPosition(ccp(textureSize/2, textureSize/2));
    //s.scale = (float)textureSize/512.0f;
    s->setScale((float)textureSize/512.0f);
    glColor4f(1,1,1,1);
	//[s visit];
	s->visit();
    //[rt end];
    rt->end();
    
    //self.background = [CCSprite spriteWithTexture:rt.sprite.texture];
    setBackground(CCSprite::spriteWithTexture(rt->getSprite()->getTexture()));
    ccTexParams tp = {GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT};
    //[_background.texture setTexParameters:&tp];
    getBackground()->getTexture()->setTexParameters(&tp);
    //_background.position = ccp(screenW/2,screenH/2);
    getBackground()->setPosition(ccp(screenW/2,screenH/2));
    //    _background.scale = 0.5f;
	
	//return [CCSprite spriteWithTexture:rt.sprite.texture];
    //return CCSprite::spriteWithTexture(rt->getSprite()->getTexture());
    
    addChild(getBackground());
}

bool GameLayer::init() {
    
    CCLayer::init();
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    screenW = size.width;
    screenH = size.height;
    
    createBox2DWorld();
    
    //setBackground(CCSprite::spriteWithFile("background.png"));
    //setBackground(generateBackground());
    //getBackground()->setPosition(ccp(screenW/2, screenH/2));
    
    //getBackground()->setScaleX(screenW/getBackground()->getTextureRect().size.width);
    //getBackground()->setScaleY(screenH/getBackground()->getTextureRect().size.height);
    
    //CCLog("sw = %f, sh = %f, bw = %f, bh = %f", screenW, screenH, getBackground()->getTextureRect().size.width, getBackground()->getTextureRect().size.height);
    
    //ccTexParams tp = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
    //getBackground()->getTexture()->setTexParameters(&tp);
    //addChild(getBackground());
    
    generateBackground();
    
    setTerrain(Terrain::terrainWithWorld(world));
    
    addChild(getTerrain());
    
    setHero(Hero::heroWithWorld(world));
    getTerrain()->addChild(getHero());
    
    setIsTouchEnabled(true);
    
    tapDown = false;
    
    scheduleUpdate();

    return true;
}

GameLayer::~GameLayer() {
    
    delete world;
    world = NULL;
    
    getBackground()->release();
    getTerrain()->release();
    
    setBackground(NULL);
    setTerrain(NULL);
    
}

void GameLayer::registerWithTouchDispatcher() {
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint location = pTouch->locationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    //getTerrain()->toggleScrolling();
    tapDown = true;
    
    return true;
}

void GameLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
    CCPoint location = pTouch->locationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    tapDown = false;
}

void GameLayer::update(ccTime dt) {
    
    /*
    if (tapDown) {
		hero->run();
	} else {
		hero->walk();
	}
	
	int32 velocityIterations = 8;
	int32 positionIterations = 1;
	
	world->Step(dt, velocityIterations, positionIterations);
	
	hero->updatePosition();
     */
    
    if (tapDown) {
        if ( ! hero->getAwake()) {
            hero->wake();
            tapDown = false;
        } else {
            hero->dive();
        }
    }
    
    hero->limitVelocity();
    
	//terrain->offsetX = hero->position.x - screenW/4;
    //terrain->setOffsetX(hero->getPosition().x - screenW/4);
    
    int32 velocityIterations = 8;
    int32 positionIterations = 3;
    world->Step(dt, velocityIterations, positionIterations);
    //world->ClearForces();
    
    hero->updateNodePosition();
    
    float scale = (screenH*4/5) / hero->getPosition().y;
    if (scale > 1) scale = 1;
    terrain->setScale(scale);
    
    //terrain_.offsetX = hero_.position.x;
    terrain->setOffsetX(hero->getPosition().x);
    
    CCSize size = getBackground()->getTextureRect().size;
    //background_.textureRect = CGRectMake(terrain_.offsetX*0.2f, 0, size.width, size.height);
    getBackground()->setTextureRect(CCRectMake(getTerrain()->getOffsetX()*0.2f, 0, size.width, size.height));
}
