#include "testApp.h"
#include "ofxLibArtnet.h"

ofxLibArtnet::Node    node;

#define NUM_UNIVERSES   4
int activeIndex = 0;
int activeValue = 0;

//--------------------------------------------------------------
void testApp::setup(){
    //ofSetFrameRate(60);
    
    node.addUniverses(NUM_UNIVERSES);//optional, will create a single universe 0 otherwise
    node.setup("2.170.181.2",true);//only need to enable sendRaw if sending to multiple artnet devices

}

//--------------------------------------------------------------
void testApp::update(){
    
    //create some test data
    int length = 512;
    unsigned char data[length];
    activeValue = ofClamp(( (float)ofGetMouseX()/ofGetWidth() ) *255, 0, 255);
    for (int i=0; i<length; i++) data[i] = activeValue;
    
    ofBackground(activeValue);
    
    //update universe data - can be unsigned char * of pixels
    node.updateDataByIndex(activeIndex, data, length);
    //node.updateData(data, length);//can be called if only using one universe
    
    //send
    node.send();

    ofSetWindowTitle(ofToString(ofGetFrameRate(),2));
}

//--------------------------------------------------------------
void testApp::draw(){
    char buf[256];
    sprintf(buf," num universes: %i\n active universe: %i\n active value: %i\n", (int) node.getNumUniverses(), activeIndex, activeValue );
    ofSetColor(ofColor::red);
    ofDrawBitmapString(buf,20,20);
}

//--------------------------------------------------------------
void testApp::exit(){
    node.close();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    int i = key - '0';
    if (i >= 0 && i < NUM_UNIVERSES) {
        activeIndex = i;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}