#pragma once

#include "BaseSceneInterface.hpp"
#include "DataLebel.hpp"
#include "Ocsillators.hpp"
#include "ScanLines.hpp"

class SceneA : public BaseSceneInterfase
{
    BaseContentsController mHead;
    BaseContentsController mBody;
    BaseContentsController mPlant;
    
    ScanLines * mSL;
    
public:
    
    void setup()
    {
        
        //----------
        // setup contents
        //----------
        mHead.createInstance<DataLabel>();
        mBody.createInstance<Ocsillators>();
        mSL = mPlant.createInstance<ScanLines>();
        
        mHead.setBlendMode(OF_BLENDMODE_ADD);
        mBody.setBlendMode(OF_BLENDMODE_ADD);
        mPlant.setBlendMode(OF_BLENDMODE_ADD);
        
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
        s2.width  = plant::width;
        s2.height = plant::height;
        s2.useDepth = true;
        s2.useStencil = true;
        s2.internalformat = GL_RGBA;
        mPlant.setFboAllocate(s3);
        
        mHead.playAll();
        mBody.playAll();
        mPlant.playAll();
        
        //----------
        // setup timeline
        //----------
        timeline.setDurationInSeconds(60);
        timeline.addFlags("flag");
        timeline.addCurves("bgbri", ofRange(0, 255));
        
        ofAddListener(timeline.events().bangFired, this, &SceneA::getFlag);
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
        mHead.setBackgroundColor(ofColor(g,g,g,g));
        mBody.setBackgroundColor(ofColor(g,g,g,g));
//        mPlant.setBackgroundColor(ofColor(g,g,g,g));
        
        mHead.draw(plant::edgeX, plant::edgeY, plant::edgeW, plant::edgeH);
        mBody.draw(plant::bodyX, plant::bodyY, plant::bodyW, plant::bodyH);
        mPlant.draw(0, 0, plant::width, plant::width);
    }
    
    void keyPressed( int key )
    {
        mHead.sendMessageAll(key);
        mBody.sendMessageAll(key);
        
        if (key == 'q') mSL->createVerticalScanLine(0, plant::height, true, 8);
    }
    
    void getFlag(ofxTLBangEventArgs & args)
    {
        if (args.flag == "1") keyPressed('1');
        if (args.flag == "2") keyPressed('2');
        if (args.flag == "sl") keyPressed('q');
    }
};

