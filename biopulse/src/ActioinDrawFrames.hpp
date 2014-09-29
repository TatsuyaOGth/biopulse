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
        const bool * bUpdate;
        vector<ofImage> * mFramesPtr;
        
    public:
        
        Frame(int x, int y, float scale, float deg, vector<ofImage> * framesPtr, bool * updateTog):
        mPos(ofVec2f(x, y)),
        mScale(scale),
        mRot(deg),
        mFramesPtr(framesPtr),
        bUpdate(updateTog)
        {
            mFrameCount = 0;
            mUpdateCount = 0;
        }
        void update()
        {
            if (*bUpdate) {
                mUpdateCount++;
                if (mUpdateCount % 3 == 0) {
                    if (mFrameCount < mFramesPtr->size() - 1) mFrameCount++;
                }
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
    bool bDraw;
    bool bEdge;
    ofShader mShader;
    ofFbo targetBuffer;
    ofFbo ShadingBuffer;
    
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
        
        mScale = 1.0;
        bDraw = false;
        bEdge = false;
        ASSERT(mShader.load("myShaders/outline.vert", "myShaders/outline.frag"));
        
        targetBuffer.allocate(getWidth(), getHeight());
        ShadingBuffer.allocate(getWidth(), getHeight());
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
            
            int i = (int)ofRandom(mFrameImgs.size());
            //        x = debugMouseX(getWidth());
            //        y = debugMouseY(getHeight());
            mFrames.createInstance<Frame>(x, y, mScale, ofRandom(360), &mFrameImgs[i], &bDraw)->play(14);
        }
        mFrames.update();
        
    }
    
    void draw()
    {
        
        targetBuffer.begin();
        ofBackground(mBGColor);
        ofSetColor(255, 255, 255, 255);
        mFrames.draw();
        targetBuffer.end();
        
        if (bEdge) {
            ofFill();
            mShader.begin();
            mShader.setUniformTexture("image", targetBuffer, 0);
            mShader.setUniform1f("th", 0.025);
            
            ShadingBuffer.begin();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            ofRect(0, 0, getWidth(), getHeight());
            ShadingBuffer.end();
            mShader.end();
            
            ShadingBuffer.draw(0, 0);
        } else {
            ofSetColor(255, 255, 255, 255);
            targetBuffer.draw(0 ,0);
        }
        
    }
    
    void gotMessage(int msg)
    {
    }
    
    void setScale(float scale)
    {
        mScale = scale;
    }
    
    void setDraw(bool b)
    {
        bDraw = b;
    }
    
    void setEdge(bool b)
    {
        bEdge = b;
    }
};
