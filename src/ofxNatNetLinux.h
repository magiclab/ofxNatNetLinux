//
//  ofxNatNetLinux.h
//
//  magicLab 2017
//  http://www.magiclab.nyc
//  Created by enrico<nausea>viola on 2/3/17.
//
//

#pragma once
#include "ofMain.h"
#include "ofxNatNetCmdListener.h"
#include "ofxNatNetFrameListener.h"
#include "NatNetLinux/NatNet.h"
#include "NatNetLinux/NatNetPacket.h"
#include "NatNetLinux/FPSCounter.h"

class ofxNatNetLinux{
public:
    class Frame{
    public:
        ofxNatNetFrameListener::FrameResult result;
        MocapFrame frame;
        double fps;
    };
    
    ofxNatNetLinux();
    ~ofxNatNetLinux();
    void exit();
    void setup(string _ipLocal, string _ipServer);
    void setupFrameListener();
    ofxNatNetLinux::Frame update();
    
    void ping();
    void askVersion();
    
    inline string getLocalIP(){return ipLocal;}
    inline string getServerIP(){return ipServer;}
    inline unsigned char & getVersionMajor(){return natNetMajor;}
    inline unsigned char & getVersionMinor(){return natNetMinor;}
    inline ofEvent<void> & getPongEvent(){return cmdListener.evPong;}
    string getVersionString();
protected:
    
    ofxNatNetCmdListener cmdListener;
    ofxNatNetFrameListener frameListener;
    FPSCounter fps;
    string ipLocal;
    string ipServer;
    uint32_t ipLocalI;
    uint32_t ipServerI;
    int sdCommand;
    int sdData;
    unsigned char natNetMajor;
    unsigned char natNetMinor;
    struct sockaddr_in serverCommands;
    bool bStarted;
};