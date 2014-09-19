#pragma once

#include "ofMain.h"
#include "ofxTimeline.h"

//==============================================================
// utils
//==============================================================
// マウスポインタ座標を画面サイズで0-1正規化した値を返す
#define debugMouseX(a) (((float)ofGetMouseX()/(float)ofGetWidth())*a)
#define debugMouseY(a) (((float)ofGetMouseY()/(float)ofGetHeight())*a)

// aよりbが大きければb-aを返す（変数更新時のリミッター用）
#define	MIN_NORM(a,b) (((a)<(b))?(a):(b-a))

// 画面中心を取得
#define CENTER_X (ofGetWidth()/2)
#define CENTER_Y (ofGetHeight()/2)


// 処理速度の計測
static clock_t start = 0;
#define START_CLOCK (start = clock())
#define GET_CLOCK (clock()-start)
#define PRINT_CLOCK(comment) (std::cout<<comment<<clock()-start<<std::endl)


// ofLogger utils
#define printLOG( msg ) printf("%s,%d:%s\n",__FILE__,__LINE__,msg)
#define LOG_INFO ofToString(ofGetElapsedTimeMillis(),8)
#define LOG_NOTICE ofLogNotice(LOG_INFO)
#define LOG_ERROR ofLogError(LOG_INFO)
#define LOG_WARNING ofLogWarning(LOG_INFO)

// debug
#define ASSERT(a) (assert(a))

#define MY_CHECK(e) assert(e)

inline string getHome()
{
    char *home = getenv("HOME");
    return string(home);
}

//==============================================================
// constants
//==============================================================
namespace plant
{
    static const int width = 800;
    static const int height = 800;
    
    static const int edgeX = 0;
    static const int edgeY = 0;
    static const int edgeW = width;
    static const int edgeH = height * 0.25;
    static const int bodyX = 0;
    static const int bodyY = edgeH;
    static const int bodyW = width;
    static const int bodyH = height * 0.75;
    static const ofRectangle edgeRect(edgeX, edgeY, edgeW, edgeH);
    static const ofRectangle bodyRect(bodyX, bodyY, bodyW, bodyH);
    
    static const ofVec2f bodyP1(bodyX, bodyY);
    static const ofVec2f bodyP2(bodyW, bodyY);
    static const ofVec2f bodyP3(bodyW * 0.8, edgeH + bodyH);
    static const ofVec2f bodyP4(bodyW * 0.2, edgeH + bodyH);
    
    
    inline void drawPlantGuide()
    {
        ofPushStyle();
        ofFill();
        ofSetColor(255,0,0, 40);
        ofRect(plant::edgeRect);
        ofSetColor(0,255, 0, 40);
#ifndef TARGET_OPENGLES
        glBegin(GL_QUADS);
        glVertex2f(bodyP1.x, bodyP1.y);
        glVertex2f(bodyP2.x, bodyP2.y);
        glVertex2f(bodyP3.x, bodyP3.y);
        glVertex2f(bodyP4.x, bodyP4.y);
        glEnd();
#endif
        ofPopStyle();
    }
}

namespace common
{
    
    // biopulse data
    static const int numDatasetPath = 2;
    static const char* ht[] = {
        "ycam-3-20130501.csv",
        "symbiotica-26-20140916.csv"
    };
    
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      data controller
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *  data controller
 */
class DataController
{
public:
    float width, height, speed, gain;
    const string mDataPath;
    int bufferLength;
    bool bPlay;
    int offset;
    typedef struct _Data {
        string timestamp;
        float voltage;
    } Data;
    vector<Data> data;
    
    
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
            Data d = {col[0], ofToFloat(col[1]) };
            data.push_back(d);
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
        gain = 1.0;
    }
    
    void update()
    {
        if (bPlay) {
            offset += speed;
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
//        if (ofGetStyle().bFill) {
//            glBegin(GL_POLYGON);
//            glVertex2d(posx + w, posy + (h * 0.5));
//            glVertex2d(posx, posy + (h * 0.5));
//            
//        } else {
//            glBegin(GL_LINE_STRIP);
//        }
        
        ofBeginShape();
        if (ofGetStyle().bFill) ofVertex(posx, posy + (h * 0.5));
        for(int i = start; i < end; i++) {
            int index = i < 0 ? datasize - i : i;
            index = index > datasize ? index - datasize : index;
//            glVertex2d(posx + ofMap(i, start, end, 0, w, true), (posy + (h * 0.5)) + (data[index].voltage * gain));
            ofVertex(posx + ofMap(i, start, end, 0, w, true), (posy + (h * 0.5)) + (data[index].voltage * gain));;
        }
        if (ofGetStyle().bFill) ofVertex(posx + w, posy + (h * 0.5));
        ofEndShape();
//        glEnd();
    }
    
    void setSize(float _width, float _height)
    {
        width = _width;
        height = _height;
    }
    void setSpeed(int sp){ speed = sp; }
    void setBufferLemgth(int b){ bufferLength = b; }
    void play(){ bPlay = true; }
    void stop(){ bPlay = false; }
    void togglePlay(){ bPlay = !bPlay; }
    bool isPlay(){ return bPlay; }
    void setOffset(int v){ offset = v; }
    void setGain(float v){ gain = v; }
    float getPosition() { return offset == 0 ? offset : (float)offset / data.size(); }
    
};




////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      Scene Class Interfase
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Scene Class Interfase
 */
class BaseSceneInterfase
{
    ofxTimeline timeline;
    
    
public:
    BaseSceneInterfase(){};
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
    void setEnableTimeline(bool enable){ enable ? timeline.enable() : timeline.disable(); }
    void drawTimeline(){ timeline.draw(); }
};





////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      share values
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace share
{
    static float elapsedTime = 0;
    
    typedef shared_ptr<DataController> datasetPtr;
    static vector<datasetPtr> datasets;
}

#define DATASETS share::datasets
#define FOR_DATASETS for (vector<share::datasetPtr>::iterator it = share::datasets.begin(); it != share::datasets.end(); it++)


