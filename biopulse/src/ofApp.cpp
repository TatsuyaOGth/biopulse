#include "ofApp.h"
#include "SceneA.hpp"
#include "SceneB.hpp"
#include "SceneC.hpp"



#define FOR_SCENES for (vector<scenePtr>::iterator it = mScenes.begin(); it != mScenes.end(); it++)
#define CURRENT_SCENE mScenes[mNumCurrentScene]

OscSenderController BaseContentsInterface::smOscSender("127.0.0.1", 9999);

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
    data::bufferLength = 512;
    data::gain = 1;
    
    //----------
    // setup midi
    //----------
    MIDI_SENDER->listPorts();
    MIDI_SENDER->mMidiOut.openPort("IAC Driver buss 2");
    MIDI_RECEIVER->openPort("IAC Driver buss 1");
    ofAddListener(MIDI_RECEIVER->receivedMidiEvent, this, &ofApp::receivedMidiMessage);
    
    //----------
    // create scenes
    //----------
    mScenes.push_back(scenePtr(new SceneA()));
    mScenes.push_back(scenePtr(new SceneB()));
    mScenes.push_back(scenePtr(new SceneC()));
    
    mNumCurrentScene = 0;
    FOR_SCENES {
        it->get()->setupTimeline();
        it->get()->setup();
    }
    CURRENT_SCENE->setEnableTimeline(true);
    
    //-----------
    // setup post glitch
    //-----------
    ofFbo::Settings s1;
    s1.width  = plant::width;
    s1.height = plant::height;
    s1.useDepth = true;
    s1.useStencil = true;
    s1.internalformat = GL_RGBA;
    mFbo.allocate(s1);
    mGlitch = new PostGlitch(&mFbo);
    share::mainFbo = &mFbo;
    
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
    MIDI_SENDER->update();
    mGlitch->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetWindowTitle("scene: " + ofToString(mNumCurrentScene));

    
    mFbo.begin();
    ofBackground(0, 0, 0, 0);
    
    CURRENT_SCENE->draw();
    
    plant::drawPlantMask();
    mFbo.end();
    
    mGlitch->generateFx();
    
    mFbo.draw(0, 0);

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
        ofDrawBitmapStringHighlight(s.str(), 20, 20);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case OF_KEY_TAB: mMode = (mMode + 1) % 4; break;
        case '<': mNumCurrentScene = changeScene(-1); break;
        case '>': mNumCurrentScene = changeScene( 1); break;
        
        case 'g': mGlitch->setFxAsTime(OFXPOSTGLITCH_CONVERGENCE, 0.5); break;
            
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

void ofApp::changeTargetSccene(int sceneid)
{
    if (sceneid < 0) return;
    if (sceneid >= mScenes.size()) return;
    //    CURRENT_SCENE->exit();
    CURRENT_SCENE->setEnableTimeline(false);
    mNumCurrentScene += sceneid;
    //    CURRENT_SCENE->setup();
    CURRENT_SCENE->setEnableTimeline(true);
}

void ofApp::receivedMidiMessage(ofxMidiMessage & e)
{
    if (e.status == MIDI_NOTE_ON) {
        
        if (e.channel == 5) {
            if (e.pitch == 105) {
                mGlitch->setFxAsTime(OFXPOSTGLITCH_INVERT, 0.05);
            }
        }
        
        if (e.channel == 6) mGlitch->setFx(OFXPOSTGLITCH_NOISE, true);
        
        if (e.channel == 7) {
            if (e.pitch == 45) {
                mGlitch->setFx(OFXPOSTGLITCH_GLOW, true);
            }
        }
        if (e.channel == 16) {
            if (e.pitch == 20) changeTargetSccene(0);
            if (e.pitch == 21) changeTargetSccene(1);
            if (e.pitch == 22) changeTargetSccene(2);
        }
    } else if (e.status == MIDI_NOTE_OFF) {
        if (e.channel == 7) {
            if (e.pitch == 45) {
                mGlitch->setFx(OFXPOSTGLITCH_GLOW, false);
            }
        }
        if (e.channel == 6) mGlitch->setFx(OFXPOSTGLITCH_NOISE, false);
    }
}

