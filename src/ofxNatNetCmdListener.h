//
//  ofxNatNetCmdListener.h
//
//  magicLab 2017
//  http://www.magiclab.nyc
//  Created by enrico<nausea>viola on 2/3/17.
//
//

#pragma once
#include "ofMain.h"
#include "NatNetLinux/NatNet.h"
#include "NatNetLinux/NatNetPacket.h"
#include "NatNetLinux/NatNetSender.h"

class ofxNatNetCmdListener : public ofThread{
public:
    void setSocket(int _sd){
        if(isThreadRunning())lock();
        sd=_sd;
        if(isThreadRunning())unlock();
    }
    
    void getNatNetVersion(unsigned char & major, unsigned char & minor){
        if(isThreadRunning())lock();
        major = nnMajor;
        minor = nnMinor;
        if(isThreadRunning())unlock();
    }
    
    void start(bool _verbose=false){
        senderAddressLength = sizeof(senderAddress);
        response = NULL;
        bVerbose = _verbose;
        startThread();
    }
    
    ofEvent<void> evPong;
protected:
    
    void threadedFunction(){
        while(isThreadRunning()){
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;
            FD_ZERO(&rfds);
            FD_SET(sd, &rfds);
            if(!select(sd+1, &rfds, 0, 0, &timeout)){
                continue;
            }
            
            len = recvfrom(sd,
                           nnp.rawPtr(), nnp.maxLength(),
                           0, reinterpret_cast<struct sockaddr*>(&senderAddress), &senderAddressLength);
            
            if(len<=0)
                continue;
            
            switch(nnp.iMessage()){
                case NatNetPacket::NAT_MODELDEF:
                    break;
                case NatNetPacket::NAT_FRAMEOFDATA:
                    break;
                case NatNetPacket::NAT_PINGRESPONSE:
                    sender.unpack(nnp.read<char>(0));
                    nnMajor = sender.natNetVersion()[0];
                    nnMinor = sender.natNetVersion()[1];
                    cout<<"NatNet Server: "<<sender.name()<<endl;
                    cout<<"NatNet Version: "<<(int)(sender.natNetVersion()[0])<<"."<<(int)(sender.natNetVersion()[1])<<endl;
                    ofNotifyEvent(evPong);
                    break;
                case NatNetPacket::NAT_RESPONSE:
                    response = nnp.read<char>(0);
                    if(bVerbose)printf("Response: %s",response);
                    break;
                case NatNetPacket::NAT_UNRECOGNIZED_REQUEST:
                    printf("[Client] received 'unrecognized request'\n");
                    break;
                case NatNetPacket::NAT_MESSAGESTRING:
                    response = nnp.read<char>(0);
                    if(bVerbose)printf("[Client] Received message: %s\n", response);
                    break;
                default:
                    break;
            }
        }
    }
    
    NatNetPacket nnp;
    NatNetSender sender;
    int sd;
    unsigned char nnMajor;
    unsigned char nnMinor;
    ssize_t len;
    fd_set rfds;
    socklen_t senderAddressLength;
    struct sockaddr_in senderAddress;
    struct timeval timeout;
    char const * response;
    bool bVerbose;
};