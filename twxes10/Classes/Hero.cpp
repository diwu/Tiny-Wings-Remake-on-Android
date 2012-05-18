//
//  Hero.cpp
//  twxes10
//
//  Created by diwwu on 5/17/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Hero.h"

#define PTM_RATIO 32 // pixel to metre ratio

Hero * Hero::heroWithWorld(b2World * w) {
    Hero *result = new Hero();
    result->initWithWorld(w);
    result->autorelease();
    return result;
}

bool Hero::initWithWorld(b2World * w) {
    world = w;
    radius = 16.0f;
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    int screenW = size.width;
    int screenH = size.height;
    
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(screenW/4/PTM_RATIO, screenH/2/PTM_RATIO);
    body = world->CreateBody(&bd);
    
    b2CircleShape shape;
    shape.m_radius = radius/PTM_RATIO;
    
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 1.0f;
    fd.restitution = 0; // 0 - no bounce, 1 - perfect bounce
    fd.friction = 1000.0f;
    
    body->CreateFixture(&fd);
    
    return true;
}

void Hero::draw() {
	glColor4f(0.25f, 0.25f, 1.0f, 1.0f);
	glLineWidth(2);
	ccDrawCircle(ccp(0,0), radius, body->GetAngle(), 16, true);
}

void Hero::updatePosition() {
    setPosition(ccp(body->GetPosition().x*PTM_RATIO, body->GetPosition().y*PTM_RATIO));
    //setPosition(ccp(body->GetPosition().x*32, body->GetPosition().y*32));

}

void Hero::walk() {
	body->ApplyTorque(-1);
}

void Hero::run() {
    body->ApplyTorque(-7);
}





