#pragma once

#include "BaseContentsInterface.h"

class EmittingObject : public BaseContentsInterface
{

    ofShader mShader;
    float resolution[2];
    float mousePos[2];
    float mSize;
    
public:
    EmittingObject()
    {
        ASSERT(mShader.load("myShaders/empty.vert", "myShaders/emit.frag"));
        mSize = 80;
    }
    
    void update()
    {
        mSize += sin(share::elapsedTimef) * 20;
    }
    
    void draw()
    {
        ofSetColor(255, 255, 255, 255);
        ofFill();
        
        mShader.begin();
        
        
        resolution[0] = ofGetWidth();
        resolution[1] = ofGetHeight();
        float time = ofGetElapsedTimef();
        
        mousePos[0] = (float)ofGetMouseX() / (float)ofGetWidth();
        mousePos[1] = -((float)ofGetMouseY() / (float)ofGetHeight()) + 1.;
        
        mShader.setUniform1f("time", time);
        mShader.setUniform2fv("resolution", resolution);
        mShader.setUniform2fv("mouse", mousePos);
        mShader.setUniform1f("size", mSize);
        
        //    glEnable(GL_DEPTH_TEST);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex2f(0, 0);
        glVertex2f(ofGetWidth(), 0);
        glVertex2f(0, ofGetWidth());
        glVertex2f(ofGetWidth(), ofGetHeight());
        glEnd();
        mShader.end();
    }
    
    void setSize(float size)
    {
        mSize = size;
    }
};
