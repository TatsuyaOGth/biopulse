#pragma once

#include "BaseSceneInterface.hpp"
#include "ofxEasingCam.h"
#include "ofxAnimationPrimitives.h"
#include "Ground.hpp"
#include "VoltageIndicator.hpp"


class Scene01 : public BaseSceneInterfase
{
    
    share::datasetPtr * mDataset;
    deque<string> mDataList;

    BaseContentsController mHead;
    BaseContentsController mBody;
    
public:
    
    void setup()
    {
        mDataset = &share::datasets[0];
        mDataset->get()->setBufferLemgth(256);
        mDataset->get()->stop();
        mDataset->get()->setGain(200);
        
        timeline.setDurationInSeconds(30);
        timeline.addFlags("flag");
        timeline.addCurves("speed", ofRange(0.1, 3));
        timeline.addCurves("gain", ofRange(100, 1000));
        timeline.addColors("bgcolor");
        timeline.addColors("datacolor");
        
        ofAddListener(timeline.events().bangFired, this, &Scene01::getFlag);
        
        //----------
        // setup contents
        //----------
        mHead.createInstance<VoltageIndicator>();
        mBody.createInstance<Ground>();
        
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
        
        mHead.playAll();
        mBody.playAll();
    }
    
    void update()
    {
        mDataset->get()->setSpeed(timeline.getValue("speed"));
        mDataset->get()->setGain(timeline.getValue("gain"));
        
        DataController::Data * d = mDataset->get()->getDataOnStartPoint();
        string idx = ofToString(mDataset->get()->offset, 100);
        mDataList.push_back(idx + " date: " + d->timestamp + " voltage: " + ofToString(d->voltage));
        if (mDataList.size() > 36) {
            mDataList.pop_front();
        }
        
        
        mHead.update();
        mBody.update();
    }
    
    void draw()
    {
        ofPushStyle();
        ofDisableAntiAliasing();
        
        ofBackground(timeline.getColor("bgcolor"));
        
        mHead.draw();
        mBody.draw();

        drawDataList();
        drawCircleGraph();
        
        ofPopStyle();
    }
    
    void getFlag(ofxTLBangEventArgs & args)
    {
        if (args.flag == "start") {
            mDataset->get()->play();
        }
        if (args.flag == "stop") {
            mDataset->get()->stop();
        }
    }
    
    void keyPressed( int key )
    {
        mHead.sendMessage(0, key);
    }

    
    void drawDataList()
    {
        ofSetColor(0, 255, 0);
        
        int x = plant::bodyXW * 0.2;
        int y = plant::bodyY + 10;
        for (int i = 0; i < mDataList.size(); i++) {
            ofDrawBitmapString(mDataList[i], x, y + (i * 16));
        }
    }
    
    void drawCircleGraph()
    {
        
    }
    

        
};
