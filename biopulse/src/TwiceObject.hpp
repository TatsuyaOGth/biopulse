#pragma once

#include "BaseContentsInterface.h"
#include "ofxPostGlitch.h"

class TwiceObject : public BaseContentsInterface
{
    ofxPostGlitch mGlitch;
    ofFbo mFbo;
    
public:
    TwiceObject()
    {
    }
    
    void setupedFbo()
    {
        if (!mFbo.isAllocated()) {
            mFbo.allocate(getWidth(), getHeight(), GL_RGBA);
            mGlitch.setup(&mFbo);
            mGlitch.setFx(OFXPOSTGLITCH_TWIST, true);
        }
    }
    
    void draw()
    {
        mFbo.begin();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ofSetColor(255, 255, 255);
        ofSetLineWidth(4);
        ofLine(0, getHeight() * 0.5, getWidth(), getHeight() * 0.5);
        mFbo.end();
        
        mGlitch.generateFx();
        
        mFbo.draw(0, 0);
    }
};