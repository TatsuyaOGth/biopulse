#pragma once

#include "../../common.h"
#include "ofxEasingCam.h"
#include "ofxAnimationPrimitives.h"


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

ofxAnimationPrimitives::InstanceManager mScanLines;

class Scene01 : public BaseSceneInterfase
{
    
    share::datasetPtr * mDataset;
    deque<string> mDataList;
    
    
    ofVec3f previous, current;
    ofxEasingCam cam;
    ofLight lig;
    ofParameter<float> mSpeed;
    ofParameter<ofVec3f> mLigPos;
    ofParameter<ofVec3f> mLigDirection;
    ofFbo mPlaneFbo;
    
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
        
        cam.setup(plant::bodyW, plant::bodyH);
        lig.setup();
        lig.setDirectional();
        mSpeed = 0.010699;
        mLigPos = ofVec3f(237.651, 600, -237.151);
        mLigDirection = ofVec3f(266.289, 3.89838, -89.8126);
        mPlaneFbo.allocate(plant::bodyW, plant::bodyH, GL_RGBA);
    }
    
    void update()
    {
        mDataset->get()->setSpeed(timeline.getValue("speed"));
        mDataset->get()->setGain(timeline.getValue("gain"));
        
        DataController::Data & d = mDataset->get()->getDataOnStartPoint();
        string idx = ofToString(mDataset->get()->offset, 100);
        mDataList.push_back(idx + " date: " + d.timestamp + " voltage: " + ofToString(d.voltage));
        if (mDataList.size() > 36) {
            mDataList.pop_front();
        }
        
//        for (deque<ScanLinePtr>::iterator it = mScanLines.begin(); it != mScanLines.end(); it++) {
//            (*it)->update();
//        }
        mScanLines.update();
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
        
        drawPlane();
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
    
    
    void drawPlane()
    {
        // update
        previous = current;
        
        // generate a noisy 3d position over time
        float t = (2 + share::elapsedTime) * mSpeed;
        current.x = ofSignedNoise(t, 0, 0);
        current.y = ofSignedNoise(0, t, 0);
        current.z = ofSignedNoise(0, 0, t);
        current *= 1200; // scale from -1,+1 range to -400,+400
        
        cam.setPos(current);
        cam.update();
        
        lig.setPosition(mLigPos);
        lig.setOrientation(mLigDirection);
        
        // draw
        mPlaneFbo.begin();
        ofBackground(0, 0, 0);
        ofSetColor(255, 255, 255);
        ofPushStyle();
        ofNoFill();
        ofEnableLighting();
        ofEnableDepthTest();
        cam.begin();
        lig.enable();
        
        ofSetColor(255, 255, 255, 255);
        drawGrid(1500, 20, true, false, true, false);
        lig.draw();
        lig.disable();
        cam.end();
        ofDisableLighting();
        ofDisableDepthTest();
        
        
        drawRaderWindowMarks(6, (abs(sin(share::elapsedTime))) * 20 + 5);
        ofPopStyle();
        mPlaneFbo.end();
        
        mPlaneFbo.draw(plant::bodyX, plant::bodyY);

    }

    
    
    void drawGrid(float scale, float ticks, bool labels, bool x, bool y, bool z)
    {
        ofPushStyle();
        
        if (x) {
            drawGridPlane(scale, ticks, labels);
        }
        if (y) {
            ofPushMatrix();
            ofRotate(90, 0, 0, -1);
            drawGridPlane(scale, ticks, labels);
            ofPopMatrix();
        }
        if (z) {
            ofPushMatrix();
            ofRotate(90, 0, 1, 0);
            drawGridPlane(scale, ticks, labels);
            ofPopMatrix();
        }
        
        if (labels) {
            ofPushStyle();
            ofSetColor(255, 0, 0);
            float labelPos = scale * (1.0f + 0.5f / ticks);
            ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
            ofDrawBitmapString("x", labelPos, 0, 0);
            ofDrawBitmapString("y", 0, labelPos, 0);
            ofDrawBitmapString("z", 0, 0, labelPos);
            ofPopStyle();
        }
        ofPopStyle();
    }
    
    void drawGridPlane(float scale, float ticks, bool labels)
    {
        
        float minor = scale / ticks;
        float major =  minor * 2.0f;
        
        ofPushStyle();
        ofFill();
        for (int iDimension=0; iDimension<2; iDimension++)
        {
            for (float yz=-scale; yz<=scale; yz+= minor)
            {
                
                //major major
                if (fabs(yz) == scale || yz == 0)
                    ofSetLineWidth(2);
                
                //major
                else if (yz / major == floor(yz / major) )
                    ofSetLineWidth(1.5);
                
                //minor
                else
                    ofSetLineWidth(1);
                if (iDimension==0) {
                    ofLine(0, yz, -scale, 0, yz, scale);
                }
                else {
                    ofLine(0, -scale, yz, 0, scale, yz);
                }
                
            }
        }
        ofPopStyle();
        
        if (labels) {
            //draw numbers on axes
            ofPushStyle();
            ofSetColor(0, 255, 0);
            
            float accuracy = ceil(-log(scale/ticks)/log(10.0f));
            
            ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
            for (float yz = -scale; yz<=scale; yz+=minor)
            {
                ofDrawBitmapString(ofToString(yz, accuracy), 0, yz, 0);
                ofDrawBitmapString(ofToString(yz, accuracy), 0, 0, yz);
            }
            ofPopStyle();
        }
    }
    
    void drawRaderWindowMarks(int res, float size)
    {
        int xstep = (int)(plant::bodyW  / res);
        int ystep = xstep;
        
        for (int y = 0; y <= plant::bodyH; y += ystep) {
            for (int x = 0; x <= plant::bodyW; x += xstep) {
                ofLine(x    - (size * 0.5), y, x + (size * 0.5), y);
                ofLine(x, y - (size * 0.5), x, y + (size * 0.5)   );
            }
        }
    }
    
};
