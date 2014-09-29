#pragma once

#include "BaseContentsInterface.h"
#include "ofxAnimationPrimitives.h"

class ActionDrawing : public BaseContentsInterface
{
    class Ink : public ofxAnimationPrimitives::Instance
    {
        ofTexture * mTex;
        ofVec2f pos;
        float mRot;
        float mScale;
        
    public:
        Ink(ofTexture * tex, float x, float y, float scale, float rot):
        mTex(tex),
        pos(ofVec2f(x, y)),
        mScale(scale),
        mRot(rot)
        {
        }
        
        void update()
        {
        }
        
        void draw()
        {
            ofPushMatrix();
            ofPushStyle();
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofEnableAlphaBlending();
            if (mTex == NULL) return;
            unsigned char a = (unsigned char)(getLife() * 255);
            ofSetColor(255, 255, 255, a);
            float w = mTex->getWidth();
            float h = mTex->getHeight();
            
            ofTranslate(pos.x, pos.y);
            ofRotateZ(mRot);

            mTex->draw(0, 0, w * mScale, h * mScale);
            ofPopStyle();
            ofPopMatrix();
        }
        
    };
    
    
    ofxAnimationPrimitives::InstanceManager mInks;
    
    float mScale;
    float mExp;
    bool bDraw;
    
public:
    
    ActionDrawing()
    {
        // load ink image
        if (rs::inks.empty()) {
            ofDirectory dir;
            ASSERT(dir.listDir("images") > 0);
            for (int i = 0; i < dir.size(); i++) {
                ofImage img;
                ASSERT(img.loadImage(dir.getPath(i)));
                rs::inks.push_back(ofTexture());
                rs::inks.back().allocate(img.getWidth(), img.getHeight(), GL_RGBA);
                rs::inks.back().loadData(img.getPixelsRef());
            }
        }
        
        reset();
        bDraw = true;
    }
    
    void update()
    {
        if (bDraw) {
            float t = share::elapsedTimef * 2;
            float vx = ofNoise(t, 0);
            float vy = ofNoise(0, t);
            float x = (vx - 0.5) * 1000 + (getWidth()/2);
            float y = (vy - 0.5) * 1000 + (getHeight()/2);
            float rx = ofRandom(-100, 100);
            float ry = ofRandom(-100, 100);
            x += rx;
            y += ry;
            mScale += mExp;
            
            int i = (int)ofRandom(rs::inks.size());
            //        x = debugMouseX(getWidth());
            //        y = debugMouseY(getHeight());
            mInks.createInstance<Ink>(&rs::inks[i], x, y, mScale, ofRandom(360))->play(3);
        }
        mInks.update();
    }
    
    void draw()
    {
        mInks.draw();
    }
    
    void gotMessage(int msg)
    {
        if (msg == '0') reset();
        if (msg == '1') bDraw = !bDraw;
    }

    
    void reset()
    {
        mScale = 0.1;
        mExp = 0.005;
    }
};