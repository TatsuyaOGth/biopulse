#pragma once

#include "BaseContentsInterface.h"
#include "ofxAnimationPrimitives.h"
#include "ofxExtras.h"

class ScanLines : public BaseContentsInterface
{
    //=======================================================================================================
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
            ofSetColor(mPare->mCol);
            ofLine(mPosX, 0, mPosX, mPare->getHeight());
        }
    };
    
    //=======================================================================================================
    class SimpleCircle : public ofxAnimationPrimitives::Instance
    {
        ScanLines * p;
        float mX, mY, mRadius;
    public:
        SimpleCircle(ScanLines * P, float x, float y, float radius)
        {
            p = P;
            mX = x;
            mY = y;
            mRadius = radius;
        }
        void draw()
        {
            ofFill();
            ofSetColor(p->mCol);
            ofCircle(mX, mY, mRadius);
        }
    };
    //=======================================================================================================
    class Strobo : public ofxAnimationPrimitives::Instance
    {
        ScanLines * mPare;
        bool bFade;
    public:
        Strobo(ScanLines * pare, bool fade): mPare(pare), bFade(fade){}
        void draw()
        {
            ofFill();
            if (bFade) ofSetColor(mPare->mCol, getLife() * 255);
            ofRect(0, 0, mPare->getWidth(), mPare->getHeight());
        }
    };
    //=======================================================================================================
    class VerticalSplitWave : public ofxAnimationPrimitives::Instance
    {
        ScanLines * p;
        int r;;
    public:
        VerticalSplitWave(ScanLines * pare):p(pare){ r = ofRandom(DATASET.size()); }
        void draw()
        {
            ofPushStyle();
//            ofSetRectMode(OF_RECTMODE_CENTER);
            ofSetColor(p->mCol);
            for (int i = 0; i < p->getHeight(); i += 3) {
//                float s = sin((double)share::elapsedTimef + i);
                float s = DATASET[r]->getTargetData(i)->voltage;
                float x = (p->getWidth() * 0.5);
                float w = s * 120;
                ofRect(x, i, w, 2);
            }
            ofPopStyle();
        }
    };
    //=======================================================================================================
    class RippleCircle : public ofxAnimationPrimitives::Instance
    {
        ScanLines * p;
        float mSize;
        float mSpeed;
        float mCSize;
    public:
        RippleCircle(ScanLines * _p, float size, float speed):
        p(_p), mSize(size), mSpeed(speed), mCSize(0) {}
        void update()
        {
            mCSize += (mSize - mCSize) * mSpeed;
        }
        void draw()
        {
            unsigned char a = ofxAnimationPrimitives::Easing::Quad::easeOut(getLife()) * 255;
            ofSetColor(p->mCol, a);
            ofNoFill();
            ofSetLineWidth(1);
            ofSetCircleResolution(90);
            ofCircle(p->getWidth() * 0.5, p->getHeight() * 0.5, mCSize);
        }
    };
    //=======================================================================================================
    class RippleArc : public ofxAnimationPrimitives::Instance
    {
        ScanLines * p;
        float mSize;
        float mSpeed;
        float mCSize;
        float mRot;
        float mLen;
    public:
        RippleArc(ScanLines * _p, float size, float speed):
        p(_p), mSize(size), mSpeed(speed), mCSize(0), mRot(ofRandom(360)), mLen(ofRandom(0.5)) {}
        void update()
        {
            mCSize += (mSize - mCSize) * mSpeed;
        }
        void draw()
        {
            unsigned char a = ofxAnimationPrimitives::Easing::Quad::easeOut(getLife()) * 255;
            ofSetColor(p->mCol, a);
            ofNoFill();
            ofSetLineWidth(1);
            ofPushMatrix();
            ofTranslate(p->getWidth() * 0.5, p->getHeight() * 0.5);
            ofRotateZ(mRot);
            ofxArc(mCSize, 0, mLen, 60);
            ofPopMatrix();
        }
    };
    //=======================================================================================================
    class HorizontalBarcord : public ofxAnimationPrimitives::Instance
    {
        ScanLines * p;
    public:
        HorizontalBarcord(ScanLines * P): p(P) {}

        void draw()
        {
            unsigned char a = ofxAnimationPrimitives::Easing::Quad::easeOut(getLife()) * 255;
            ofSetColor(p->mCol, a);
            ofFill();
            vector<vector<float> > & data = rs::scanedVoltages;
            for (int i = 0; i < data.size(); i++) {
                for (int j = 0; j < data[i].size(); j++) {
                    float w = p->getWidth() / (float)data.size();;
                    float h = 4;
                    float x = i * (p->getWidth() / (float)data.size());
                    float y = (j * h) + ((p->getHeight() * 0.5) - (data[i].size() * 0.5 * h));
                    p->drawBarcode(ofToBinary(data[i][j]), x, y, w, h);
                }
            }
        }
    };
    //=======================================================================================================
    class ParticleDot : public ofxAnimationPrimitives::Instance
    {
        ScanLines * p;
        ofVec2f mPos;
        ofVec2f mVec;
        float mReduction;
    public:
        ParticleDot(ScanLines * P): p(P)
        {
            mPos.set(p->getWidth() * 0.5, p->getHeight() * 0.5);
            
            float vx, vy;
            if (ofRandomf() > 0) {
                ofRandomf() > 0 ? vx = ofRandom(-20, -8) : vx = ofRandom(8, 20);
                vy = ofRandom(-20, 20);
            } else {
                ofRandomf() > 0 ? vy = ofRandom(-20, -8) : vy = ofRandom(8, 20);
                vx = ofRandom(-20, 20);
            }
            
//            ofRandomf() > 0 ? vx = ofRandom(-20, -8) : vx = ofRandom(8, 20);
//            ofRandomf() > 0 ? vy = ofRandom(-20, -8) : vy = ofRandom(8, 20);
            mVec.set(vx, vy);
            mReduction = 0.95;
        }
        void update()
        {
            mPos += mVec;
            mVec *= mReduction;
        }
        void draw()
        {
            unsigned char a = 255;
            if (getLife() < 0.3) if (ofRandomuf() > 0.5) return;
            glPointSize(1);
            ofVboMesh mesh;
            mesh.addColor(ofColor(p->mCol, a));
            mesh.addVertex(mPos);
            mesh.setMode(OF_PRIMITIVE_POINTS);
            mesh.draw();
        }
    };
    //=======================================================================================================
    
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
    
    void drawBarcode(string binalyString, float x, float y, float w, float h)
    {
        float size = w / (float)binalyString.size();
        int i = 0;
        for (string::iterator it = binalyString.begin(); it != binalyString.end(); it++) {
//            (*it) == '1' ? ofFill() : ofNoFill();
//            ofRect(x + (i * size), y, size, h);
            if ((*it) == '1') ofRect(x + (i * size), y, size, h);
            i++;
        }
    }
    
    void createVerticalScanLine(float startH, float stopH, bool right, float duration)
    {
        mInstances.createInstance<VerticalScanLine>(this, startH, stopH, right)->play(duration);
    }
    
    void createSimpleCircle(float x, float y, float radius, float duration)
    {
        mInstances.createInstance<SimpleCircle>(this, x, y, radius)->play(duration);
    }
    
    void createStrobo(float duration, bool fade)
    {
        mInstances.createInstance<Strobo>(this, fade)->play(duration);
    }
    
    void createVerticalSplitWave(float duration)
    {
        mInstances.createInstance<VerticalSplitWave>(this)->play(duration);
    }
    
    void createRippleCircles(float size, float speed, float duration)
    {
        mInstances.createInstance<RippleCircle>(this, size, speed)->play(duration);
    }
    
    void createRippleArc(float size, float speed, float duration)
    {
        mInstances.createInstance<RippleArc>(this, size, speed)->play(duration);
    }
    
    void createHorizontalBarcord(float duration)
    {
        mInstances.createInstance<HorizontalBarcord>(this)->play(duration);
    }
    
    void createParticleDot(float duration)
    {
        mInstances.createInstance<ParticleDot>(this)->play(duration);
    }
    
    void setColor(ofColor & col) { mCol.set(col); }
};
