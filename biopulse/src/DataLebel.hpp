#pragma once

#include "ofxAnimationPrimitives.h"
#include "DataController.hpp"

class DataLabel : public BaseContentsInterface
{
    int mode;
    int mCounter;

    typedef struct _line
    {
        float offsetX;
        float hLength;
        int   lineWidth;
        string label;
    } line;
    deque<line> mLines;
    
public:
    
    DataLabel()
    {
        mode = 0;
        mCounter = 0;
    }
    
    void update()
    {
    
        // add line
        line l;
        l.offsetX = getWidth();
        l.label = "";
        l.lineWidth = 1;
        if        ((mCounter % 500) == 0) {
            l.hLength = getHeight() * 0.9;
            l.label = DATASET[0]->getCurrentData()->timestamp;
            l.lineWidth = 2;
        } else if ((mCounter % 100) == 0) {
            l.hLength = getHeight() * 0.5;
        } else if ((mCounter % 50) == 0) {
            l.hLength = getHeight() * 0.25;
        } else if ((mCounter % 10) == 0) {
            l.hLength = getHeight() * 0.2;
        } else if ((mCounter % 2) == 0) {
            l.hLength = getHeight() * 0.15;
        } else {
            l.hLength = 1;
        }
        mLines.push_back(l);
        
        // update lines
        float prog = getWidth() / (float)data::bufferLength;
        for (int i = 0; i < mLines.size(); i++) {
            mLines[i].offsetX -= prog;
        }
        if (mLines.size() > data::bufferLength * 1.5) mLines.pop_front();
        
        mCounter++;
    }
    
    void draw()
    {
        int size = data::bufferLength * 0.5;
        ofNoFill();
        ofSetColor(0);
        
        // draw lines
        for (int i = 0; i < mLines.size(); i++) {
            ofSetLineWidth(mLines[i].lineWidth);
            ofLine(mLines[i].offsetX, getHeight() - mLines[i].hLength, mLines[i].offsetX, getHeight());
            
            if (!mLines[i].label.empty()) {
                share::font.drawString(mLines[i].label, mLines[i].offsetX + 20, getHeight() - 40);
            }
        }
                
    }
    
    void gotMessage(int msg)
    {
        switch (msg) {
            case '0': mode = 0; break;
            case '1': mode = 1; break;
            case '2': mode = 2; break;
            case '3': mode = 3; break;
            case '4': mode = 4; break;
        }
        
        if (msg == '9') data::bufferLength = 90;
    }
    
};