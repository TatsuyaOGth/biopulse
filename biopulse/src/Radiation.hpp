#pragma once

#include "BaseContentsInterface.h"

class Radiation : public BaseContentsInterface
{
    float radius;
    int startAngle;
    int stopAngle;
    int mSpeed;
    vector<vector<float> > mWaves;
    
public:
    
    Radiation()
    {
        radius = 400;
        startAngle = 0;
        stopAngle = 0;
        mSpeed = 1;
    }
    
    void update()
    {
        updateWaves();
        
        startAngle += mSpeed;
        stopAngle = startAngle + data::bufferLength;
//        startAngle = startAngle % 360;
//        stopAngle = stopAngle % 360;
    }
    
    void updateWaves()
    {
        int j = 0;
        mWaves.clear();
        for (DATASET_IT it = DATASET.begin(); it != DATASET.end(); it++) {
            vector<float> v;
            for (int i = 0; i < data::bufferLength; i++) {
                v.push_back((*it)->getTargetData(i)->voltage);
            }
            mWaves.push_back(v);
            j++;
        }
    }
    
    void draw()
    {
        ofPushMatrix();
        ofTranslate(getWidth() * 0.5, getHeight() * 0.5);
        
        

//        mesh.addColor(ofColor(255));
//        mesh.addVertex(ofVec3f(0));
        int br = 2;
        for (int i = 0; i < mWaves.size(); i++) {
            ofVboMesh mesh;
            int l = mWaves[i].size() - 1;
            int size = mWaves[i].size();
            for (int j = 0; j < mWaves[i].size(); j++,l --) {
                float voltage = mWaves[i][l];
                float offset = ((float)ofGetFrameNum() / (float)data::bufferLength) * TWO_PI;
                float f = -ofMap(j, 0, size, 0, 1) * TWO_PI + offset;
                float x = radius * cos(f) * voltage;
                float y = radius * sin(f) * voltage;
                int min = size;
                unsigned char a = j <= min ? ofMap(j, 0, min, 255, 0) : 255;
                mesh.addColor(ofColor(0, 255, 0, a));
                mesh.addVertex(ofVec3f(x, y, 0));
            }
            mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
            mesh.drawWireframe();
//            ofTranslate(ofRandom(-5, 5), 0);
//            mesh.setMode(OF_PRIMITIVE_POINTS);
//            mesh.drawVertices();
        }
        ofPopMatrix();
    }
};