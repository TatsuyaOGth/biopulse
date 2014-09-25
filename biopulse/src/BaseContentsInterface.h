#pragma once

#include "ofxAnimationPrimitives.h"
#include "Constants.h"


class BaseContentsInterface : public ofxAnimationPrimitives::Instance
{
protected:
    ofFbo mFbo;
    float mOffsetX;
    float mOffsetY;
    ofColor mBgColor;
    
public:
    
    BaseContentsInterface(): mOffsetX(0), mOffsetY(0), mBgColor(ofColor(0,0,0,0)){}
    
    void setupFbo(ofFbo::Settings settings, float x = 0, float y = 0)
    {
        mFbo.allocate(settings);
        mOffsetX = x;
        mOffsetY = y;
    }
    
    ofFbo & getFbo() {
        return mFbo;
    }
    
    void drawFbo()
    {
        rendering();
        mFbo.draw(mOffsetX, mOffsetY);
    }
    
    void drawFbo(int x, int y)
    {
        rendering();
        mFbo.draw(x, y);
    }
    
    void rendering()
    {
        if (!mFbo.isAllocated()) return;
        mFbo.begin();
        ofBackground(mBgColor);
        draw();
        mFbo.end();
    }
};


class BaseContentsManager : public ofxAnimationPrimitives::InstanceManager
{
protected:
    vector<BaseContentsInterface *> instances;
    
    template <typename T>
    T* setupInstance(T* o)
    {
        o->class_id = RTTI::getTypeID<T>();
        instances.push_back(o);
        return o;
    }
    
public:
    

};
