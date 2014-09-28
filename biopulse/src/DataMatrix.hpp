#pragma once

#include "BaseContentsInterface.h"

class DataMatrix : public BaseContentsInterface
{
    int mode;
    deque<string> list1;
    deque<string> list2;
    deque<string> list3;
    deque<string> list4;
    deque<string> list5;
    deque<string> list6;
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
            string s4 = "";
            string s5 = "";
            string s6 = "";
            
            s1 += d1->timestamp;
            s2 += ofToString(d1->voltage, 8);
            s3 += d2->timestamp;
            s4 += ofToString(d2->voltage, 8);
            s5 += d3->timestamp;
            s6 += ofToString(d3->voltage, 8);
            
            list1.push_back(s1);
            list2.push_back(s2);
            list3.push_back(s3);
            list4.push_back(s4);
            list5.push_back(s5);
            list6.push_back(s6);
            
            if (list1.size() > 36) list1.pop_front();
            if (list2.size() > 36) list2.pop_front();
            if (list3.size() > 36) list3.pop_front();
            if (list4.size() > 36) list4.pop_front();
            if (list5.size() > 36) list5.pop_front();
            if (list6.size() > 36) list6.pop_front();
        }
    }
    
    void draw()
    {
        ofSetColor(190);
        
        if (mode == 0) {
            int x = 0;
            int y = 10;
            int b1 = 165;
            int b2 = 100;
            for (int i = 0; i < list1.size(); i++) share::font.drawString(list1[i], x, y + (i * 16));
            for (int i = 0; i < list2.size(); i++) share::font.drawString(list2[i], x + b1, y + (i * 16));
            for (int i = 0; i < list3.size(); i++) share::font.drawString(list3[i], x + b1 + b2, y + (i * 16));
            for (int i = 0; i < list4.size(); i++) share::font.drawString(list4[i], x + (b1 * 2) + b2, y + (i * 16));
            for (int i = 0; i < list5.size(); i++) share::font.drawString(list5[i], x + (b1 * 2) + (b2 * 2), y + (i * 16));
            for (int i = 0; i < list6.size(); i++) share::font.drawString(list6[i], x + (b1 * 2) + (b2 * 2) + b1, y + (i * 16));
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