#pragma once

#include "BaseContentsInterface.h"

class Radiation : public BaseContentsInterface
{
    dataset_type data;
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
        mWaves.resize(DATASET.size(), vector<float>(data::bufferLength));
    }
    
    void update()
    {
        updateWaves();
        
        startAngle += mSpeed;
        stopAngle = startAngle + data.size();
//        startAngle = startAngle % 360;
//        stopAngle = stopAngle % 360;
    }
    
    void updateWaves()
    {
        int j = 0;
        for (DATASET_IT it = DATASET.begin(); it != DATASET.end(); it++) {
            for (int i = 0; i < data::bufferLength; i++) {
                mWaves[j][i] = ((*it)->getTargetData(i)->voltage);
            }
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
            for (int j = 0; j < mWaves[i].size(); j++) {
                int size = mWaves[i].size();
                float voltage = mWaves[i][j];
//                float f = -ofMap(j, 0, size, ofDegToRad((float)startAngle), ofDegToRad((float)stopAngle));
                float f = ofMap(j, 0, size, 0, 2 );
                float x = radius * cos(ofRadToDeg(f)) * voltage;
                float y = radius * sin(ofRadToDeg(f)) * voltage;
                mesh.addColor(ofColor(255));
                mesh.addVertex(ofVec3f(x, y, 0));
            }
            mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
            mesh.drawWireframe();
//            ofTranslate(ofRandom(-5, 5), 0);
//            mesh.setMode(OF_PRIMITIVE_POINTS);
//            mesh.drawVertices();
        }
        ofPopMatrix();
        
        for (int i = 0; i < rs::scanedVoltages.size(); i++) {
            for (int j = 0; j < rs::scanedVoltages[i].size(); j++) {
                ofDrawBitmapString(ofToString(rs::scanedVoltages[i][j]), i * 20, i * 16);
            }
        }
    }
};