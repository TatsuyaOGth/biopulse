#pragma once

#include "BaseSceneInterface.hpp"
//#include "ActionDrawing.hpp"
#include "ActioinDrawFrames.hpp"
#include "EmittingObject.hpp"

class SceneC : public BaseSceneInterfase
{
    
    BaseContentsController mPlant;
    BaseContentsController mBody;
    
    
public:
    
    void setup()
    {
        timeline.setDurationInSeconds(30);
        
        ofAddListener(timeline.events().bangFired, this, &SceneC::getFlag);
        
        //----------
        // setup contents
        //----------
        mPlant.createInstance<EmittingObject>()->play();
        mPlant.createInstance<ActionDrawFrames>()->play();
        
        ofFbo::Settings s1;
        s1.width  = plant::width;
        s1.height = plant::height;
        s1.useDepth = true;
        s1.useStencil = true;
        s1.internalformat = GL_RGBA;
        mPlant.setFboAllocate(s1);
        
        ofFbo::Settings s2;
        s2.width  = plant::bodyW;
        s2.height = plant::bodyH;
        s2.useDepth = true;
        s2.useStencil = true;
        s2.internalformat = GL_RGBA;
        mBody.setFboAllocate(s2);
        
    }
    
    void update()
    {
        mPlant.update();
        mBody.update();
    }
    
    void draw()
    {
        ofPushStyle();
        ofDisableAntiAliasing();
        
        mPlant.draw(0, 0, plant::width, plant::width);
        mBody.draw(plant::bodyX, plant::bodyY, plant::bodyW, plant::bodyH);
        
        ofPopStyle();
    }
    
    void getFlag(ofxTLBangEventArgs & args)
    {
    }
    
    void keyPressed( int key )
    {
        mPlant.sendMessageAll(key);
        mBody.sendMessageAll(key);
    }
    
    
};
