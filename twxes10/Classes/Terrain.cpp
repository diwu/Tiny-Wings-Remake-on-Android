//
//  Terrain.cpp
//  twx
//
//  Created by diwwu on 5/16/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Terrain.h"
//#define DRAW_WIREFRAME

#define PTM_RATIO 32 // pixel to metre ratio

Terrain * Terrain::terrainWithWorld(b2World* w) {
    Terrain *result = new Terrain();
    
    result->initWithWorld(w);
    result->autorelease();
    
    return result;
}
bool Terrain::initWithWorld(b2World* w) {
    
    world = w;
    //body = NULL;
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    screenW = size.width;
    screenH = size.height;
    
    //scrolling = false;
    //offsetX = 0;
    
    textureSize = 512;
    
        
    generateStripes();
    generateHillKeyPoints();
    
    generateBorderVertices();
    createBox2DBody();
    
    offsetX = 0;
    
    //scheduleUpdate();
    
    return true;
}

/*
bool Terrain::init(void) {
    
    scrolling = false;
    offsetX = 0;
    
    this->generateStripes();
    this->generateHills();
    this->scheduleUpdate();
    
    return true;
}
*/

Terrain::~Terrain() {
    stripes->release();
    stripes = NULL;
}

void Terrain::draw() {
    glBindTexture(GL_TEXTURE_2D, stripes->getTexture()->getName());
	
	glDisableClientState(GL_COLOR_ARRAY);
	
    /*
	glVertexPointer(2, GL_FLOAT, 0, hillVisibleVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, hillVisibleTexCoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nHillVisibleVertices);
	*/
    
    glColor4f(1, 1, 1, 1);
	glVertexPointer(2, GL_FLOAT, 0, hillVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, hillTexCoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nHillVertices);
    
    /*
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
	glLineWidth(3);
	glColor4ub(56, 125, 0, 255);
	glVertexPointer(2, GL_FLOAT, 0, borderVertices);
	glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)nBorderVertices);
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
    */
    
	glEnableClientState(GL_COLOR_ARRAY);
	
#ifdef DRAW_WIREFRAME
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
    
	glColor4f(1, 1, 1, 1);
	glVertexPointer(2, GL_FLOAT, 0, hillVertices);
	glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)nHillVertices);
    
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);	
#endif

}

ccColor3B Terrain::generateDarkColor() {
    const int threshold = 250;
    int r, g, b;
    while (true) {
        r = arc4random()%256;
        g = arc4random()%256;
        b = arc4random()%256;
        if (r+g+b > threshold) break;
    }
    return ccc3(r, g, b);
}

ccColor3B Terrain::generateLightColorFrom(ccColor3B c) {
    const int addon = 30;
    int r, g, b;
    r = c.r + addon;
    g = c.g + addon;
    b = c.b + addon;
    if (r > 255) r = 255;
        if (g > 255) g = 255;
            if (b > 255) b = 255;
                return ccc3(r, g, b);
}

void Terrain::generateStripes() {
	
    //const int maxStripes = 20;

    // random even number of stripes (2,4,6,etc)
    const int minStripes = 2;
    const int maxStripes = 20;
    int nStripes = arc4random()%(maxStripes-minStripes)+minStripes;
    if (nStripes%2 != 0) {
        nStripes++;
    }
    CCLog("nStripes = %d", nStripes);
    
    /*
    int textureSize = 512;
    int nStripes = 4;
	ccColor3B c1 = (ccColor3B){86,155,30};
	ccColor3B c2 = (ccColor3B){123,195,56};
     */
    ccColor3B c1 = generateDarkColor();
    ccColor3B c2 = generateLightColorFrom(c1);
	//float gradientAlpha = 0.5f;
	
    //CCRenderTexture *rt = [CCRenderTexture renderTextureWithWidth:textureSize height:textureSize];
    CCRenderTexture *rt = CCRenderTexture::renderTextureWithWidthAndHeight(textureSize, textureSize);
    //[rt beginWithClear:(float)c1.r/256.0f g:(float)c1.g/256.0f b:(float)c1.b/256.0f a:1];
    //rt->beginWithClear(c1.r/256.0f, c1.g/256.0f, c1.b/256.0f, 1);
    ccColor4F c1f = ccc4FFromccc3B(c1);
    rt->beginWithClear(c1f.r, c1f.g, c1f.b, c1f.a);
    
	// layer 1: stripes
	
    //CGPoint vertices[kMaxStripes*6];
    CCPoint vertices[maxStripes*6];
    int nVertices = 0;
    float x1 = -textureSize;
    float x2;
	float y1 = textureSize;
    float y2 = 0;
    float dx = textureSize*2 / nStripes;
    float stripeWidth = dx/2;
    for (int i=0; i<nStripes; i++) {
        x2 = x1 + textureSize;
        vertices[nVertices++] = ccp(x1, y1);
        vertices[nVertices++] = ccp(x1+stripeWidth, y1);
        vertices[nVertices++] = ccp(x2, y2);
        vertices[nVertices++] = vertices[nVertices-2];
        vertices[nVertices++] = vertices[nVertices-2];
        vertices[nVertices++] = ccp(x2+stripeWidth, y2);
        x1 += dx;
    }
    
     glDisable(GL_TEXTURE_2D);
     glDisableClientState(GL_TEXTURE_COORD_ARRAY);
     glDisableClientState(GL_COLOR_ARRAY);
	
    glColor4ub(c2.r, c2.g, c2.b, 255);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)nVertices);
	
	// layer 2: gradient
	
    float gradientAlpha = 0.7f;
	glEnableClientState(GL_COLOR_ARRAY);
	
    /*
    float borderWidth = 8.0f;
    ccColor3B borderColor = ccc3(86, 155, 30);
    ccColor4F colors[6];
     */
    ccColor4F colors[4];
    nVertices = 0;
    /*
    vertices[nVertices] = CCPointMake(0, 0);
    colors[nVertices++] = ccc4FFromccc3B(borderColor);
    vertices[nVertices] = CCPointMake(textureSize, 0);
    colors[nVertices++] = ccc4FFromccc3B(borderColor);
     */

    vertices[nVertices] = ccp(0, 0);
    colors[nVertices++] = (ccColor4F){0, 0, 0, 0};
    vertices[nVertices] = ccp(textureSize, 0);
    colors[nVertices++] = (ccColor4F){0, 0, 0, 0};
    
    vertices[nVertices] = ccp(0, textureSize);
    colors[nVertices++] = (ccColor4F){0, 0, 0, gradientAlpha};
    vertices[nVertices] = ccp(textureSize, textureSize);
    colors[nVertices++] = (ccColor4F){0, 0, 0, gradientAlpha};
	
    glVertexPointer(2, GL_FLOAT, 0, vertices);
	glColorPointer(4, GL_FLOAT, 0, colors);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);
	
    // layer 3: highlight
    float highlightWidth = textureSize/8;
    ccColor4F highlightColor = (ccColor4F){1, 1, 1, 0.3f};

    nVertices = 0;
    
    vertices[nVertices] = ccp(0, 0);
    colors[nVertices++] = highlightColor;
    vertices[nVertices] = ccp(textureSize, 0);
    colors[nVertices++] = highlightColor;
    
    vertices[nVertices] = ccp(0, highlightWidth);
    colors[nVertices++] = (ccColor4F){0, 0, 0, 0};
    vertices[nVertices] = ccp(textureSize, highlightWidth);
    colors[nVertices++] = (ccColor4F){0, 0, 0, 0};
    
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);
    
    
    glDisableClientState(GL_COLOR_ARRAY);	
    
    // layer: top border
    
    float borderWidth = 2.0f;
    ccColor4F borderColor = (ccColor4F){0, 0, 0, 0.5f};
    
    nVertices = 0;
    
    vertices[nVertices] = ccp(0, borderWidth/2);
    colors[nVertices++] = borderColor;
    vertices[nVertices] = ccp(textureSize, borderWidth/2);
    colors[nVertices++] = borderColor;
    
    glLineWidth(borderWidth);
    glColor4f(borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)nVertices);
    
	// layer 4: noise
    
    
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
	
	//CCSprite *s = [CCSprite spriteWithFile:@"noise.png"];
    CCSprite *s = CCSprite::spriteWithFile("noise.png");

	//[s setBlendFunc:(ccBlendFunc){GL_DST_COLOR, GL_ZERO}];
	s->setBlendFunc((ccBlendFunc){GL_DST_COLOR, GL_ZERO});
    //s.position = ccp(textureSize/2, textureSize/2);
    s->setPosition(ccp(textureSize/2, textureSize/2));
    //+    s.scale = (float)textureSize/512.0f;
    s->setScale((float)textureSize/512.0f);
    glColor4f(1,1,1,1);
	//[s visit];
    s->visit();
    
    //[rt end];
    rt->end();
	
	//self.stripes = [CCSprite spriteWithTexture:rt.sprite.texture];
    setStripes(CCSprite::spriteWithTexture(rt->getSprite()->getTexture()));
	ccTexParams tp = {GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_CLAMP_TO_EDGE};
	//[stripes_.texture setTexParameters:&tp];
    getStripes()->getTexture()->setTexParameters(&tp);
}

void Terrain::generateHillKeyPoints() {
	
    
    // random key points
    nHillKeyPoints = 0;

    float x, y, dx, dy, ny;
    
    x = -screenW/4;
    y = screenH*3/4;
    hillKeyPoints[nHillKeyPoints++] = CCPointMake(x, y);

    // right under the starting point
    x = 0;
    y = screenH/2;
    hillKeyPoints[nHillKeyPoints++] = CCPointMake(x, y);
    
    srand(1);
    /*
    nHillKeyPoints = kMaxHillKeyPoints;
	float minDX = 160;
	float minDY = 60;
    float x = -minDX;
	float y = screenH/2-minDY;
	float dy, ny;
    float sign = 1; // +1 - going up, -1 - going  down
     */
    int minDX = 160, rangeDX = 80;
    int minDY = 60,  rangeDY = 60;
    float sign = -1; // +1 - going up, -1 - going  down
    float paddingTop = 20;
    float paddingBottom = 20;
    /*
    for (int i=0; i<nHillKeyPoints; i++) {
        hillKeyPoints[i] = CCPointMake(x, y);
		if (i == 0) {
			x = 0;
			y = screenH/2;
		} else {
			x += rand()%80+minDX;
			while(true) {
				dy = rand()%40+minDY;
				ny = y + dy*sign;
				if(ny < 320-paddingTop && ny > paddingBottom) break;
			}
			y = ny;
		}
        sign *= -1;
    }
     */
    while (nHillKeyPoints < kMaxHillKeyPoints - 1) {
        dx = rand()%rangeDX+minDX;
        x += dx;
        /*
        while(true) {
            dy = rand()%rangeDY+minDY;
            ny = y + dy*sign;
            if(ny < screenH-paddingTop && ny > paddingBottom) break;
        }
         */
        dy = rand()%rangeDY+minDY;
        ny = y + dy*sign;
        if(ny > screenH-paddingTop) ny = screenH-paddingTop;
        if(ny < paddingBottom) ny = paddingTop;
        y = ny;
        sign *= -1;
        hillKeyPoints[nHillKeyPoints++] = CCPointMake(x, y);
    }
    
    // cliff
    x += minDX+rangeDX;
    y = 0;
    hillKeyPoints[nHillKeyPoints++] = ccp(x, y);
    
    fromKeyPointI = 0;
    toKeyPointI = 0;
	
	//resetHillVertices();
}

void Terrain::generateBorderVertices() {
    
    nBorderVertices = 0;
    CCPoint p0, p1, pt0, pt1;
    p0 = hillKeyPoints[0];
    for (int i=1; i<nHillKeyPoints; i++) {
        p1 = hillKeyPoints[i];
        
        int hSegments = floorf((p1.x-p0.x)/kHillSegmentWidth);
        float dx = (p1.x - p0.x) / hSegments;
        float da = M_PI / hSegments;
        float ymid = (p0.y + p1.y) / 2;
        float ampl = (p0.y - p1.y) / 2;
        pt0 = p0;
        borderVertices[nBorderVertices++] = pt0;
        for (int j=1; j<hSegments+1; j++) {
            pt1.x = p0.x + j*dx;
            pt1.y = ymid + ampl * cosf(da*j);
            borderVertices[nBorderVertices++] = pt1;
            pt0 = pt1;
        }
        
        p0 = p1;
    }
    //    NSLog(@"nBorderVertices = %d", nBorderVertices);
}

void Terrain::createBox2DBody() {
    
    b2BodyDef bd;
    bd.position.Set(0, 0);
    
    body = world->CreateBody(&bd);
    
    b2Vec2 b2vertices[kMaxBorderVertices];
    int nVertices = 0;
    for (int i=0; i<nBorderVertices; i++) {
        b2vertices[nVertices++].Set(borderVertices[i].x/PTM_RATIO,borderVertices[i].y/PTM_RATIO);
    }
    b2vertices[nVertices++].Set(borderVertices[nBorderVertices-1].x/PTM_RATIO,0);
    b2vertices[nVertices++].Set(-screenW/4,0);
    
    b2LoopShape shape;
    shape.Create(b2vertices, nVertices);
    body->CreateFixture(&shape, 0);

}

void Terrain::resetHillVertices() {
	
    static int prevFromKeyPointI = -1;
    static int prevToKeyPointI = -1;
    
    // key points interval for drawing
    float leftSideX = offsetX-screenW/8/getScale();
    float rightSideX = offsetX+screenW*7/8/getScale();
    while (hillKeyPoints[fromKeyPointI+1].x < leftSideX) {
        fromKeyPointI++;
    }
    while (hillKeyPoints[toKeyPointI].x < rightSideX) {
        toKeyPointI++;
    }
    
    if (prevFromKeyPointI != fromKeyPointI || prevToKeyPointI != toKeyPointI) {
        
        //NSLog(@"from %d: %f, %f",fromKeyPointI,hillKeyPoints[fromKeyPointI].x,hillKeyPoints[fromKeyPointI].y);
        //NSLog(@"to   %d: %f, %f",toKeyPointI,hillKeyPoints[toKeyPointI].x,hillKeyPoints[toKeyPointI].y);
        
        // vertices for visible area
        nHillVertices = 0;
        //nBorderVertices = 0;
        CCPoint p0, p1, pt0, pt1;
        p0 = hillKeyPoints[fromKeyPointI];
        for (int i=fromKeyPointI+1; i<toKeyPointI+1; i++) {
            p1 = hillKeyPoints[i];
            
            // triangle strip between p0 and p1
            int hSegments = floorf((p1.x-p0.x)/kHillSegmentWidth);
            int vSegments = 1;
            float dx = (p1.x - p0.x) / hSegments;
            float da = M_PI / hSegments;
            float ymid = (p0.y + p1.y) / 2;
            float ampl = (p0.y - p1.y) / 2;
            pt0 = p0;
            //borderVertices[nBorderVertices++] = pt0;
            for (int j=1; j<hSegments+1; j++) {
                pt1.x = p0.x + j*dx;
                pt1.y = ymid + ampl * cosf(da*j);
                //borderVertices[nBorderVertices++] = pt1;
                for (int k=0; k<vSegments+1; k++) {
                    hillVertices[nHillVertices] = ccp(pt0.x, pt0.y-(float)textureSize/vSegments*k);
                    hillTexCoords[nHillVertices++] = ccp(pt0.x/(float)textureSize, (float)(k)/vSegments);
                    hillVertices[nHillVertices] = ccp(pt1.x, pt1.y-(float)textureSize/vSegments*k);
                    hillTexCoords[nHillVertices++] = ccp(pt1.x/(float)textureSize, (float)(k)/vSegments);
                }
                pt0 = pt1;
            }
            
            p0 = p1;
        }
        
        //NSLog(@"nHillVertices = %d",nHillVertices);
        //NSLog(@"nBorderVertices = %d",nBorderVertices);
        
        prevFromKeyPointI = fromKeyPointI;
        prevToKeyPointI = toKeyPointI;
        
        //resetBox2DBody();
    }
}

/*
void Terrain::offsetChanged() {
    setPosition(CCPointMake(-offsetX, 0));
	updateHillVertices();
}
 */

/*
void Terrain::resetBox2DBody() {
    
	if(body) {
		world->DestroyBody(body);
	}
	
	b2BodyDef bd;
	bd.position.Set(0, 0);
	
	body = world->CreateBody(&bd);
	
	//b2PolygonShape shape;
    b2EdgeShape shape;
    
	b2Vec2 p1, p2;
	for (int i=0; i<nBorderVertices-1; i++) {
		p1 = b2Vec2(borderVertices[i].x/PTM_RATIO,borderVertices[i].y/PTM_RATIO);
		p2 = b2Vec2(borderVertices[i+1].x/PTM_RATIO,borderVertices[i+1].y/PTM_RATIO);
		shape.Set(p1, p2);
		body->CreateFixture(&shape, 0);
	}
}
 */

void Terrain::setOffsetX(float newOffsetX) {
    //CCLog(" - customized set offset x...");
	float minOffsetX = 0;
	float maxOffsetX = hillKeyPoints[nHillKeyPoints-1].x-screenW;
	if (newOffsetX < minOffsetX) {
		newOffsetX = minOffsetX;
	}
	if (newOffsetX > maxOffsetX) {
		newOffsetX = maxOffsetX;
	}
    static bool firstTime = true;
    if (offsetX != newOffsetX || firstTime) {
        firstTime = false;
        offsetX = newOffsetX;
        setPosition(CCPointMake(screenW/8-offsetX*getScale(), 0));
        resetHillVertices();
    }
}

/*
void Terrain::toggleScrolling() {
    scrolling = !scrolling;
}
 */
