#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxAnimationPrimitives.h"


class MidiSenderController
{
    class Note : public ofxAnimationPrimitives::Instance
    {
        ofxMidiOut * mMidi;
        const int mCh;
        const int mPich;
    public:
        Note(ofxMidiOut * midi, int pich, int velo, int channel):
        mMidi(midi), mCh(channel), mPich(pich)
        {
            mMidi->sendNoteOn(channel, pich, velo);
        }
        void willDelete()
        {
            mMidi->sendNoteOff(mCh, mPich);
        }
    };
    

    // singleton
    MidiSenderController(){ init(); };
    
    // midi controll instances
    ofxAnimationPrimitives::InstanceManager mMidi;

    
public:
    ofxMidiOut      mMidiOut;
    int             mChannel;
    unsigned int    mCurrentPgm;
    
public:
    
    static MidiSenderController * getInstance()
    {
        static MidiSenderController * instance = new MidiSenderController();
        return instance;
    }
    
    void init()
    {
        mChannel = 1;
        mCurrentPgm = 0;
    }
    
    void update()
    {
        mMidi.update();
    }
    
    void draw()
    {
        mMidi.draw();
    }
    
    void setCurrentChannel(int ch) { mChannel = ch; }
    void setCurrentProgram(unsigned int pgm) { mCurrentPgm = pgm; }
    
    void makeNote(int note, int velo, float duration, int channel = 0)
    {
        if (channel != 0) mChannel = channel;
        mMidi.createInstance<Note>(&mMidiOut, note, velo, mChannel)->play(duration);
    }
    
    void ctlOut(int cc, int value, int channel = 0)
    {
        if (channel != 0) mChannel = channel;
        mMidiOut.sendControlChange(mChannel, cc, value);
    }
    
    void listPorts() { mMidiOut.listPorts(); }

};
