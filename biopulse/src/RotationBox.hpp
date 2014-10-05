#pragma once

#include "BaseContentsInterface.h"
#include "ofxEasingCam.h"

class RotationBox : public BaseContentsInterface
{
    
    ofVec3f previous, current;
    ofxEasingCam cam;
    ofLight lig;
    float mSpeed;
    
    ofBoxPrimitive mBox;
    
public:
    float mSize;
    
public:
    RotationBox()
    {
        cam.setup(getWidth(), getHeight());
        lig.setup();
        lig.setPosition(ofVec3f(0, 0, 0));
        lig.setOrientation(ofVec3f(0, 0, 0));
        
        mSpeed = 0.010699;
        mSize = 160;
        
        mBox.setResolution(4);
        mBox.setMode( OF_PRIMITIVE_TRIANGLE_STRIP );
        vector<ofMeshFace> triangles = mBox.getMesh().getUniqueFaces();
        mBox.getMesh().setFromTriangles( triangles, true );

    }
    void setupCam(float w, float h){ cam.setup(w, h); }
    
    void update()
    {
        previous = current;
        
        // generate a noisy 3d position over time
        float t = (2 + share::elapsedTimef) * mSpeed;
        current.x = ofSignedNoise(t, 0, 0);
        current.y = ofSignedNoise(0, t, 0);
        current.z = ofSignedNoise(0, 0, t);
        current *= 1200; // scale from -1,+1 range to -400,+400
        
        cam.setCurrentPos(current);
        cam.update();
        
        lig.setPosition(-current);
        lig.setOrientation(ofVec3f(0, 0, 0));

    }
    
    void draw()
    {
        
        ofPushStyle();
        ofEnableLighting();
        ofEnableDepthTest();
        lig.enable();
        
        cam.begin();
        

        float x = getWidth() * 0.5;
        float y = getHeight() * 0.5;
        float z = 0;
        float w = mSize;
        float h = mSize;
        float d = mSize;
        
//        ofTranslate(x, y, z);
        mBox.set(w, h, d);
        
        ofSetColor(255, 255, 255);
        ofNoFill();
        mBox.drawWireframe();

        
        cam.end();
        
        lig.disable();
        ofDisableLighting();
        ofDisableDepthTest();
        ofPopStyle();
    }
};