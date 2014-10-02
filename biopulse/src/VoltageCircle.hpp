#pragma once

#include "BaseContentsInterface.h"
#include "DataController.hpp"
#include "ofxExtras.h"

class VoltageCircle : public BaseContentsInterface
{
    vector<float> mVoltages;
    float mRot;
    float mRotSpeed;
    
public:
    
    VoltageCircle()
    {
        mRot = 0;
        mRotSpeed = 0.5;
    }
    
    void update()
    {
        mVoltages.clear();
        for (DATASET_IT it = DATASET.begin(); it != DATASET.end(); it++) {
            mVoltages.push_back((*it)->getCurrentData()->voltage);
        }
        
        mRot += mRotSpeed;
    }
    
    void draw()
    {
        ofSetColor(0);

        ofPushMatrix();
        ofDisableAntiAliasing();
        ofTranslate(getWidth() * 0.5, getHeight() * 0.5);
        
        for (int i = 0; i < 360; i += 2) {
            ofPushMatrix();
            ofRotateZ(i);
            float len1, len2;
            if (i % 90 == 0) {
                len1 = 100;
                len2 = 220;
            } else if (i % 30 == 0) {
                len1 = 120;
                len2 = 160;
            } else if (i % 10 == 0) {
                len1 = 120;
                len2 = 140;
            } else {
                len1 = 120;
                len2 = 125;
            }
            ofLine(0, -len1, 0, -len2);
            ofPopMatrix();
        }
        
        ofPushMatrix();
        ofRotateZ(mRot);
        for (int i = 0; i < 360; i += 30) {
            ofPushMatrix();
            ofRotateZ(i);
            ofLine(0, -120, 0, -140);
            ofPopMatrix();
        }
        ofPopMatrix();
        
        ofPushMatrix();
        ofRotateZ(-mRot * 0.5);
        for (int i = 0; i < 360; i += 60) {
            ofPushMatrix();
            ofRotateZ(i);
            ofLine(0, -120, 0, -160);
            ofPopMatrix();
        }
        ofPopMatrix();
        
        ofSetCircleResolution(90);
        ofNoFill();
        ofCircle(0, 0, 100);
        ofCircle(0, 0, 125);
        
        for (vector<float>::iterator it = mVoltages.begin(); it != mVoltages.end(); it++) {
            ofPushMatrix();
            ofRotateZ(*it * 360 * 2);
            ofFill();
            ofTriangle(0, -118, -3, -108, 3, -108);
            string s(ofToString(*it));
            ofRotateZ(90);
            share::font.drawString(s, -95, 5);
            ofPopMatrix();
        }
        
        ofPopMatrix();

    }
};