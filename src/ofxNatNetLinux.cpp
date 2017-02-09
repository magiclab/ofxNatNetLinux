//
//  ofxNatNetLinux.cpp
//
//  magicLab 2017
//  http://www.magiclab.nyc
//  Created by enrico<nausea>viola on 2/3/17.
//
//

#include "ofxNatNetLinux.h"
#include <unistd.h>

ofxNatNetLinux::ofxNatNetLinux(){
    ipLocal="127.0.0.1";
    ipServer="127.0.0.1";
    bStarted = false;
}

ofxNatNetLinux::~ofxNatNetLinux(){
    exit();
}

void ofxNatNetLinux::exit(){
    if(!bStarted)return;
    cmdListener.waitForThread();
    close(sdData);
    close(sdCommand);
}

void ofxNatNetLinux::setup(string _ipLocal, string _ipServer){
    ipLocal = _ipLocal;
    ipServer = _ipServer;
    
    ipLocalI = inet_addr(ipLocal.c_str());
    ipServerI = inet_addr(ipServer.c_str());
    
    serverCommands = NatNet::createAddress(ipServerI, NatNet::commandPort);
    sdCommand = NatNet::createCommandSocket(ipLocalI);
    sdData = NatNet::createDataSocket(ipLocalI);
    
    cmdListener.setSocket(sdCommand);
    cmdListener.start();
    
    ping();
    
    askVersion();
    
    bStarted = true;
}

void ofxNatNetLinux::setupFrameListener(){
    frameListener.setup(sdData, natNetMajor, natNetMinor);
}

ofxNatNetLinux::Frame ofxNatNetLinux::update(){
    ofxNatNetLinux::Frame ff;
    ofxNatNetFrameListener::FrameResult fr = frameListener.nextFrame();
    ff.result = fr;
    switch (fr) {
        case ofxNatNetFrameListener::SUCCESS:
            cout<<frameListener.getLastFrame().asString()<<endl;
            fps.update(frameListener.getLastTimeStamp());
            cout<<fps.getFps()<<" fps"<<endl;
            ff.frame = frameListener.getLastFrame();
            ff.fps = fps.getFps();
            break;
        case ofxNatNetFrameListener::TIMEOUT:
            ofLogError("ofxNatNetFrameListener","frame timeout");
            break;
        case ofxNatNetFrameListener::UNKNOWN:
            ofLogError("ofxNatNetFrameListener","frame unknown");
            break;
            
        default:
            break;
    }
    return ff;
}

void ofxNatNetLinux::ping(){
    NatNetPacket pingPack = NatNetPacket::pingPacket();
    pingPack.send(sdCommand, serverCommands);
}

void ofxNatNetLinux::askVersion(){
    cmdListener.getNatNetVersion(natNetMajor, natNetMinor);
}

string ofxNatNetLinux::getVersionString(){
    stringstream ss;
    ss << (int)natNetMajor << "." << (int)natNetMinor;
    return ss.str();
}