#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

class OscSenderController
{
    ofxOscMessage oscMess;
    ofxOscSender oscSender;
    string mHost;
    int mPort;
    
    string mModuleName;
    
public:
    
    OscSenderController():
    mHost(""),
    mPort(0)
    {
    }
    
    OscSenderController(const string & host, const int port):
    mHost(host),
    mPort(port)
    {
        setup(host, port);
    }
    
    inline void setup(const string & host, const int port)
    {
        mHost = host;
        mPort = port;
        oscSender.setup(mHost, mPort);
        mModuleName = " - OscSenderController " + mHost + ":" + ofToString(mPort);
//        ofLogNotice(mModuleName) << "setup host and port";
    }
    
    inline void setAddress(const string& addr)
    {
        if (oscMess.getNumArgs() || !oscMess.getAddress().empty()) oscMess.clear();
        oscMess.setAddress(addr);
    }
    
    inline void addArg(int v)
    {
        oscMess.addIntArg(v);
    }
    
    inline void addArg(float v)
    {
        oscMess.addFloatArg(v);
    }
    
    inline void addArg(string v)
    {
        oscMess.addStringArg(v);
    }
    
    inline void send()
    {
        oscSender.sendMessage(oscMess);
        oscMess.clear();
    }
    
    //----------------------------------------- simple sender
    template <typename T>
    void send(const string& addr, T arg)
    {
        setAddress(addr);
        addArg(arg);
        send();
    }
    
    template <typename T1, typename T2>
    void send(const string& addr, T1 arg1, T2 arg2)
    {
        setAddress(addr);
        addArg(arg1);
        addArg(arg2);
        send();
    }
    
    template <typename T1, typename T2, typename T3>
    void send(const string& addr, T1 arg1, T2 arg2, T3 arg3)
    {
        setAddress(addr);
        addArg(arg1);
        addArg(arg2);
        addArg(arg3);
        send();
    }
};
