#pragma once

#include "ofMain.h"
#include "common.h"
#include "DataController.hpp"
#include "BaseSceneInterface.hpp"
#include "MidiSenderController.hpp"
#include "MidiReceiverController.hpp"
#include "PostGlitch.hpp"
#include "ofxExtras.h"
#include "ofxOsc.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    int changeScene(int mv);
    void changeTargetSccene(int sceneid);
    void receivedMidiMessage(ofxMidiMessage & e);
    
    DataController * mDataController;
    
    typedef shared_ptr<BaseSceneInterfase> scenePtr;
    vector<scenePtr> mScenes;
    int mNumCurrentScene;
    
    int mMode;
    
    PostGlitch * mGlitch;
    ofFbo mFbo;
};
