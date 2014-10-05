#pragma once

#include "BaseContentsInterface.h"

class EmittingObject : public BaseContentsInterface
{

    ofShader mShader;
    float resolution[2];
    float mousePos[2];
    
public:
    float mBaseSize;
    float mSize;
    
public:
    EmittingObject()
    {
        ASSERT(mShader.load("myShaders/empty.vert", "myShaders/emit.frag"));
        mSize = 80;
    }
    
    void update()
    {
        mBaseSize = (sin(share::elapsedTimef) * 5);
        mBaseSize += mSize;
    }
    
    void draw()
    {
        ofSetColor(255, 255, 255, 255);
        ofFill();
        
        mShader.begin();
        
        
        resolution[0] = getWidth();
        resolution[1] = getHeight();
        float time = ofGetElapsedTimef();
        
        mousePos[0] = (float)ofGetMouseX() / (float)ofGetWidth();
        mousePos[1] = -((float)ofGetMouseY() / (float)ofGetHeight()) + 1.;
        
        mShader.setUniform1f("time", time);
        mShader.setUniform2fv("resolution", resolution);
        mShader.setUniform2fv("mouse", mousePos);
        mShader.setUniform1f("size", mBaseSize);
        
        //    glEnable(GL_DEPTH_TEST);
        glBegin(GL_TRIANGLE_STRIP);
        glVertex2f(0, 0);
        glVertex2f(getWidth(), 0);
        glVertex2f(0, getWidth());
        glVertex2f(getWidth(), getHeight());
        glEnd();
        mShader.end();
    }
    
    void setSize(float size)
    {
        mSize = size;
    }
};
