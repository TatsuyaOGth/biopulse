#pragma once

#include "ofxAnimationPrimitives.h"
#include "DataController.hpp"

class ScanLine : public ofxAnimationPrimitives::Instance
{
    float mPos;
    float mSpeed;
    float mWidth;
    bool bStay;
    
    
public:
    ScanLine(float pos, float speed, float width)
    {
        mPos = pos;
        mSpeed = speed;
        mWidth = width;
        bStay = false;
    }
    ~ScanLine(){}
    
    void update()
    {
        if (!bStay) {
            mPos += mSpeed;
            if (mPos < 0 || mPos + mWidth > plant::edgeW) {
                mSpeed *= -1;
            }
        }
    }
    
    void draw()
    {
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(plant::edgeX, plant::edgeY);
        ofSetColor(0, 0, 255, 200);
        ofFill();
        ofRect(mPos, 0, mWidth, plant::edgeH);
        ofPopStyle();
        ofPopMatrix();
    }
    
    void stay(bool b)
    {
        bStay = b;
    }
};


class VoltageIndicator : public BaseContentsInterface
{
    ofxAnimationPrimitives::InstanceManager mScanLines;
    
public:
    
    VoltageIndicator()
    {
    }
    
    void update()
    {
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
        if (msg == 1) mScanLines.createInstance<ScanLine>(ofRandom(plant::edgeW - 20), ofRandom(-5, 5), 5)->playInfinity();
        if (msg == 2) mScanLines.release<ScanLine>();
    }
    
    
    void drawGraph()
    {
        ofSetColor(255, 255, 255);
//        float l = 25;
//        float x1 = plant::edgeRect.getMaxX();
//        float y1 = plant::edgeRect.getCenter().y + mDataset->get()->getVoltageOnStartPoint() - 10;
//        float x2 = plant::edgeRect.getMaxX() - l;
//        float y2 = plant::edgeRect.getCenter().y + mDataset->get()->getVoltageOnStartPoint();
//        float x3 = plant::edgeRect.getMaxX();
//        float y3 = plant::edgeRect.getCenter().y + mDataset->get()->getVoltageOnStartPoint() + 10;
//        ofTriangle(x1, y1, x2, y2, x3, y3);
        
        ofSetColor(0, 255, 0);
        ofNoFill();
//        mDataset->get()->draw(plant::edgeX, plant::edgeY, plant::edgeW - l, plant::edgeH);
        
    }
    
    void drawGraphLine()
    {
        ofSetLineWidth(1);
        
        float topLane = 15;
        float bottomLane = 15;
        
        
        // ruler
        ofSetColor(60);
        for (int i = plant::edgeX; i <= plant::edgeXW; i += 40) {
            ofLine(i, plant::edgeY + topLane, i, plant::edgeYH - bottomLane);
        }
        ofSetColor(30);
        for (int i = plant::edgeX + 20; i <= plant::edgeXW; i += 40) {
            ofLine(i, plant::edgeY + topLane, i, plant::edgeYH - bottomLane);
        }
        ofSetColor(30);
        for (int i = plant::edgeY + topLane; i <= plant::edgeYH - bottomLane; i += 10) {
            ofLine(plant::edgeX, i, plant::edgeXW, i);
        }
        
        // holizon line
        ofSetColor(80);
        ofLine(plant::edgeX, plant::edgeH * 0.5, plant::edgeW, plant::edgeH * 0.5);
        ofLine(plant::edgeX, plant::edgeY + topLane, plant::edgeW, plant::edgeY + topLane);
        ofLine(plant::edgeX, plant::edgeYH - bottomLane, plant::edgeW, plant::edgeYH - bottomLane);
        
        // panchi
        ofSetColor(abs(sin(share::elapsedTime)) * 60 + 20);
        int offset = ofGetFrameNum() % 40;
        for (int i = plant::edgeX + 20 + offset; i <= plant::edgeXW + offset; i += 40) {
            ofEllipse(i, plant::edgeY + (topLane * 0.5), 12, 10);
            ofEllipse(i, plant::edgeYH - (bottomLane * 0.5), 12, 10);
        }
    }
    
};
