#pragma once

#include "BaseContentsInterface.h"

class DataMatrix : public BaseContentsInterface
{
    int mode;
    deque<string> list1;
    deque<string> list2;
    deque<string> list3;
    deque< vector<string> > matrix;
    
public:
    DataMatrix()
    {
        mode = 0;
    }
    
    void update()
    {
        if (mode == 0) {
            Data * d1 = DATASET[0]->getNextData();
            Data * d2 = DATASET[1]->getNextData();
            Data * d3 = DATASET[2]->getNextData();
            string s1 = "";
            string s2 = "";
            string s3 = "";
            
            s1 += "DATA:" + d1->timestamp + " VOLTAGE:" + ofToString(d1->voltage, 8);
            s2 += "DATA:" + d2->timestamp + " VOLTAGE:" + ofToString(d2->voltage, 8);
            s3 += "DATA:" + d3->timestamp + " VOLTAGE:" + ofToString(d3->voltage, 8);
            
            list1.push_back(s1);
            list2.push_back(s2);
            list3.push_back(s3);
            
            if (list1.size() > 36) list1.pop_front();
            if (list2.size() > 36) list2.pop_front();
            if (list3.size() > 36) list3.pop_front();
        }
        
        if (mode == 1) {
            vector<string> v;
            for (int i = 0; i < 40; i++) v.push_back(ofToString((int)ofRandom(10)));
            matrix.push_back(v);
            if (matrix.size() > 36) matrix.pop_front();
        }
    }
    
    void draw()
    {
        ofSetColor(190);
        
        if (mode == 0) {
            int x = 0;
            int y = 10;
            int b = 385;
            for (int i = 0; i < list1.size(); i++) share::font.drawString(list1[i], x, y + (i * 16));
            for (int i = 0; i < list2.size(); i++) share::font.drawString(list2[i], x + b, y + (i * 16));
            for (int i = 0; i < list3.size(); i++) share::font.drawString(list3[i], x + (b * 2), y + (i * 16));
        }
        
        if (mode == 1) {
            for (int i = 0; i < matrix.size(); i++) {
                for (int j = 0; j < matrix[i].size(); j++) {
                    int x = j * 32;
                    int y = 10 + (i * 16);
                    share::font.drawString(matrix[i][j], x, y);
                }
            }
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
    }
    
};