#pragma once

#include "BaseContentsInterface.h"

class Radiation : public BaseContentsInterface
{
    dataset_type data;
    float radius;
    int startAngle;
    int stopAngle;
    int mSpeed;
    
public:
    
    Radiation()
    {
        radius = 1200;
        startAngle = 0;
        stopAngle = 0;
        mSpeed = 1;
    }
    
    void update()
    {
        data.clear();
        for (int i = 0; i < data::bufferLength; i++) {
            data.push_back(DATASET[0]->getTargetData(i));
        }
        
        startAngle += mSpeed;
        stopAngle = startAngle + data.size();
//        startAngle = startAngle % 360;
//        stopAngle = stopAngle % 360;
    }
    
    void draw()
    {
        ofPushMatrix();
        ofTranslate(getWidth() * 0.5, getHeight() * 0.5);
        
        ofVboMesh mesh;
//        mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);

//        mesh.addColor(ofColor(255));
//        mesh.addVertex(ofVec3f(0));
        int br = 2;
        int n = data.size();
        for (int i = 0; i < n; i += br) {
            float f = -ofMap(i, 0, n, ofDegToRad((float)startAngle), ofDegToRad((float)stopAngle));
            float x = radius * cos(f) * data[i]->voltage;;
            float y = radius * sin(f) * data[i]->voltage;
            mesh.addColor(ofColor(255));
            mesh.addVertex(ofVec3f(x, y, 0));
        }
        mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
        mesh.drawWireframe();
        ofTranslate(ofRandom(-5, 5), 0);
        mesh.setMode(OF_PRIMITIVE_POINTS);
        mesh.drawVertices();
        ofPopMatrix();
    }
};