#pragma once

#include "BaseContentsInterface.h"

class GenerativeObject : public BaseContentsInterface
{
    
    float mRadius;
    
    
public:
    
    GenerativeObject()
    {
    }
    
    void update()
    {
        mRadius = DATASET[0]->getCurrentData()->voltage * getHeight();
    }
    
    void draw()
    {
        ofPushMatrix();
        ofSetColor(255);
        ofTranslate(getWidth() * 0.5, getHeight() * 0.5);
        ofRotateY(ofGetFrameNum() * 0.3);
        ofRotateX(ofGetFrameNum() * 0.4);
        float s = 0;
        float t = 0;
        
        ofVboMesh mesh;
        mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
        
        while (t < 180) {
            s += 18;
            t += 1;
            float radianS = ofDegToRad(s);
            float radianT = ofDegToRad(t);
            float thisx = 0 + (mRadius * cos(radianS) * sin(radianT));
            float thisy = 0 + (mRadius * sin(radianS) * sin(radianT));
            float thisz = 0 + (mRadius * cos(radianT));
            mesh.addColor(ofColor(255));
            mesh.addVertex(ofVec3f(thisx, thisy, thisz));
        }
        mesh.draw();
        
        ofPopMatrix();
    }
    
    
};