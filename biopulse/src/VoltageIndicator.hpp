#pragma once

#include "BaseContentsInterface.h"
#include "ofxAnimationPrimitives.h"
#include "DataController.hpp"

class VoltageIndicator : public BaseContentsInterface
{
    class ScanLine : public ofxAnimationPrimitives::Instance
    {
        VoltageIndicator * p;
        float mPos;
        float mSpeed;
        bool bStay;
        float mWidth;
        
        
    public:
        ScanLine(VoltageIndicator * P, float pos, float speed)
        {
            p = P;
            mPos = pos;
            mSpeed = speed;
            bStay = false;
            mWidth = 2;
        }
        ~ScanLine(){}
        
        void update()
        {
            if (!bStay) {
                mPos += mSpeed;
//                if (mPos < 0 || mPos + mWidth > p->getWidth()) {
//                    mSpeed *= -1;
//                }
                if (mPos + mWidth < 0) mPos = p->getWidth();
                if (mPos > p->getWidth()) mPos = -mWidth;
            }
        }
        
        void draw()
        {
            ofSetColor(0, 255, 255, 255);
            ofFill();
            ofRect(mPos, 15, mWidth, p->getHeight() - 30);
        }
        
        void stay(bool b){ bStay = b; }
        bool isStay() { return bStay; }
        float getPos() { return mPos; }
        void addSpeed(float v){ mSpeed > 0 ?  mSpeed += v : mSpeed -= v; }
    };
    
    ofxAnimationPrimitives::InstanceManager mScanLines;
    vector<ScanLine*> mScanLinePtr;
    
    //=============================================================================
    
    float mMoveRulurX;
    vector<vector<float> > mWaves;
    
public:
    int mNumMaxDataIndex; // 0~
    
public:
    
    VoltageIndicator()
    {
        mMoveRulurX = 0;
        updateWaves();
        
        mNumMaxDataIndex = 0;
    }
    
    void update()
    {
        updateWaves();
        mScanLines.update();
    }
    
    void draw()
    {
        drawGraphLine();
        
        // draw voltage
        for (int i = 0; i < mNumMaxDataIndex; i++) {
            drawGraph(i, (i % 2 == 0) ? false : true );
        }
        // draw scanlines
        mScanLines.draw();
        // draw barcode
        rs::scanedVoltages.clear();
        for (int i = 0; i < mScanLinePtr.size(); i++) {
            if (mScanLinePtr[i]->isStay()) {
                ofSetColor(255, 255, 255);
                float pos = mScanLinePtr[i]->getPos();
                int posi = (int)(pos / (float)data::bufferLength);
                vector<float> v;
                for (int j = 0; j < mNumMaxDataIndex; j++) {
                    if (j >= mWaves.size()) break;
                    float vol = mWaves[j][posi];
                    v.push_back(vol);
                    drawBarcode(ofToBinary(vol), pos, (getHeight() * 0.5) + (vol * getHeight()), 96, 5);
                }
                rs::scanedVoltages.push_back(v);
            }
        }
        
    }
    
    void updateWaves()
    {
        int j = 0;
        mWaves.clear();
        for (DATASET_IT it = DATASET.begin(); it != DATASET.end(); it++) {
            vector<float> v;
            for (int i = 0; i < data::bufferLength; i++) {
                v.push_back((*it)->getTargetData(i)->voltage);
            }
            mWaves.push_back(v);
            j++;
        }
    }
    
    void gotMessage(int msg)
    {
    }
    
    void createScanLine()
    {
        mScanLinePtr.push_back(mScanLines.createInstance<ScanLine>(this, ofRandom(getWidth() - 20), ofRandom(-5, 5)));
        mScanLinePtr.back()->playInfinity();
    }
    
    void removeScanLines()
    {
        mScanLines.release<ScanLine>();
        mScanLinePtr.clear();
    }
    
    void stopScanLines()
    {
        for (int i = 0; i < mScanLinePtr.size(); i++) {
            mScanLinePtr[i]->stay(true);
        }
    }
    
    void moveScanLines()
    {
        for (int i = 0; i < mScanLinePtr.size(); i++) {
            mScanLinePtr[i]->stay(false);
        }
    }
    
    void addSpeedScanLines(float v)
    {
        for (int i = 0; i < mScanLinePtr.size(); i++) {
            mScanLinePtr[i]->addSpeed(v);
        }
    }
    
    void drawGraphLine()
    {
        ofSetLineWidth(1);
        
        float topLane = 15;
        float bottomLane = 15;
        
        
        // ruler
        ofSetColor(60);
        for (int i = 0; i <= getWidth(); i += 40) {
            ofLine(i, 0 + topLane, i, getHeight() - bottomLane);
        }
        ofSetColor(30);
        for (int i = 0 + 20; i <= getWidth(); i += 40) {
            ofLine(i, 0 + topLane, i, getHeight() - bottomLane);
        }
        ofSetColor(30);
        for (int i = 0 + topLane; i <= getHeight() - bottomLane; i += 10) {
            ofLine(0, i, getWidth(), i);
        }
        
        // move ruler
        float moveWidthSize = getWidth() / data::bufferLength;
        float oneWidthSize = getWidth() / 20;
        int offsetFrameMax = (int)ceil(oneWidthSize / moveWidthSize);
        mMoveRulurX += moveWidthSize;
        if (mMoveRulurX > oneWidthSize) mMoveRulurX -= oneWidthSize;
        ofPushMatrix();
        ofSetColor(125);
        ofTranslate(-mMoveRulurX, 0);
        for (int i = 0; i <= getWidth(); i += 40) {
            ofLine(i, 0 + topLane, i, getHeight() - bottomLane);
        }
        ofPopMatrix();
        
        // holizon line
        ofSetColor(99);
        ofLine(0, getHeight() * 0.5, getWidth(), getHeight() * 0.5);
        ofLine(0, 0 + topLane, getWidth(), 0 + topLane);
        ofLine(0, getHeight() - bottomLane, getWidth(), getHeight() - bottomLane);
        
        // panchi
        ofSetColor(0);
        ofRect(0, 0, getWidth(), topLane);
        ofRect(0, getHeight() - bottomLane, getWidth(), getHeight());
        ofSetColor(199);
        int offset = ofGetFrameNum() % 40;
        for (int i = 0 + 20 + offset; i <= getWidth() + offset; i += 40) {
            ofEllipse(i, 0 + (topLane * 0.5), 12, 10);
            ofEllipse(i, getHeight() - (bottomLane * 0.5), 12, 10);
        }
    }
    
    
    void drawGraph(int n, bool reverse = false)
    {
        if (n >= mWaves.size()) return;
        ofSetColor(255, 255, 255);
        
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
        if (reverse) {
            for (int i = mWaves[n].size() - 1; i >= 0 ; i--) {
                int k = ofMap(i, 0, data::bufferLength, getWidth(), 0);
                mesh.addColor(ofColor(0, 255, 0));
                mesh.addVertex(ofVec3f(k, mWaves[n][i] * getHeight(), 0));
            }
        } else {
            int l = mWaves[n].size() - 1;
            for (int j = 0; j < mWaves[n].size(); j++, l--) {
                int k = ofMap(j, 0, data::bufferLength, getWidth(), 0);
                mesh.addColor(ofColor(0, 255, 0));
                mesh.addVertex(ofVec3f(k, mWaves[n][l] * getHeight(), 0));
            }
        }
        ofPushMatrix();
        ofTranslate(0, getHeight() * 0.5);
        mesh.draw();
        ofPopMatrix();
    }
    
    void drawBarcode(string binalyString, float x, float y, float w, float h)
    {
        ofFill();
        float size = w / (float)binalyString.size();
        int i = 0;
        for (string::iterator it = binalyString.begin(); it != binalyString.end(); it++) {
            if ((*it) == '1') ofRect(x + (i * size), y, size - 1, h);
            i++;
        }
    }
    
};
