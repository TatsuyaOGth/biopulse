#include "ofApp.h"
#include "SceneA.hpp"
#include "SceneB.hpp"


#define FOR_SCENES for (vector<scenePtr>::iterator it = mScenes.begin(); it != mScenes.end(); it++)
#define CURRENT_SCENE mScenes[mNumCurrentScene]

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofSetWindowShape(plant::width, plant::height);
    ofxTimeline::removeCocoaMenusFromGlut("biopulse");
    
    //----------
    // set dataset
    //----------
    if (!DATA_CONTROLLER->loadDataSet("dataset")) {
        ofLogError() << "faild load dataset";
        OF_EXIT_APP(1);
    }
    data::bufferLength = 256;
    data::gain = 1;
    
    //----------
    // create scenes
    //----------
    mScenes.push_back(scenePtr(new SceneA()));
    mScenes.push_back(scenePtr(new SceneB()));
    
    mNumCurrentScene = 0;
    FOR_SCENES {
        it->get()->setupTimeline();
        it->get()->setup();
    }
    CURRENT_SCENE->setEnableTimeline(true);
//    CURRENT_SCENE->setup();
    
    
    //-----------
    // init values
    //-----------
    mMode = 0;
    share::font.loadFont("pf_arma_five.ttf", 11);
}

//--------------------------------------------------------------
void ofApp::update(){
    share::elapsedTimef         = ofGetElapsedTimef();
    share::elapsedTimeMicros    = ofGetElapsedTimeMicros();
    share::elapsedTimeMillis    = ofGetElapsedTimeMillis();
    
    
    CURRENT_SCENE->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    
    
    CURRENT_SCENE->draw();
    
    plant::drawPlantMask();

    ofSetColor(255);
    if (mMode == 3) {
        ofSetColor(0, 0, 0, 120);
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
        ofSetColor(255, 255, 255, 255);
        CURRENT_SCENE->drawTimeline();
    }
    if (mMode == 2) plant::drawPlantGuide();
    if (mMode == 1) {
        stringstream s;
        s << "fps: " << ofGetFrameRate() << endl;
        s << "width:  " << ofGetWidth() << endl;
        s << "height: " << ofGetHeight() << endl;
        ofDrawBitmapString(s.str(), 20, 20);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case OF_KEY_TAB: mMode = (mMode + 1) % 4; break;
        case OF_KEY_LEFT:  mNumCurrentScene = changeScene(-1); break;
        case OF_KEY_RIGHT: mNumCurrentScene = changeScene( 1); break;
        
            
        default: CURRENT_SCENE->keyPressed(key); break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key) {
        default: CURRENT_SCENE->keyReleased(key); break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    CURRENT_SCENE->mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    CURRENT_SCENE->mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    CURRENT_SCENE->mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    CURRENT_SCENE->mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    CURRENT_SCENE->windowResized(w, h);
}

int ofApp::changeScene(int mv)
{
    int tmp = mv + mNumCurrentScene;
    if (tmp < 0) return mNumCurrentScene;
    if (tmp >= mScenes.size()) return mNumCurrentScene;
//    CURRENT_SCENE->exit();
    CURRENT_SCENE->setEnableTimeline(false);
    mNumCurrentScene += mv;
//    CURRENT_SCENE->setup();
    CURRENT_SCENE->setEnableTimeline(true);
    return mNumCurrentScene;
}

