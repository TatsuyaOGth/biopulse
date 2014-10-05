#pragma once

#include "BaseContentsInterface.h"

class TargetRadar : public BaseContentsInterface
{
public:
    bool bVisible;
    
public:
    TargetRadar()
    {
        bVisible = false;
    }
    
    void update()
    {
        
    }
    
    void draw()
    {
        if (bVisible) {
            ofSetColor(255, 255, 255);
            ofSetLineWidth(1);
            ofSetCircleResolution(60);
            ofNoFill();
            
            float cx = getWidth() * 0.5;
            float cy = getHeight() * 0.5;
            
            ofLine(0, cy, getWidth(), cy);
            ofLine(cx, 0, cx, getHeight());
            
            ofCircle(cx, cy, 30);
//            ofCircle(cx, cy, 25);
            
            ofPushMatrix();
            ofTranslate(cx, cy);
            ofRotateZ((int)(share::elapsedTimef * 2) % 360);
            for (int i = 0; i < 360; i += 15) {
                ofPushMatrix();
                ofRotateZ(i);
                ofLine(0, -25, 0, -35);
                ofPopMatrix();
            }
            ofPopMatrix();
        }
    }
};