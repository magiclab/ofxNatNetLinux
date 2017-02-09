//
//  ofxNatNetFrameListener.h
//
//  magicLab 2017
//  http://www.magiclab.nyc
//  Created by enrico<nausea>viola on 2/6/17.
//
//

#pragma once
#include "ofMain.h"
#include <NatNetLinux/NatNet.h>
#include <NatNetLinux/NatNetPacket.h>
#include <NatNetLinux/NatNetSender.h>

#include <utility>
#include <time.h>
#ifdef TARGET_OSX
#include <mach/clock.h>
#include <mach/mach.h>
#endif

class ofxNatNetFrameListener{
public:
    enum FrameResult{
        SUCCESS,
        TIMEOUT,
        UNKNOWN
    };
    
    void init(){
        toSec = 1;
        bNewFrame = false;
    }
    
    void setup(int sd=-1, unsigned char nnMajor=0, unsigned char nnMinor=0, size_t bufferSize=64){
        init();
        _sd=sd;
        _nnMajor=nnMajor;
        _nnMinor=nnMinor;
        lastFrame = MocapFrame(_nnMajor, _nnMinor);
    }
    
    bool isNewFrameReady(){return bNewFrame;}
    
    void updateTimeStamp(){
#ifdef TARGET_OSX
        clock_serv_t cclock;
        mach_timespec_t mts;
        host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
        clock_get_time(cclock, &mts);
        mach_port_deallocate(mach_task_self(), cclock);
        lastTs.tv_sec = mts.tv_sec;
        lastTs.tv_nsec = mts.tv_nsec;
#else
        clock_gettime( CLOCK_MONOTONIC, &lastTs );
#endif
    }
    
    FrameResult nextFrame(){
        FrameResult fr = ofxNatNetFrameListener::UNKNOWN;
        
        timeout.tv_sec = 1;
        timeout.tv_usec= 0;
        
        FD_ZERO(&rfds);
        FD_SET(_sd, &rfds);
        
        if(!select(_sd+1, &rfds, 0, 0, &timeout)){
            fr = ofxNatNetFrameListener::TIMEOUT;
            return fr;
        }
        
        dataBytes = read(_sd, nnp.rawPtr(), nnp.maxLength());
        
        if(dataBytes>0 && nnp.iMessage()==NatNetPacket::NAT_FRAMEOFDATA){
            MocapFrame mFrame(_nnMajor, _nnMinor);
            mFrame.unpack(nnp.rawPayloadPtr());
            
            updateTimeStamp();
            lastFrame = mFrame;
            bNewFrame = true;
            fr = ofxNatNetFrameListener::SUCCESS;
        }
        
        return fr;
    }
    
    MocapFrame getLastFrame(){
        bNewFrame = false;
        return lastFrame;
    }
    
    struct timespec getLastTimeStamp(){
        return lastTs;
    }
    
    std::pair<MocapFrame, struct timespec> getLastFrameInfo(){
        return std::make_pair(getLastFrame(), getLastTimeStamp());
    }
protected:
    MocapFrame lastFrame;
    NatNetPacket nnp;
    struct timespec lastTs;
    struct timeval timeout;
    fd_set rfds;
    size_t dataBytes;
    int _sd;
    int toSec;
    unsigned char _nnMajor;
    unsigned char _nnMinor;
    bool bNewFrame;
};