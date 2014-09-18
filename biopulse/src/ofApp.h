#pragma once

#include "ofMain.h"
#include "../../common.h"
#include "ofxExtras.h"

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
    
    typedef shared_ptr<BaseSceneInterfase> scenePtr;
    vector<scenePtr> mScenes;
    int mNumCurrentScene;
    
    int mMode;
};
