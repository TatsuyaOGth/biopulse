#pragma once

#include "BaseContentsInterface.h"
#include "ofxAnimationPrimitives.h"
#include "DataController.hpp"

class ScanLines : public BaseContentsInterface
{
    class VerticalScanLine : public ofxAnimationPrimitives::Instance
    {
        ScanLines * mPare;
        float mPosX;
        float mStartH;
        float mStopH;
        bool bRight;
        
    public:
        VerticalScanLine(ScanLines * pare, float startH, float stopH, bool right)
        {
            mPare = pare;
            mStartH = startH;
            mStopH = stopH;
            bRight = right;
            mPosX = 0;
        }
        void update()
        {
            if (bRight) {
                mPosX = ofMap(getLife(), 0, 1, mPare->getWidth(), 0);
            } else {
                mPosX = ofMap(getLife(), 0, 1, 0, mPare->getWidth());
            }
        }
        void draw()
        {
            ofLine(mPosX, 0, mPosX, mPare->getHeight());
        }
    };
    ofxAnimationPrimitives::InstanceManager mVerticalScanLines;

    
public:
    void update()
    {
        mVerticalScanLines.update();
    }
    
    void draw()
    {
        ofSetColor(0, 0, 0);
        ofSetLineWidth(1);
        mVerticalScanLines.draw();
    }
    
    void createVerticalScanLine(float startH, float stopH, bool right, float duration)
    {
        mVerticalScanLines.createInstance<VerticalScanLine>(this, startH, stopH, right)->play(duration);
    }
};
