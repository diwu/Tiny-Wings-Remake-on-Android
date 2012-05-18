//
//  GameLayer.cpp
//  twxes10
//
//  Created by diwwu on 5/17/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "GameLayer.h"

void GameLayer::createBox2DWorld() {
	
	b2Vec2 gravity;
	gravity.Set(0.0f, -7.0f);
	
	world = new b2World(gravity);
	world->SetContinuousPhysics(true);
	
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

CCSprite * GameLayer::generateBackground() {
	
	//CCSize textureSize = CCSizeMake(screenW, screenH);
    int textureSize = 512;
    
	ccColor3B c = (ccColor3B){140,205,221};
	
    //CCRenderTexture *rt = [CCRenderTexture renderTextureWithWidth:textureSize.width height:textureSize.height];
    CCRenderTexture * rt = CCRenderTexture::renderTextureWithWidthAndHeight(textureSize, textureSize);
    //[rt beginWithClear:(float)c.r/256.0f g:(float)c.g/256.0f b:(float)c.b/256.0f a:1];
    rt->beginWithClear((float)c.r/256.0f, (float)c.g/256.0f, (float)c.b/256.0f, 1);
    
	// layer 1: gradient
    
	float gradientAlpha = 0.5f;
    
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
    CCPoint vertices[4];
	ccColor4F colors[4];
    int nVertices = 0;
	
	vertices[nVertices] = CCPointMake(0, 0);
	colors[nVertices++] = (ccColor4F){0, 0, 0, gradientAlpha};
	vertices[nVertices] = CCPointMake(textureSize, 0);
	colors[nVertices++] = (ccColor4F){0, 0, 0, gradientAlpha};
	vertices[nVertices] = CCPointMake(0, textureSize/2);
	colors[nVertices++] = (ccColor4F){0, 0, 0, 0};
	vertices[nVertices] = CCPointMake(textureSize, textureSize/2);
	colors[nVertices++] = (ccColor4F){0, 0, 0, 0};
	
    glVertexPointer(2, GL_FLOAT, 0, vertices);
	glColorPointer(4, GL_FLOAT, 0, colors);
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
    glColor4f(1,1,1,1);
	//[s visit];
	s->visit();
    //[rt end];
    rt->end();
	
	//return [CCSprite spriteWithTexture:rt.sprite.texture];
    return CCSprite::spriteWithTexture(rt->getSprite()->getTexture());
}

bool GameLayer::init() {
    
    CCLayer::init();
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    screenW = size.width;
    screenH = size.height;
    
    createBox2DWorld();
    
    //setBackground(CCSprite::spriteWithFile("background.png"));
    setBackground(generateBackground());
    getBackground()->setPosition(ccp(screenW/2, screenH/2));
    
    getBackground()->setScaleX(screenW/getBackground()->getTextureRect().size.width);
    getBackground()->setScaleY(screenH/getBackground()->getTextureRect().size.height);
    
    //CCLog("sw = %f, sh = %f, bw = %f, bh = %f", screenW, screenH, getBackground()->getTextureRect().size.width, getBackground()->getTextureRect().size.height);
    
    ccTexParams tp = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
    getBackground()->getTexture()->setTexParameters(&tp);
    addChild(getBackground());
    
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
    
    int32 velocityIterations = 2;
    int32 positionIterations = 1;
    world->Step(dt, velocityIterations, positionIterations);
    world->ClearForces();
    
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
