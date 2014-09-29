#pragma once

#include "BaseContentsInterface.h"

class Radar : public BaseContentsInterface
{
    
public:
    
    Radar()
    {
        if (!rs::radar.isLoaded()) ASSERT(rs::radar.loadMovie("movies/radar.mov"));
    }
    
    void update()
    {
        if (rs::radar.isPlaying()) rs::radar.update();
        
    }
    
    void draw()
    {
        ofSetRectMode(OF_RECTMODE_CENTER);
        
        // mask
//        ofSetColor(0, 0, 0, 255);
//        ofSetCircleResolution(40);
//        ofCircle(getWidth() * 0.5, getHeight() * 0.5, 200);
        
        ofSetColor(255, 255, 255, 255);
        if (rs::radar.isPlaying()) {
            float w = rs::radar.getWidth();
            float h = rs::radar.getHeight();
            float p = 0.9;
            rs::radar.draw(getWidth()*0.5, getHeight()*0.5, w * p, h * p);
        }
    }
    
    void startPlaying() { rs::radar.play(); }
    void endPlaying() { rs::radar.stop(); }

};