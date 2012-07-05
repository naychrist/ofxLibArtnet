#include "testApp.h"

#define NUM_UNIVERSES   4
int activeIndex = 0;

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    node.addUniverses(NUM_UNIVERSES);//optional, will create a single universe 0 otherwise
    node.setup("2.170.181.2",true);//only need to enable sendRaw if sending to multiple artnet devices

}

//--------------------------------------------------------------
void testApp::update(){
    
    //create some test data
    int length = 512;
    unsigned char data[length];
    int val = ( (float)ofGetMouseX()/ofGetWidth() ) *255;
    cout<< val <<endl;
    for (int i=0; i<length; i++) data[i] = val;
    
    //update universe data - can be unsigned char * of pixels
    node.updateDataByIndex(activeIndex, data, length);
    
    //send
    node.send();

    ofSetWindowTitle(ofToString(ofGetFrameRate(),2));
}

//--------------------------------------------------------------
void testApp::draw(){
    char buf[256];
    sprintf(buf," num universes: %i\n active universe: %i\n", (int) node.getNumUniverses(), activeIndex );
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