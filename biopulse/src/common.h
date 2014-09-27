#pragma once

#include "ofMain.h"
#include "DataController.hpp"

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
    static const int edgeXW = edgeX + edgeW;
    static const int edgeYH = edgeY + edgeH;
    static const int bodyX = 0;
    static const int bodyY = edgeH;
    static const int bodyW = width;
    static const int bodyH = height * 0.75;
    static const int bodyXW = bodyX + bodyW;
    static const int bodyYH = bodyY + bodyH;
    static const ofRectangle edgeRect(edgeX, edgeY, edgeW, edgeH);
    static const ofRectangle bodyRect(bodyX, bodyY, bodyW, bodyH);
    
    static const ofVec2f bodyP1(bodyX, bodyY);
    static const ofVec2f bodyP2(bodyW, bodyY);
    static const ofVec2f bodyP3(bodyW * 0.8, edgeH + bodyH);
    static const ofVec2f bodyP4(bodyW * 0.2, edgeH + bodyH);
    
    inline float getEdgeCenterX(){ return ((float)edgeX + ((float)edgeW * 0.5)); }
    inline float getEdgeCenterY(){ return ((float)edgeY + ((float)edgeH * 0.5)); }
    
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
    
    inline void drawPlantMask()
    {
        ofSetColor(0, 0, 0, 255);
#ifndef TARGET_OPENGLES
        glBegin(GL_TRIANGLE_STRIP);
        glVertex2f(bodyP1.x, bodyP1.y);
        glVertex2f(bodyP4.x, bodyP4.y);
        glVertex2f(bodyX, bodyYH);
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
        glVertex2f(bodyP2.x, bodyP2.y);
        glVertex2f(bodyP3.x, bodyP3.y);
        glVertex2f(bodyXW, bodyYH);
        glEnd();
#endif
        ofPopStyle();
    }
}

namespace share
{
    static float elapsedTimef = 0;
    static long long elapsedTimeMillis = 0;
    static long long elapsedTimeMicros = 0;
    static ofTrueTypeFont font;
}
