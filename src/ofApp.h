#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOpenCV.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"
#include "ofxFaceTrackerThreaded.h"
#include "ofxNetwork.h"
#include "ofxSyphon.h"

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void exit();
    
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
    
    /* ofxGui */
	bool bHide;
    ofxToggle faceDetect;
    ofxVec2Slider facePos;
    ofxVec2Slider mousePos;
	ofxPanel gui;
	ofSoundPlayer ring;

    /* face tracking */
    ofVideoGrabber cam;
	ofxFaceTrackerThreaded tracker;
    
    /* sun light */
    ofPoint sunCenter;
    ofSoundPlayer space;
    
    /* UDP Connection */
    ofxUDPManager udpConnection;
    
    /* Syphon */
    ofTexture tex;
	ofxSyphonServer mainOutputSyphonServer;
	ofxSyphonServer individualTextureSyphonServer;
	ofxSyphonClient mClient;
    
    /* mask Image */
    ofImage mask;
    ofImage earth;
    ofImage space_back;
};