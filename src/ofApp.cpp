#include "ofApp.h"

using namespace ofxCv;


//--------------------------------------------------------------
void ofApp::setup(){
    
    /* ofxGui */
	ofSetVerticalSync(true);
	gui.setup(); // most of the time you don't need a name
	gui.add(faceDetect.setup("Face Detect", true));
	gui.add(facePos.setup("face position",ofVec2f(0,0),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
    gui.add(mousePos.setup("face position",ofVec2f(0,0),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
	bHide = true;
    
    /* face tracker */
    cout << "*****************************************************************" << endl;
    for(int i=0; i < cam.listDevices().size(); i++) {
        cout << "deviceName : " << cam.listDevices()[i].deviceName << ", deviceID : " << cam.listDevices()[i].id << endl;
    }
    cout << "*****************************************************************" << endl;
    cam.setDeviceID(1);
	cam.initGrabber(640, 480);
    ofEnableAlphaBlending();
	tracker.setup();
    
    /* sun light */
    sunCenter.x = ofGetWidth() / 2;
    sunCenter.y = ofGetHeight() + 100;
    space.loadSound("sunlight.mp3");
    space.play();
    
    /* UDP Connection */
    udpConnection.Create();
	udpConnection.Connect("127.0.0.1",11999);
	udpConnection.SetNonBlocking(true);
    
    /* Syphone */
	mainOutputSyphonServer.setName("one_night_2014");
    
    /* mask image */
    mask.loadImage("mask.png");
    earth.loadImage("earth.png");
    space_back.loadImage("space.jpg");
}

//--------------------------------------------------------------
void ofApp::exit(){
}

//--------------------------------------------------------------
void ofApp::update(){
    /* gui */
    facePos = tracker.getPosition();
    faceDetect = tracker.getFound();
    
    ofPoint mouseP;
    mouseP.x = ofGetMouseX();
    mouseP.y = ofGetMouseY();
    mousePos = mouseP;
    
    /* face tracker */
    cam.update();
	if(cam.isFrameNew()) {
		tracker.update(toCv(cam));
	}
    
    if(faceDetect) {
        sunCenter.y -= 0.5;
    } else {
        sunCenter.y += 0.2;
        if(sunCenter.y > ofGetHeight() + 100) {
            sunCenter.y = ofGetHeight() + 100;
        }
    }
    
    //sunCenter.x = ofGetWidth() / 2;
    //sunCenter.y = ofGetHeight() / 2;
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor::white, ofColor::gray);
    
    float posX = tracker.getPosition().x;
    float posY = tracker.getPosition().y;
    ofRectangle faceSize = tracker.getHaarRectangle();
    //cout << faceSize.x << " : " << faceSize.y << " : " << faceSize.width << " : " << faceSize.height;
    ofMesh mesh = tracker.getImageMesh();
    mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
    float rationFace = 1;
    
    //背景の描画
    space_back.draw(0,0);
    
    //位置を保存
    ofPushMatrix();
    //映像の位置を調整 & 映像の描画
    ofTranslate(sunCenter.x / rationFace - posX, sunCenter.y / rationFace - posY);
    ofSetColor(255, 255, 255);
    //cam.draw(0, 0);
    
    unsigned char * pixels = cam.getPixels();
    
    for (int i = 0; i < 640; i++) {
        for (int j = 0; j < 480; j++) {
            ofColor color;
            color.r = pixels[(int)((j * 640 + i) * 3)];
            color.g = pixels[(int)((j * 640 + i) * 3 + 1)];
            color.b = pixels[(int)((j * 640 + i) * 3 + 2)];
            
            if(ofDist(posX, posY, i, j) < 70) {
                ofSetColor(color.r, color.g, color.b);
                ofRect(i, j, 1, 1);
            }
            
        }
    }
    
    //face trackerの描画
    ofSetColor(255, 255, 255);
    ofSetLineWidth(0.5);
    //tracker.draw(true);
    //mesh.draw();
    
    //太陽の描画
    ofSetColor(255, 255, 255);
    mask.draw(posX-310, posY-250,  mask.width, mask.height);
    
    ofPopMatrix();
    
    //地球の描画
    ofSetColor(255, 255, 255);
    earth.draw(0, 416, ofGetWidth(), ofMap(earth.height, 0, ofGetHeight(), 0, earth.height));
    
    
	if( bHide ){
		gui.draw();
	}
    
    /* Syphon */    
	mainOutputSyphonServer.publishScreen();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == 'h' ){
		bHide = !bHide;
	}
	if(key == 's') {
		gui.saveToFile("settings.xml");
	}
	if(key == 'l') {
		gui.loadFromFile("settings.xml");
	}
    
    if (key == '@') {
        unsigned char * pixels = cam.getPixels();
        //char pix[65535];
        vector<int> pix;
        for (int x = 0; x < cam.width; x++) {
            for (int y = 0; y < cam.height; y++) {
                int index = cam.height * x + y;
                pix.push_back(pixels[index]);
            }
        }
        
        cout << ofToString(pix.size()) << endl;
        udpConnection.Send((const char*)pixels, strlen((const char*)pixels));
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
	
}
