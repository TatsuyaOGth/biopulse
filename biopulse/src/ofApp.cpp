#include "ofApp.h"
#include "Scene01.hpp"

#define FOR_SCENES for (vector<scenePtr>::iterator it = mScenes.begin(); it != mScenes.end(); it++)
#define CURRENT_SCENE mScenes[mNumCurrentScene]

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofSetWindowShape(plant::width, plant::height);
    
    //----------
    // create scenes
    //----------
    mScenes.push_back(scenePtr(new Scene01()));
    mNumCurrentScene = 0;
    FOR_SCENES it->get()->setup();
    
    //-----------
    // init values
    //-----------
    mMode = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    CURRENT_SCENE->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    
    
    CURRENT_SCENE->draw();

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
        case ' ':
            mMode = (mMode + 1) % 3;
            break;
            
        case OF_KEY_LEFT:
            break;
            
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

