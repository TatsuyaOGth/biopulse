#pragma once

#include "../../common.h"

class Scene01 : public BaseSceneInterfase
{
    
    
public:
    
    void setup()
    {
        FOR_DATASETS {
            it->get()->setBufferLemgth(255);
            it->get()->play();
            it->get()->setGain(200);
        }
    }
    
    void update()
    {
        FOR_DATASETS {
            it->get()->setSpeed(debugMouseX(100));
            it->get()->setGain(debugMouseY(1000));
        }
    }
    
    void draw()
    {
        ofSetColor(255, 0, 0);
        ofFill();
        share::datasets[0]->draw(20, 20, plant::edgeW - 20, plant::edgeH - 20);
        
        ofSetColor(0, 0, 255);
        ofNoFill();
        share::datasets[1]->draw(plant::bodyX + 20, plant::bodyY + 20, plant::bodyW - 20, plant::bodyH - 20);
        
        cout << share::datasets[0]->getPosition() << endl;
    }
    
    
};
