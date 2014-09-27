#pragma once

#include "ofxAnimationPrimitives.h"
#include "DataController.hpp"

class Ocsillators : public BaseContentsInterface
{
    vector<deque<float> > mWaves;
    vector<bool> mAssignQue;
    ofColor mCol;
    
    int mode;
    
public:
    
    Ocsillators()
    {
        mWaves.resize(DATASET.size());
        mAssignQue.resize(mWaves.size(), false);
        mCol.set(255, 255, 255); //tmp
        mode = 0;
    }
    
    void update()
    {
        if (mode == 0) return;
        
        for (int i = 0; i < DATASET.size(); i++) {
            
            Data * data = DATASET[i]->getNextData();
            if (data == NULL) break;
            
            if (mode == 1) {
                mWaves[i].push_back(0.0);
            } else if (mode == 2) {
                if (mAssignQue[i])
                    mWaves[i].push_back(data->voltage);
                else
                    mWaves[i].push_back(0);
            } else if (mode == 3) {
                mWaves[i].push_back(data->voltage);
            }
            
            if (mWaves[i].size() > data::bufferLength) mWaves[i].pop_front();
        }
    }
    
    void draw()
    {
        ofBackground(255, 255, 255);
        if (mode == 0) return;
        
       
        for (int i = 0; i < mWaves.size(); i++) {
            ofMesh mesh;
            mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
            int l = mWaves[i].size() - 1;
            for (int j = 0; j < mWaves[i].size(); j++, l--) {
                int k = ofMap(j, 0, data::bufferLength, getWidth(), 0);
                mesh.addColor(ofColor(0));
                mesh.addVertex(ofVec3f(k, mWaves[i][l] * 200, 0));
            }
            ofPushMatrix();
            int offsetY = (getHeight() / (float)mWaves.size()) * 0.5;
            ofTranslate(0, ofMap(i, 0, mWaves.size(), offsetY, getHeight()));
            mesh.draw();
            ofPopMatrix();
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
        
        if (msg == '2') {
            while (true) {
                int i = ofRandom(mAssignQue.size());
                if (!mAssignQue[i]) {
                    mAssignQue[i] = true;
                    break;
                }
            }
        }
    }
    
};