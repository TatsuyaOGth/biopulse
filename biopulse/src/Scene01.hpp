#pragma once

#include "BaseSceneInterface.hpp"
#include "ofxEasingCam.h"
#include "ofxAnimationPrimitives.h"
#include "Ground.hpp"


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


class Scene01 : public BaseSceneInterfase
{
    
    share::datasetPtr * mDataset;
    deque<string> mDataList;
    ofxAnimationPrimitives::InstanceManager mScanLines;
    ofxAnimationPrimitives::InstanceManager mContents;
    
public:
    
    void setup()
    {
        mDataset = &share::datasets[0];
        mDataset->get()->setBufferLemgth(256);
        mDataset->get()->stop();
        mDataset->get()->setGain(200);
        
        timeline.setDurationInSeconds(30);
        timeline.addFlags("flag");
        timeline.addCurves("speed", ofRange(0.1, 3));
        timeline.addCurves("gain", ofRange(100, 1000));
        timeline.addColors("bgcolor");
        timeline.addColors("datacolor");
        
        ofAddListener(timeline.events().bangFired, this, &Scene01::getFlag);
    }
    
    void update()
    {
        mDataset->get()->setSpeed(timeline.getValue("speed"));
        mDataset->get()->setGain(timeline.getValue("gain"));
        
        DataController::Data * d = mDataset->get()->getDataOnStartPoint();
        string idx = ofToString(mDataset->get()->offset, 100);
        mDataList.push_back(idx + " date: " + d->timestamp + " voltage: " + ofToString(d->voltage));
        if (mDataList.size() > 36) {
            mDataList.pop_front();
        }
        
//        for (deque<ScanLinePtr>::iterator it = mScanLines.begin(); it != mScanLines.end(); it++) {
//            (*it)->update();
//        }
        mScanLines.update();
        mContents.update();
    }
    
    void draw()
    {
        ofPushStyle();
        ofDisableAntiAliasing();
        
        ofBackground(timeline.getColor("bgcolor"));
        drawGraphLine();
        drawGraph();
//        drawVoltage();
        mScanLines.draw();
        
//        drawPlane();

        drawDataList();
        drawCircleGraph();
        
        ofPopStyle();
    }
    
    void getFlag(ofxTLBangEventArgs & args)
    {
        if (args.flag == "start") {
            mDataset->get()->play();
        }
        if (args.flag == "stop") {
            mDataset->get()->stop();
        }
    }
    
    void keyPressed( int key )
    {
        if (key == '1') mScanLines.createInstance<ScanLine>(ofRandom(plant::edgeW - 20), ofRandom(-5, 5), 5)->playInfinity();
        if (key == '2') mScanLines.release<ScanLine>();
    }

    
    
    //====================================================================================================
    //
    //      PLANT EDGE
    //
    //====================================================================================================
    void drawGraph()
    {
        ofColor col(timeline.getColor("datacolor"));
        ofSetColor(255, 255, 255, col.a);
        float l = 25;
        float x1 = plant::edgeRect.getMaxX();
        float y1 = plant::edgeRect.getCenter().y + mDataset->get()->getVoltageOnStartPoint() - 10;
        float x2 = plant::edgeRect.getMaxX() - l;
        float y2 = plant::edgeRect.getCenter().y + mDataset->get()->getVoltageOnStartPoint();
        float x3 = plant::edgeRect.getMaxX();
        float y3 = plant::edgeRect.getCenter().y + mDataset->get()->getVoltageOnStartPoint() + 10;

        ofTriangle(x1, y1, x2, y2, x3, y3);
        ofSetColor(col);
        ofNoFill();
        mDataset->get()->draw(plant::edgeX, plant::edgeY, plant::edgeW - l, plant::edgeH);

    }

    
    void drawVoltage()
    {
        ofSetColor(255, 255, 255);
        ofFill();
        ofCircle(plant::getEdgeCenterX(), plant::getEdgeCenterY() + mDataset->get()->getVoltage(), 10);
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
    
    
    
    //====================================================================================================
    //
    //      PLANT BODY
    //
    //====================================================================================================
    
    void drawDataList()
    {
        ofSetColor(0, 255, 0);
        
        int x = plant::bodyXW * 0.2;
        int y = plant::bodyY + 10;
        for (int i = 0; i < mDataList.size(); i++) {
            ofDrawBitmapString(mDataList[i], x, y + (i * 16));
        }
    }
    
    void drawCircleGraph()
    {
        
    }
    

        
};
