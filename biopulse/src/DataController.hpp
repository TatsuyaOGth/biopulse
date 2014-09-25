#pragma once

#include "ofMain.h"
#include "common.h"

class DataController
{
    float tmpOffset;
    
public:
    float width, height, speed, gain;
    const string mDataPath;
    int bufferLength;
    bool bPlay;
    int offset;
    
    typedef struct _Data
    {
        string timestamp;
        float voltage;
    } Data;
    vector<Data *> data;
    
    DataController(const string & path, bool header = true): mDataPath(path)
    {
        ofFile file(path);
        ASSERT(file.exists());
        ofBuffer buffer(file);
        unsigned long tmpCount = 0;
        while (!buffer.isLastLine()) {
            string line = buffer.getNextLine();
            if (header) {
                header = false;
                continue;
            }
            string replaceTarget = "\"";
            string::size_type pos = line.find(replaceTarget);
            while (pos != string::npos) {
                line.replace(pos, replaceTarget.size(), "");
                pos = line.find(replaceTarget, pos);
            }
            vector<string> col = ofSplitString(line, ",");
            if (col.size() < 2) {
                LOG_WARNING << "column is low";
                continue;
            }
//            Data d = {col[0], ofToFloat(col[1]) };
//            data.push_back(d);
            data.push_back(new Data);
            data.back()->timestamp = col[0];
            data.back()->voltage = ofToFloat(col[1]);
        }
        if (data.empty()) {
            LOG_WARNING << "dataset is empty: " << path;
        } else {
            LOG_NOTICE << "load dataset: " << path;
        }
        bufferLength = 0;
        width = 200.0;
        height = 50.0;
        speed = 1;
        bPlay = false;
        offset = 0;
        tmpOffset = offset;
        gain = 1.0;
    }
    
    void update()
    {
        if (bPlay) {
            tmpOffset += speed;
            offset = (int)tmpOffset;
            if (offset < 0) offset = data.size() - 1;
            if (offset > data.size() - 1) offset = 0;
        }
    }
    
    void draw(){ draw(20.0, 20.0); }
    void draw(float posx, float posy) { draw(posx, posy, width, height, gain); }
    void draw(float posx, float posy, float w, float h) { draw(posx, posy, w, h, gain); }
    void draw(float posx, float posy, float w, float h, float gain)
    {
        int datasize = data.size() - 1;
        int start = offset;
        int end = offset + bufferLength;
        ofBeginShape();
        if (ofGetStyle().bFill) ofVertex(posx, posy + (h * 0.5));
        for(int i = start; i < end; i++) {
            int index = i < 0 ? datasize - i : i;
            index = index > datasize ? index - datasize : index;
            ofVertex(posx + ofMap(i, start, end, 0, w, true), (posy + (h * 0.5)) + (data[index]->voltage * gain));;
        }
        if (ofGetStyle().bFill) ofVertex(posx + w, posy + (h * 0.5));
        ofEndShape();
    }
    
    void setSize(float _width, float _height)
    {
        width = _width;
        height = _height;
    }
    void setSpeed(float sp){ speed = sp; }
    void setBufferLemgth(int b){ bufferLength = b; }
    void play(){ bPlay = true; }
    void stop(){ bPlay = false; }
    void togglePlay(){ bPlay = !bPlay; }
    bool isPlay(){ return bPlay; }
    void setOffset(int v){ offset = v; }
    void setGain(float v){ gain = v; }
    float getPosition() { return offset == 0 ? offset : (float)offset / data.size(); }
    Data * getDataOnOffset(){ return data[offset]; }
    float getVoltageOnOffset(){ return data[offset]->voltage * gain; }
    Data * getDataOnStartPoint(){ return data[offset + bufferLength - 1]; }
    float getVoltageOnStartPoint(){ return data[offset + bufferLength - 1]->voltage * gain; }
    float getVoltage()
    {
        int start = offset;
        int end = offset + bufferLength;
        int pos = ((end - start) * 0.5) + start;
        return data[pos]->voltage * gain;
    }
    
    void getDataBetween(vector<Data *> * dst, int start, int end)
    {
        if (start < 0 || start >= data.size()) return;
        if (end   < 0 || end   >= data.size()) return;
        if (start >= end) return;
        
        int size = end - start;
        dst->clear();
        for (int i = start; i < end; i++) {
            dst->push_back(data[i]);
        }
    }
    
};
