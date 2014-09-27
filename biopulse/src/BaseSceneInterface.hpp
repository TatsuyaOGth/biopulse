#pragma once

#include "ofMain.h"
#include "common.h"
#include "DataController.hpp"
#include "BaseContentsInterface.h"
#include "ofxTimeline.h"



class BaseSceneInterfase
{
protected:
    typedef BaseSceneInterfase base;
    ofxTimeline timeline;
    
    typedef shared_ptr<DataController> datasetPtr;
    static vector<datasetPtr> smDataSet;
        
public:
    BaseSceneInterfase()
    {
    };
    virtual ~BaseSceneInterfase(){};
    
    virtual void setup(){}
    virtual void update(){}
    virtual void draw(){}
    virtual void exit(){}
    
    virtual void windowResized(int w, int h){}
    
    virtual void keyPressed( int key ){}
    virtual void keyReleased( int key ){}
    
    virtual void mouseMoved( int x, int y ){}
    virtual void mouseDragged( int x, int y, int button ){}
    virtual void mousePressed( int x, int y, int button ){}
    virtual void mouseReleased(int x, int y, int button ){}
    
    inline string getName()
    {
        const type_info& id = typeid(*this);
        int stat;
        char *name = abi::__cxa_demangle(id.name(), 0, 0, &stat);
        if (stat != 0) LOG_WARNING << "faild get name";
        free(name);
        string dst(name);
        return dst;
    }
    
    void setupTimeline()
    {
        timeline.setup();
        timeline.setName(getName());
        timeline.setLoopType(OF_LOOP_NONE);
        timeline.setFrameRate(60);
        timeline.disable();
    }
    void setEnableTimeline(bool enable)
    {
        if (enable) {
            timeline.enable();
        } else {
            timeline.disable();
        }
    }
    void drawTimeline(){ timeline.draw(); }
};





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// SHARE VALUES
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//namespace share
//{
//    static float elapsedTime = 0;
//    typedef shared_ptr<DataController> datasetPtr;
//    static vector<datasetPtr> datasets;
//}
//
//#define DATASETS share::datasets
//#define FOR_DATASETS for (vector<share::datasetPtr>::iterator it = share::datasets.begin(); it != share::datasets.end(); it++)
