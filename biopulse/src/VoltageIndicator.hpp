#pragma once

#include "BaseContentsInterface.h"
#include "ofxAnimationPrimitives.h"
#include "DataController.hpp"

class VoltageIndicator : public BaseContentsInterface
{
    class ScanLine : public ofxAnimationPrimitives::Instance
    {
        float mPos;
        float mSpeed;
        float mWidth;
        float mHeight;
        float mAreaWidth;
        float mAreaHeight;
        bool bStay;
        
    public:
        ScanLine(float pos, float speed, float width, float height, float areaWidth, float areaHeight)
        {
            mPos = pos;
            mSpeed = speed;
            mWidth = width;
            mHeight = height;
            mAreaWidth = areaWidth;
            mAreaHeight = areaHeight;
            bStay = false;
        }
        ~ScanLine(){}
        
        void update()
        {
            if (!bStay) {
                mPos += mSpeed;
                if (mPos < 0 || mPos + mWidth > mAreaWidth) {
                    mSpeed *= -1;
                }
            }
        }
        
        void draw()
        {
            ofSetColor(0, 0, 255, 200);
            ofFill();
            ofRect(mPos, 0, mWidth, mAreaWidth);
        }
        
        void stay(bool b)
        {
            bStay = b;
        }
    };
    
    ofxAnimationPrimitives::InstanceManager mScanLines;
    
    
    
    
    float mMoveRulurX;
    deque<float> mWave;
    
public:
    
    VoltageIndicator()
    {
        for (int i = 0; i < data::bufferLength; i++) {
            mWave.push_back(DATASET[0]->getNextData()->voltage);
        }
        mMoveRulurX = 0;
    }
    
    void update()
    {
        mWave.push_back(DATASET[0]->getNextData()->voltage);
        if (mWave.size() > data::bufferLength) mWave.pop_front();
        
        mScanLines.update();
    }
    
    void draw()
    {
        drawGraphLine();
        drawGraph();
        mScanLines.draw();
    }
    
    void gotMessage(int msg)
    {
        if (msg == 1) mScanLines.createInstance<ScanLine>(
                                                          ofRandom(getWidth() - 20),
                                                          ofRandom(-5, 5),
                                                          5,
                                                          getHeight() - 20,
                                                          getWidth(),
                                                          getHeight()
                                                          )->playInfinity();
        if (msg == 2) mScanLines.release<ScanLine>();
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
        float oneWidthSize = getWidth() / 40;
        int offsetFrameMax = (int)ceil(oneWidthSize / moveWidthSize);
        mMoveRulurX += moveWidthSize;
        if (mMoveRulurX > oneWidthSize) mMoveRulurX -= oneWidthSize;
        ofPushMatrix();
        ofSetColor(90);
        ofTranslate(-mMoveRulurX, 0);
        for (int i = 0; i <= getWidth(); i += 40) {
            ofLine(i, 0 + topLane, i, getHeight() - bottomLane);
        }
        ofPopMatrix();
        
        // holizon line
        ofSetColor(80);
        ofLine(0, getHeight() * 0.5, getWidth(), getHeight() * 0.5);
        ofLine(0, 0 + topLane, getWidth(), 0 + topLane);
        ofLine(0, getHeight() - bottomLane, getWidth(), getHeight() - bottomLane);
        
        // panchi
        ofSetColor(199);
        ofRect(0, 0, getWidth(), topLane);
        ofRect(0, getHeight() - bottomLane, getWidth(), getHeight());
        ofSetColor(0);
        int offset = ofGetFrameNum() % 40;
        for (int i = 0 + 20 + offset; i <= getWidth() + offset; i += 40) {
            ofEllipse(i, 0 + (topLane * 0.5), 12, 10);
            ofEllipse(i, getHeight() - (bottomLane * 0.5), 12, 10);
        }
        
    }
    
    
    void drawGraph()
    {
        ofSetColor(255, 255, 255);
//        float l = 25;
//        float x1 = getWidth();
//        float y1 = plant::edgeRect.getCenter().y + mDataset->get()->getVoltageOnStartPoint() - 10;
//        float x2 = plant::edgeRect.getMaxX() - l;
//        float y2 = plant::edgeRect.getCenter().y + mDataset->get()->getVoltageOnStartPoint();
//        float x3 = plant::edgeRect.getMaxX();
//        float y3 = plant::edgeRect.getCenter().y + mDataset->get()->getVoltageOnStartPoint() + 10;
//        ofTriangle(x1, y1, x2, y2, x3, y3);
        
//        ofSetColor(0, 255, 0);
//        ofNoFill();
//        mDataset->get()->draw(0, 0, getWidth() - l, getHeight());
        
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
        int l = mWave.size() - 1;
        for (int j = 0; j < mWave.size(); j++, l--) {
            int k = ofMap(j, 0, data::bufferLength, getWidth(), 0);
            mesh.addColor(ofColor(0, 255, 0));
            mesh.addVertex(ofVec3f(k, mWave[l] * getHeight(), 0));
        }
        ofPushMatrix();
        ofTranslate(0, getHeight() * 0.5);
        mesh.draw();
        ofPopMatrix();

    }
    

    
};
