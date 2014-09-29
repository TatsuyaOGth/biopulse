#pragma once

#include "BaseContentsInterface.h"
#include "ofxAnimationPrimitives.h"

class ActionDrawFrames : public BaseContentsInterface
{
    class Frame : public ofxAnimationPrimitives::Instance
    {
        ofVec2f mPos;
        int mFrameCount;
        int mUpdateCount;
        float mRot;
        float mScale;
        vector<ofImage> * mFramesPtr;
        
    public:
        Frame(int x, int y, float scale, float deg, vector<ofImage> * framesPtr):
        mPos(ofVec2f(x, y)),
        mScale(scale),
        mRot(deg),
        mFramesPtr(framesPtr)
        {
            mFrameCount = 0;
            mUpdateCount = 0;
        }
        void update()
        {
            mUpdateCount++;
            if (mUpdateCount % 3 == 0) {
                if (mFrameCount < mFramesPtr->size() - 1) mFrameCount++;
            }
        }
        void draw()
        {
            unsigned char a = 255;
            if (getLife() < 0.5) {
                a = (unsigned char)(ofMap(getLife(), 0, 0.5, 0, 255, true));
            }
            ofSetColor(255, 255, 255, a);
            ofPushMatrix();
            ofTranslate(mPos);
            ofRotateZ(mRot);
            float w = (*mFramesPtr)[mFrameCount].getWidth();
            float h = (*mFramesPtr)[mFrameCount].getHeight();
            float x = -(w * 0.5);
            float y = -(h * 0.5);
            (*mFramesPtr)[mFrameCount].draw(x, y, w * mScale, h * mScale);
            ofPopMatrix();
        }
    };

    ofxAnimationPrimitives::InstanceManager mFrames;
    vector<vector<ofImage> > mFrameImgs;
    float mScale;
    float mExp;
    bool bDraw;
    
public:
    
    ActionDrawFrames()
    {
        // load frame image
        ofDirectory dir;
        ASSERT(dir.listDir("images") > 0);
        for (int i = 0; i < dir.size(); i++) {
            ofDirectory frameDir;
            ASSERT(frameDir.listDir(dir.getPath(i)) > 0);
            vector<ofImage> mTFrames;
            for (int j = 0; j < frameDir.size(); j++) {
                mTFrames.push_back(ofImage());
                ASSERT(mTFrames.back().loadImage(frameDir.getPath(j)));
            }
            mFrameImgs.push_back(mTFrames);
        }
        
        reset();
        bDraw = false;
    }
    
    void update()
    {
        if (bDraw) {
            float t = share::elapsedTimef;
            float vx = ofNoise(t, 0);
            float vy = ofNoise(0, t);
            float x = (vx - 0.5) * 1000 + (getWidth()/2);
            float y = (vy - 0.5) * 1000 + (getHeight()/2);
//            float rx = ofRandom(-100, 100);
//            float ry = ofRandom(-100, 100);
//            x += rx;
//            y += ry;
            mScale += mExp;
            
            int i = (int)ofRandom(mFrameImgs.size());
            //        x = debugMouseX(getWidth());
            //        y = debugMouseY(getHeight());
            mFrames.createInstance<Frame>(x, y, mScale, ofRandom(360), &mFrameImgs[i])->play(8);
        }
        mFrames.update();
    }
    
    void draw()
    {
        mFrames.draw();
    }
    
    void gotMessage(int msg)
    {
        if (msg == '0') reset();
        if (msg == '1') bDraw = !bDraw;
    }
    
    
    void reset()
    {
        mScale = 0.1;
        mExp = 0.01;
    }
    
    void setDraw(bool b)
    {
        bDraw = b;
    }
};
