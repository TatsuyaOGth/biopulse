#pragma once

#include "BaseSceneInterface.hpp"
#include "ofxEasingCam.h"
#include "ofxAnimationPrimitives.h"
#include "Ground.hpp"
#include "VoltageIndicator.hpp"
#include "DataMatrix.hpp"
#include "Radar.hpp"
#include "GenerativeObject.hpp"
#include "Radiation.hpp"
#include "PatternExample.hpp"


class SceneB : public BaseSceneInterfase
{

    BaseContentsController mHead;
    BaseContentsController mBody;
    
public:
    
    void setup()
    {
        timeline.setDurationInSeconds(30);
        timeline.addFlags("flag");
        timeline.addCurves("speed", ofRange(0.1, 3));
        timeline.addCurves("gain", ofRange(100, 1000));
        timeline.addColors("bgcolor");
        timeline.addColors("datacolor");
        
        ofAddListener(timeline.events().bangFired, this, &SceneB::getFlag);
        
        //----------
        // setup contents
        //----------
        mHead.createInstance<VoltageIndicator>()->play();
        mBody.createInstance<Ground>();
        mBody.createInstance<DataMatrix>()->play();
        mBody.createInstance<Radar>()->play();
        mBody.createInstance<GenerativeObject>();
        mBody.createInstance<Radiation>()->play();
        mBody.createInstance<PatternExample>()->play();
        
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
        
//        mHead.playAll();
//        mBody.playAll();
    }
    
    void update()
    {
        mHead.update();
        mBody.update();
    }
    
    void draw()
    {
        ofPushStyle();
        ofDisableAntiAliasing();
        
        ofBackground(timeline.getColor("bgcolor"));
        
        mHead.draw(plant::edgeX, plant::edgeY, plant::edgeW, plant::edgeH);
        mBody.draw(plant::bodyX, plant::bodyY, plant::bodyW, plant::bodyH);
        
        ofPopStyle();
    }
    
    void getFlag(ofxTLBangEventArgs & args)
    {
    }
    
    void keyPressed( int key )
    {
        mHead.sendMessageAll(key);
        mBody.sendMessageAll(key);
    }
    
        
};
