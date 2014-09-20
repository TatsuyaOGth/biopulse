#pragma once

#include "../../common.h"

class Scene02 : public BaseSceneInterfase
{
    share::datasetPtr * mDataset;
    
public:
    
    void setup()
    {
        
    }
    
    void update()
    {
        
    }
    
    void draw()
    {
        ofPushStyle();
        
        ofBackground(255, 255, 255);
        ofSetColor(0,0,0,255);
        ofDisableAntiAliasing();
        ofSetLineWidth(1);
        for (int i = 0; i < plant::height; i += 20) {
            ofLine(0, i, plant::width, i);
        }
        
        ofPopStyle();
    }
};