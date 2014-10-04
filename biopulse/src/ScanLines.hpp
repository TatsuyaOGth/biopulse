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
    
    class SimpleCircle : public ofxAnimationPrimitives::Instance
    {
        float mX, mY, mRadius;
    public:
        SimpleCircle(float x, float y, float radius)
        {
            mX = x;
            mY = y;
            mRadius = radius;
        }
        void draw()
        {
            ofFill();
            ofCircle(mX, mY, mRadius);
        }
    };
    
    ofxAnimationPrimitives::InstanceManager mInstances;

public:
    ofColor mCol;
    int mLineWidth;
    
public:
    
    
    ScanLines()
    {
        mCol.set(0, 0, 0);
        mLineWidth = 1;
    }
    
    void update()
    {
        mInstances.update();
    }
    
    void draw()
    {
        ofSetColor(mCol);
        ofSetLineWidth(mLineWidth);
        mInstances.draw();
    }
    
    void createVerticalScanLine(float startH, float stopH, bool right, float duration)
    {
        mInstances.createInstance<VerticalScanLine>(this, startH, stopH, right)->play(duration);
    }
    
    void createSimpleCircle(float x, float y, float radius, float duration) {
        mInstances.createInstance<SimpleCircle>(x, y, radius)->play(duration);
    }
    
    void setColor(ofColor & col) { mCol.set(col); }
};
