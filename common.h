#pragma once

#include "ofMain.h"


//==============================================================
// utils
//==============================================================
// マウスポインタ座標を画面サイズで0-1正規化した値を返す
//#define debugMouseX ((float)ofGetMouseX()/(float)ofGetWidth())
//#define debugMouseY ((float)ofGetMouseY()/(float)ofGetHeight())
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
#define LOG_INFO ofToString(ofGetElapsedTimeMillis(),8)
#define LOG_NOTICE ofLogNotice(LOG_INFO)
#define LOG_ERROR ofLogError(LOG_INFO)
#define LOG_WARNING ofLogWarning(LOG_INFO)

#define MY_CHECK(e) assert(e)

inline string getHome(){
    char *home = getenv("HOME");
    return string(home);
}

//==============================================================
// common parameters
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
    static const string biodata_ycam_3 = "ycam-3-20130501.csv";
    
    
}

namespace share
{
    static float elapsedTime = 0;
}

//==============================================================
// value types
//==============================================================



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
    DataController(const string & path);
    virtual ~DataController();
    
    void setup(int _bufferLength = 256);
    
    void allocate(int num);
    void pushData(float _d);
    
    void draw();
    void draw(float posx, float posy);
    void draw(float posx, float posy, float w, float h, float min, float max);
    
    void setSize(float _width, float _height);
    void setRange(float _min, float _max);
    
    float min, max, width, height;
    const string mDataPath;
    
private:
    int bufferLength;
    deque<float> data;
    
    
};

DataController::DataController(const string & path):
mDataPath(path)
{
    
    bufferLength = 0;
    min = -1.0;
    max = 1.0;
    width = 200.0;
    height = 50.0;
}

DataController::~DataController()
{
}

void DataController::setup(int _bufferLength)
{
    bufferLength = _bufferLength;
    
    this->allocate(bufferLength);
    
}

void DataController::allocate(int num)
{
    data.resize(num);
}

void DataController::pushData(float _d)
{
    for (int i=bufferLength-1; i> 0; i--) {
        data[i] = data[i-1];
    }
    
    data[0] = _d;
}

void DataController::draw()
{
    draw(0.0, 0.0);
}

void DataController::draw(float posx, float posy)
{
    draw(posx, posy, width, height, min, max);
}


void DataController::draw(float posx, float posy, float w, float h, float min, float max)
{
    ofPushStyle();
	ofNoFill();
	ofSetColor(ofColor::white);
	ofRect(posx, posy, w, h);
	ofSetColor(120, 120, 120);
	ofLine(posx, posy+h/2.0, posx+w, posy+h/2.0);
    
	ofSetColor(ofColor::green);
	glBegin(GL_LINE_STRIP);
	for(int i=0; i<bufferLength; i++){
		glVertex3d(posx+(w/(float)bufferLength)*i, posy + h - ofMap(data[i], min, max, 0.0, h), 0.0);
	}
	glEnd();
    
    ofPopStyle();
}

void DataController::setSize(float _width, float _height)
{
    width = _width;
    height = _height;
}

void DataController::setRange(float _min, float _max)
{
    min = _min;
    max = _max;
}

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
};

