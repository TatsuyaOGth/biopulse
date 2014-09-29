#pragma once

#include "BaseSceneInterface.hpp"
#include "DataLebel.hpp"
#include "Ocsillators.hpp"

class SceneA : public BaseSceneInterfase
{
    BaseContentsController mHead;
    BaseContentsController mBody;
    
    
public:
    
    void setup()
    {
        
        //----------
        // setup contents
        //----------
        mHead.createInstance<DataLabel>();
        mBody.createInstance<Ocsillators>();
        
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
    }
    
    void draw()
    {
        unsigned char g = timeline.getValue("bgbri");
        mHead.setBackgroundColor(ofColor(g,g,g,g));
        mBody.setBackgroundColor(ofColor(g,g,g,g));
        
        mHead.draw(plant::edgeX, plant::edgeY, plant::edgeW, plant::edgeH);
        mBody.draw(plant::bodyX, plant::bodyY, plant::bodyW, plant::bodyH);
    }
    
    void keyPressed( int key )
    {
        mHead.sendMessageAll(key);
        mBody.sendMessageAll(key);
    }
    
    void getFlag(ofxTLBangEventArgs & args)
    {
    }
};