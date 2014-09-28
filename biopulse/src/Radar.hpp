#pragma once

#include "BaseContentsInterface.h"

class Radar : public BaseContentsInterface
{
    
public:
    
    Radar()
    {
        if (!rs::radar.isLoaded()) ASSERT(!rs::radar.loadMovie("movies/Final_t.mp4"));
    }
    
    void update()
    {
        if (rs::radar.isPlaying()) rs::radar.update();
        
    }
    
    void draw()
    {
        if (rs::radar.isPlaying()) {
                
            rs::radar.draw(0, 0);
        }
    }
    
    void startPlaying() { rs::radar.play(); }
    void endPlaying() { rs::radar.stop(); }

};