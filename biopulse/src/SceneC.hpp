#pragma once

#include "BaseSceneInterface.hpp"
//#include "ActionDrawing.hpp"
#include "ActioinDrawFrames.hpp"
#include "EmittingObject.hpp"

class SceneC : public BaseSceneInterfase
{
    
    BaseContentsController mPlant;
    BaseContentsController mBody;
    
    EmittingObject * mEO;
    ActionDrawFrames * mADF;
    
public:
    
    void setup()
    {
        
        //----------
        // setup contents
        //----------
        mEO  = mPlant.createInstance<EmittingObject>();
        mADF = mPlant.createInstance<ActionDrawFrames>();
        mPlant.playAll();
        
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
        
        //----------
        // setup timeline
        //----------
        timeline.setDurationInSeconds(30);
        timeline.addFlags("flag");
        timeline.addCurves("bgbri", ofRange(0, 255));
        timeline.addCurves("fgbri", ofRange(0, 255));

        
        ofAddListener(timeline.events().bangFired, this, &SceneC::getFlag);

    }
    
    void update()
    {
        mPlant.update();
        mBody.update();
    }
    
    void draw()
    {
        unsigned char g = timeline.getValue("bgbri");
        mPlant.setBackgroundColor(ofColor(g,g,g,g));
                      g = timeline.getValue("fgbri");
        mPlant.setForegroundColor(ofColor(g,g,g,g));
        
        mPlant.draw(0, 0, plant::width, plant::width);
        mBody.draw(plant::bodyX, plant::bodyY, plant::bodyW, plant::bodyH);
        
    }
    
    void getFlag(ofxTLBangEventArgs & args)
    {
        if (args.flag == "drawstart") {
            mADF->setDraw(true);
            mADF->reset();
        }
        if (args.flag == "drawstop") {
            mADF->setDraw(false);
        }
    }
    
    void keyPressed( int key )
    {
        mPlant.sendMessageAll(key);
        mBody.sendMessageAll(key);
    }
    
    
};
