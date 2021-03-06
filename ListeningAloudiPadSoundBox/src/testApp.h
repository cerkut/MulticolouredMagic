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

#pragma once

#include "ofMain.h"
#ifdef TARGET_OF_IPHONE
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
#endif
#include "constants.h"
#include "Centerer.h"
#include "Pad.h"
#include "SoundBoxGui.h"
#include "HoldToggle.h"


class testApp :

#ifdef TARGET_OF_IPHONE
public ofxiPhoneApp
#else 
public ofBaseApp
#endif

{

public:
	void setup();
	void update();
	void draw();

	void exit();

	void audioOut(float * output, int bufferSize, int nChannels);
	void audioIn(float * output, int bufferSize, int nChannels);
	
	void touchDown(ofTouchEventArgs & touch);
	void touchMoved(ofTouchEventArgs & touch);
	void touchUp(ofTouchEventArgs & touch);
	void touchDoubleTap(ofTouchEventArgs & touch);
	void touchCancelled(ofTouchEventArgs & touch);



#ifndef TARGET_OF_IPHONE
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
#endif
	

	void lostFocus();
	void gotFocus();
	void gotMemoryWarning();
	void deviceOrientationChanged(int newOrientation);

private:
	void setupGraphics();
	void setupOrientation();
	int currOrientation;
	void updateOrientation();
	
	void setNumPads(int numPads);
	Centerer centerer;
	vector<Pad*> pads;
	SoundBoxGui gui;
	ofMutex mutex;
	HoldToggle recMode;
};

