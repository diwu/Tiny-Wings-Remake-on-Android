//
//  Terrain.cpp
//  twx
//
//  Created by diwwu on 5/16/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Terrain.h"

bool Terrain::init(void) {
    this->setStripes(CCSprite::spriteWithFile("stripes.png"));
    ccTexParams tp = {GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_CLAMP_TO_EDGE};
    this->getStripes()->getTexture()->setTexParameters(&tp);
    
    scrolling = false;
    offsetX = 0;
    
    this->generateHills();
    this->scheduleUpdate();
    
    return true;
}

Terrain::~Terrain() {
    stripes->release();
    stripes = NULL;
}

void Terrain::draw() {
    glBindTexture(GL_TEXTURE_2D, stripes->getTexture()->getName());
	
	glDisableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(2, GL_FLOAT, 0, hillVisibleVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, hillVisibleTexCoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nHillVisibleVertices);
	
	glEnableClientState(GL_COLOR_ARRAY);
}

void Terrain::update(ccTime dt) {
    if(scrolling) {
		const float acc = 0.05f;
		const float maxVel = 3.0f;
		static float vel = 0;
		if(vel < maxVel) {
			vel += acc;
		} else {
			vel = maxVel;
		}
		offsetX += vel;
		float maxOffsetX = hillKeyPoints[nHillKeyPoints-1].x-480;
		if(offsetX > maxOffsetX) {
			offsetX = maxOffsetX;
			scrolling = false;
		}
		offsetChanged();
	}
}

void Terrain::generateHills() {
	
	// random key points
	srand(1);
	nHillKeyPoints = kMaxHillKeyPoints;
    //	nHillKeyPoints = 5;
	float x = 0, y = 160, dy, ny;
	float sign = -1;
	float paddingTop = 100;
	float paddingBottom = 20;
	for (int i=0; i<nHillKeyPoints; i++) {
		hillKeyPoints[i] = CCPointMake(x, y);
		x += random()%40+160;
		while(true) {
			dy = random()%80+40;
			ny = y + dy*sign;
			if(ny < 320-paddingTop && ny > paddingBottom) break;
		}
		y = ny;
		sign *= -1;
	}
	
	nHillVertices = 0;
	CCPoint p0, p1, pt0, pt1;
	p0 = hillKeyPoints[0];
	for (int i=1; i<nHillKeyPoints; i++) {
		p1 = hillKeyPoints[i];
		
		// triangle strip between p0 and p1
		int hSegments = 30;
		int vSegments = 5;
		float dx = (p1.x - p0.x) / hSegments;
		float da = M_PI / hSegments;
		float ymid = (p0.y + p1.y) / 2;
		float ampl = (p0.y - p1.y) / 2;
		pt0 = p0;
		for (int j=1; j<hSegments+1; j++) {
			pt1.x = p0.x + j*dx;
			pt1.y = ymid + ampl * cosf(da*j);
			for (int k=0; k<vSegments+1; k++) {
				hillVertices[nHillVertices] = CCPointMake(pt0.x, pt0.y / vSegments * k);
				hillTexCoords[nHillVertices++] = CCPointMake(pt0.x/256.0f, 1.0f-(float)(k)/vSegments);
				hillVertices[nHillVertices] = CCPointMake(pt1.x, pt1.y / vSegments * k);
				hillTexCoords[nHillVertices++] = CCPointMake(pt1.x/256.0f, 1.0f-(float)(k)/vSegments);
			}
			pt0 = pt1;
		}
		
		p0 = p1;
	}
	
	updateHillVisibleVertices();
}

void Terrain::updateHillVisibleVertices() {
	
	nHillVisibleVertices = 0;
	
	CCPoint p;
	float padding = 20;
	for (int i=0; i<nHillVertices; i++) {
		p = hillVertices[i];
		if(p.x > offsetX-padding && p.x < offsetX+CCDirector::sharedDirector()->getWinSize().width+padding) {
			hillVisibleVertices[nHillVisibleVertices] = p;
			hillVisibleTexCoords[nHillVisibleVertices++] = hillTexCoords[i];
		}
	}
}

void Terrain::offsetChanged() {
    setPosition(CCPointMake(-offsetX, 0));
	updateHillVisibleVertices();
}

void Terrain::toggleScrolling() {
    scrolling = !scrolling;
}
