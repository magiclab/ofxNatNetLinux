#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetVerticalSync(true);
    
    string ipL = "192.168.1.7";
    string ipS = "192.168.1.8";
    ofxXmlSettings xml;
    if(xml.load("setup.xml")){
        if(xml.tagExists("setup")){
            xml.pushTag("setup");
            ipL = xml.getValue("local", ipL);
            ipS = xml.getValue("server", ipS);
            xml.popTag();
        }
    }else{
        ofLogError("ofApp::setup","cannot find setup.xml");
    }
    
    cout<<"Local IP: "<<ipL<<", server IP: "<<ipS<<endl;
    natNet.setup(ipL, ipS);
    natNet.setupFrameListener();
}

//--------------------------------------------------------------
void ofApp::update(){
    natNet.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
