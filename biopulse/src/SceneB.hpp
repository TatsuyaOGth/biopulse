#pragma once

#include "BaseSceneInterface.hpp"
#include "MidiReceiverController.hpp"
#include "Ground.hpp"
#include "VoltageIndicator.hpp"
#include "DataMatrix.hpp"
#include "Radar.hpp"
#include "GenerativeObject.hpp"
#include "Radiation.hpp"
#include "PatternExample.hpp"
#include "ScanLines.hpp"
#include "RotationBox.hpp"
#include "TargetRadar.hpp"
#include "EmittingObject.hpp"

class SceneB : public BaseSceneInterfase
{

    BaseContentsController mHead;
    BaseContentsController mBody;
    BaseContentsController mPlant;
    
    VoltageIndicator * mVL;
    DataMatrix * mDT;
    Radar * mR;
    GenerativeObject * mGO;
    Radiation * mRD;
    PatternExample * mPT;
    Ground * mGD;
    ScanLines * mSL;
    RotationBox * mBX;
    TargetRadar * mTR;
    EmittingObject * mEM;
    
public:
    
    void setup()
    {
        
        //----------
        // setup contents
        //----------
        mVL     = mHead.createInstance<VoltageIndicator>();
        
        mGD     = mBody.createInstance<Ground>();
        mDT     = mBody.createInstance<DataMatrix>();
        mR      = mBody.createInstance<Radar>();
        mGO     = mBody.createInstance<GenerativeObject>();
        mRD     = mBody.createInstance<Radiation>();
        mPT     = mBody.createInstance<PatternExample>();
        mEM     = mBody.createInstance<EmittingObject>();
        mBX     = mBody.createInstance<RotationBox>();
        mTR     = mBody.createInstance<TargetRadar>();
        mSL     = mBody.createInstance<ScanLines>();
        
        mBody.setBlendMode(OF_BLENDMODE_ADD);
        mPlant.setBlendMode(OF_BLENDMODE_ADD);
        
        // init
        mSL->mCol.set(255);
        mSL->mLineWidth = 4;
        mGD->setupCam(plant::bodyW, plant::bodyH);
        mBX->setupCam(plant::bodyW, plant::bodyH);
        mEM->mSize = 20;
        
        //----------
        // setup contents fbo
        //----------
        ofFbo::Settings s1;
        s1.width  = plant::edgeW;
        s1.height = plant::edgeH;
        s1.useDepth = true;
        s1.useStencil = true;
        s1.internalformat = GL_RGBA;
        mHead.setFboAllocate(s1);
        
        ofFbo::Settings s2;
        s2.width  = plant::bodyW;
        s2.height = plant::bodyH;
        s2.useDepth = true;
        s2.useStencil = true;
        s2.internalformat = GL_RGBA;
        mBody.setFboAllocate(s2);
        
        ofFbo::Settings s3;
        s1.width  = plant::width;
        s1.height = plant::height;
        s1.useDepth = true;
        s1.useStencil = true;
        s1.internalformat = GL_RGBA;
        mPlant.setFboAllocate(s3);
        
//        mVL->play();
//        mDT->play();
//        mHead.playAll();
//        mBody.playAll();
        
        //----------
        // setup timeline
        //----------
        timeline.setDurationInSeconds(90);
        timeline.addFlags("flag");
        timeline.addCurves("bgbri", ofRange(0, 255));
        
        ofAddListener(timeline.events().bangFired, this, &SceneB::getFlag);
        
        //----------
        // setup midi receiver
        //----------
        ofAddListener(MIDI_RECEIVER->receivedMidiEvent, this, &SceneB::receivedMidiMessage);
    }
    
    void update()
    {
        mHead.update();
        mBody.update();
        mPlant.update();
    }
    
    void draw()
    {
        unsigned char g = timeline.getValue("bgbri");
        ofBackground(g, g, g);
        
        ofPushStyle();
        ofEnableAlphaBlending();
        mHead.draw(plant::edgeX, plant::edgeY, plant::edgeW, plant::edgeH);
        mBody.draw(plant::bodyX, plant::bodyY, plant::bodyW, plant::bodyH);
        mPlant.draw(0, 0, plant::width, plant::width);
        ofPopStyle();
        
    }
    
    void getFlag(ofxTLBangEventArgs & args)
    {
        if (args.flag == "vl") playToggles(0, true);
        if (args.flag == "dt") playToggles(1, true);
        if (args.flag == "r") playToggles(2, true);
        if (args.flag == "rd") playToggles(3, true);
        if (args.flag == "pt") playToggles(4, true);
        if (args.flag == "go") playToggles(5, true);
        
        if (args.flag == "_vl") playToggles(0, false);
        if (args.flag == "_dt") playToggles(1, false);
        if (args.flag == "_r") playToggles(2, false);
        if (args.flag == "_rd") playToggles(3, false);
        if (args.flag == "_pt") playToggles(4, false);
        if (args.flag == "_go") playToggles(5, false);
    }
    
    void keyPressed( int key )
    {
        if (key == 'q') mDT->togglePlay();
        if (key == 'w') mR->togglePlay();
        if (key == 'e') mRD->togglePlay();
        if (key == 'r') mPT->togglePlay();
        
        mHead.sendMessageAll(key);
        mBody.sendMessageAll(key);
    }
    
    void playToggles(int i, bool sw)
    {
        switch (i) {
            case 0: mVL->play(sw); break;
            case 1: mDT->play(sw); break;
            case 2: mR->play(sw); break;
            case 3: mRD->play(sw); break;
            case 4: mPT->play(sw); break;
            case 5: mGO->play(sw); break;
        }
    }
    
    void receivedMidiMessage(ofxMidiMessage & e)
    {
        if (e.status == MIDI_NOTE_ON) {
            if (e.channel == 1) {
                mTR->bVisible = true;
            }
            if (e.channel == 2) {
            }
            if (e.channel == 3) {
                
            }
            if (e.channel == 4) {
            }
            if (e.channel == 5) {
                if (e.pitch == 105) {
                    mSL->createStrobo(0.05, false);
                    for (int i = 0; i < 8; i++) {
                        mSL->createRippleCircles(320 + (i * 20), 0.05, 4);
//                        mSL->createRippleArc(260 + (ofRandom(200)), 0.03, 4.5);
                    }
                }
            }
            if (e.channel == 6) {
            }
            if (e.channel == 7) {
                if (e.channel == 7) {
                    if (e.pitch == 43) {
                        mSL->createVerticalSplitWave(0.3);
                    }
                }
            }
            if (e.channel == 8) { // pan sign
                if (e.pitch == 117) {
                    mSL->createVerticalScanLine(0, plant::bodyH, true, 1);
                }
                if (e.pitch == 120) {
//                    mSL->createSimpleCircle(plant::bodyW * 0.5, (plant::bodyH * 0.5) + plant::edgeH, 20, 0.05);
                    mSL->createSimpleCircle(plant::bodyW * 0.5, plant::bodyH * 0.5, 20, 0.05);
                }
            }
            if (e.channel == 9) {
                mEM->mSize = 45;
            }
            if (e.channel == 10) {
            }
            
            if (e.channel == 16) {
                if (e.pitch == 0) { // scene B-1
                    mVL->play();
                    mSL->play();
                    mGD->stop();
                    mDT->play();
                    mR->stop();
                    mRD->stop();
                    mPT->stop();
                    mGO->stop();
                    mBX->stop();
                    mTR->stop();
                    mEM->stop();
                }
                if (e.pitch == 1) { // scene B-2
                    mVL->play();
                    mSL->play();
                    mGD->stop();
                    mDT->stop();
                    mR->play();
                    mRD->stop();
                    mPT->stop();
                    mGO->stop();
                    mBX->stop();
                    mTR->play();
                    mEM->stop();
                }
                if (e.pitch == 2) { // scene B-3
                    mVL->play();
                    mSL->play();
                    mGD->stop();
                    mDT->stop();
                    mR->play();
                    mRD->stop();
                    mPT->stop();
                    mGO->stop();
                    mBX->stop();
                    mTR->play();
                    mEM->stop();
                }
                if (e.pitch == 3) { // scene B-4
                    mVL->play();
                    mSL->play();
                    mGD->stop();
                    mDT->stop();
                    mR->play();
                    mRD->stop();
                    mPT->stop();
                    mGO->stop();
                    mBX->play();
                    mTR->play();
                    mEM->play();
                }
                if (e.pitch == 4) { // scene B-5
                    mVL->stop();
                    mSL->stop();
                    mGD->stop();
                    mDT->stop();
                    mR->stop();
                    mRD->stop();
                    mPT->stop();
                    mGO->stop();
                    mBX->stop();
                    mTR->stop();
                    mEM->play();
                }
                if (e.pitch == 10) { // shange matrix mode
                    mDT->mode = 1;
                }
            }
        }
        
        if (e.status == MIDI_NOTE_OFF) {
            if (e.channel == 1) {
                mTR->bVisible = false;
            }
            if (e.channel == 2) {
            }
            if (e.channel == 3) {
            }
            if (e.channel == 4) {
            }
            if (e.channel == 5) {
            }
            if (e.channel == 6) {
            }
            if (e.channel == 7) {
            }
            if (e.channel == 8) {
            }
            if (e.channel == 9) {
                mEM->mSize = 20;
            }
            if (e.channel == 10) {
            }
        }
    }
};
