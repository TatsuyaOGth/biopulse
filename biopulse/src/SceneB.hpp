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
        //----------
        // setup contents
        //----------
        mHead.createInstance<VoltageIndicator>()->play();
        mBody.createInstance<Ground>()->play();
        mBody.createInstance<DataMatrix>()->play();
        mBody.createInstance<Radar>()->play();
        mBody.createInstance<GenerativeObject>();
        mBody.createInstance<Radiation>()->play();
        mBody.createInstance<PatternExample>();
        
        mBody.setBlendMode(OF_BLENDMODE_ADD);
        
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
        
        //----------
        // setup timeline
        //----------
        timeline.setDurationInSeconds(90);
        timeline.addFlags("flag");
        timeline.addCurves("bgbri", ofRange(0, 255));
        timeline.addColors("datacolor");
        
        ofAddListener(timeline.events().bangFired, this, &SceneB::getFlag);
    }
    
    void update()
    {
        mHead.update();
        mBody.update();
    }
    
    void draw()
    {
        unsigned char g = timeline.getValue("bgbri");
        mHead.setBackgroundColor(ofColor(g,g,g,g));
        mBody.setBackgroundColor(ofColor(g,g,g,g));
        
        ofPushStyle();
        ofEnableAlphaBlending();
        mHead.draw(plant::edgeX, plant::edgeY, plant::edgeW, plant::edgeH);
        mBody.draw(plant::bodyX, plant::bodyY, plant::bodyW, plant::bodyH);
        ofPopStyle();
        
    }
    
    void getFlag(ofxTLBangEventArgs & args)
    {
    }
    
    void keyPressed( int key )
    {
        if (key == 'q') mBody.togglePlay(0);
        if (key == 'w') mBody.togglePlay(1);
        if (key == 'e') mBody.togglePlay(2);
        if (key == 'r') mBody.togglePlay(4);
        
        mHead.sendMessageAll(key);
        mBody.sendMessageAll(key);
    }
    
        
};
