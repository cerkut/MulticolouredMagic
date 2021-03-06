/**
 * == Simplified BSD *** MODIFIED FOR NON-COMMERCIAL USE ONLY!!! *** ==
 * Copyright (c) 2011, Cariad Interactive LTD
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 *   * Redistributions of source code must retain the above copyright notice, this list of 
 *     conditions and the following disclaimer.
 * 
 *   * Redistributions in binary form must reproduce the above copyright notice, this list of 
 *     conditions and the following disclaimer in the documentation and/or other materials provided 
 *     with the distribution.
 * 
 *   * Any redistribution, use, or modification is done solely for personal benefit and not for any 
 *     commercial purpose or for monetary gain
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Cascades.h"

void Cascades::setup()
{

	// colorImg is null at this point, this was crashing the app.
    //colorImg->allocate(VISION_WIDTH,VISION_HEIGHT);
	grayImage.allocate(VISION_WIDTH,VISION_HEIGHT);
	grayBg.allocate(VISION_WIDTH,VISION_HEIGHT);
	grayDiff.allocate(VISION_WIDTH,VISION_HEIGHT);
	
	ofSetVerticalSync(true);
	
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.setFPS(30.0);
	
	bodyShape = NULL;
	
	bLearnBakground = true;
	threshold = 80;
	
	debugging = false;
	contours = false;
	
	waterfallLEdge = 10;
	waterfallREdge = 300;
}

void Cascades::update()
{
	ofBackground(100,100,100);
	float height = ofGetHeight();
	
	vector<ofxBox2dCircle>::iterator cit = circles.begin();
	
	while( cit != circles.end()) 
	{
		if(cit->getPosition().y > height + 100) {
			circles.erase(cit);
		}
		++cit;
	}
	
	if( circles.size() < 100 ) { // > 100 on screen
		ofxBox2dCircle circle;
		circle.setPhysics(1, 0.5, 0.1);
		float pos = ofRandom(waterfallREdge - waterfallLEdge) + 10;
		circle.setup(box2d.getWorld(), pos, -100, 80);
		circles.push_back(circle); // add a new one
	}
	
	box2d.update();
	
	colorImg->mirror(false, true);
	
	grayImage = *colorImg;
	if (bLearnBakground == true){
		grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
		bLearnBakground = false;
	}
	grayDiff.absDiff(grayBg, grayImage);
	grayDiff.threshold(threshold);
	//|| find holes //||
	contourFinder.findContours(grayDiff, 20, (VISION_WIDTH*VISION_HEIGHT)/3, 10, true);
	
	checkBlobs();
}

void Cascades::draw(){
	
	ofSetColor(255, 255, 255);
	ofFill();
	
	vector<ofxBox2dCircle>::iterator cit = circles.begin();
	while( cit != circles.end()) {
		//cit->draw();
		ofCircle(cit->getPosition().x, cit->getPosition().y, 30);
		++cit;
	}
	
	ofSetColor(0, 0, 255);
	
	vector<ofxCvBlob>::iterator c_it = contourFinder.blobs.begin();
	
	if(contours) {
		while(c_it != contourFinder.blobs.end() ) {
			c_it->draw();
			++c_it;
		}
	}
	
	if(bodyShape != NULL) {
		bodyShape->draw();
		
		ofSetColor(0, 0, 255);
		ofFill();
		vector <ofPoint>::iterator pit = bodyShape->getVertices().begin();
		while(pit != bodyShape->getVertices().end() )
		{
			ofCircle(pit->x, pit->y, 30);
			++pit;
		}
	}
}

void Cascades::checkBlobs() 
{
	
	contourFinder.findContours(grayDiff, 120, (grayDiff.getWidth()*grayDiff.getHeight())/2, 5, false); // no holes
	
	if(contourFinder.blobs.size() < 1) return;
	
	vector<ofxCvBlob>::iterator c_it = contourFinder.blobs.begin();
	ofxCvBlob largest = contourFinder.blobs[0];
	
	while(c_it != contourFinder.blobs.end() ) {
		if(c_it->area > largest.area) largest = *c_it;
		++c_it;
	}
	
	cout << " largest " <<  largest.area << " " << largest.pts.size() << endl;
	

	if(!debugging)
	{
		if(bodyShape) {
			bodyShape->destroy();
			delete bodyShape;
		}
		
		bodyShape = new ofxBox2dPolygon();
		ofVec2f scaleUp(ofGetWidth()/320, ofGetHeight()/240);
		
		vector <ofPoint>::iterator pit = largest.pts.begin();
		while(pit != largest.pts.end() )
		{
			// triangulatePolygonWithOutline(resampled, outline);??
			bodyShape->addVertex( *pit * scaleUp );
			++pit;
		}
		
		bodyShape->simplify(20);
		bodyShape->create(box2d.getWorld());
		
		cout << " is good shape " << bodyShape->isGoodShape() << " ";
	
	}
	
}

void Cascades::mouseDragged(int x, int y, int button){

	if(debugging) {
		
		if(bodyShape) 
		{
			bodyShape->destroy();
			delete bodyShape;
		}
		bodyShape = new ofxBox2dPolygon();
		
		ofVec2f ul( x - 50.f, y-50.f);
		ofVec2f ur( x + 50.f, y-50.f);
		ofVec2f lr( x + 50.f, y+50.f);
		ofVec2f ll( x - 50.f, y+50.f);
		
		bodyShape->addVertex(ul);
		bodyShape->addVertex(ur);
		bodyShape->addVertex(lr);
		bodyShape->addVertex(ll);
		
		bodyShape->create(box2d.getWorld());
		
	}
}

void Cascades::mousePressed(int x, int y, int button){

	if(debugging) {
		
		if(bodyShape) 
		{
			bodyShape->destroy();
			delete bodyShape;
		}
		bodyShape = new ofxBox2dPolygon();
		
		ofVec2f ul( x - 50.f, y-50.f);
		ofVec2f ur( x + 50.f, y-50.f);
		ofVec2f lr( x + 50.f, y+50.f);
		ofVec2f ll( x - 50.f, y+50.f);
		
		bodyShape->addVertex(ul);
		bodyShape->addVertex(ur);
		bodyShape->addVertex(lr);
		bodyShape->addVertex(ll);
		
		bodyShape->create(box2d.getWorld());
	
	}
}

/*void Cascades::keyPressed(int key){
	
	switch (key){
		case ' ':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
		case 'd':
			debugging = !debugging;
			break;
		case 'c':
			contours = !contours;
			break;
	}
}*/