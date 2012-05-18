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
    
    initWithFile("hero.png");
    
    world = w;
    radius = 16.0f;
    awake = false;
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
//    int screenW = size.width;
    int screenH = size.height;
    
    CCPoint startPosition = ccp(0, screenH/2+radius);
    
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    //bd.position.Set(screenW/4/PTM_RATIO, screenH/2/PTM_RATIO);
    bd.linearDamping = 0.1f;
    bd.fixedRotation = true;
    bd.position.Set(startPosition.x/PTM_RATIO, startPosition.y/PTM_RATIO);
    body = world->CreateBody(&bd);
    
    b2CircleShape shape;
    shape.m_radius = radius/PTM_RATIO;
    
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 1.0f;
    fd.restitution = 0.0f; // 0 - no bounce, 1 - perfect bounce
    fd.friction = 0;
    
    body->CreateFixture(&fd);
    
    updateNodePosition();
    sleep();
    
    return true;
}

/*
void Hero::draw() {
	glColor4f(0.25f, 0.25f, 1.0f, 1.0f);
	glLineWidth(2);
	ccDrawCircle(ccp(0,0), radius, body->GetAngle(), 16, true);
}
 */

void Hero::sleep() {
    awake = false;
    body->SetActive(false);
}

void Hero::wake() {
    awake = true;
    body->SetActive(true);
    body->ApplyLinearImpulse(b2Vec2(1,2), body->GetPosition());
}


void Hero::dive() {
    body->ApplyForce(b2Vec2(0,-40),body->GetPosition());
}


void Hero::limitVelocity() {
    const float minVelocityX = 2;
    const float minVelocityY = -40;
    b2Vec2 vel = body->GetLinearVelocity();
    if (vel.x < minVelocityX) {
        vel.x = minVelocityX;
    }
    if (vel.y < minVelocityY) {
        vel.y = minVelocityY;
    }
    
    body->SetLinearVelocity(vel);
}

void Hero::updateNodePosition() {
    //self.position = ccp(body->GetPosition().x*PTM_RATIO, body->GetPosition().y*PTM_RATIO);
    setPosition(ccp(body->GetPosition().x*PTM_RATIO, body->GetPosition().y*PTM_RATIO));
    b2Vec2 vel = body->GetLinearVelocity();
    float angle = atan2f(vel.y, vel.x);
    //self.rotation = -1 * CC_RADIANS_TO_DEGREES(angle);
    setRotation(-1 * CC_RADIANS_TO_DEGREES(angle));
}





