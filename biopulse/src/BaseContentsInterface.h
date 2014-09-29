#pragma once

#include "common.h"

namespace rs
{
    static ofVideoPlayer radar;
    static vector<ofTexture> inks;
    
}


class BaseContentsController;

class BaseContentsInterface
{
    friend class BaseContentsController;
    
    ofFbo mFbo;
    ofColor mBGColor;
    ofColor mFGColor;
    ofBlendMode mBlendMode;
    
    inline void rendering()
    {
        if (mFbo.isAllocated()) {
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            ofPushMatrix();
            ofPushStyle();
            mFbo.begin();
            ofBackground(mBGColor);
            ofSetColor(255,255,255,255);
            this->draw();
            mFbo.end();
            ofPopStyle();
            ofPopMatrix();
            glPopAttrib();
        }
    }
    
protected:
    
    bool bPlay;
    
    float getWidth()  { return mFbo.getWidth();  }
    float getHeight() { return mFbo.getHeight(); }
#define WIDTH getWidth()
#define HEIGHT getHeight()
    
public:
    
    BaseContentsInterface():
    bPlay(false),
    mBGColor(ofColor(0,0,0,0)),
    mFGColor(ofColor(255,255,255,255)),
    mBlendMode(OF_BLENDMODE_ALPHA)
    {}
    virtual ~BaseContentsInterface() {}
    
    virtual void update() {}
    virtual void draw() {}
    virtual void gotMessage(int msg) {}
    
    inline void setupFbo(ofFbo::Settings settings) { mFbo.allocate(settings); }
    inline void setBackgroundColor(ofColor col) { mBGColor.set(col); }
    inline void setForegroundColor(ofColor col) { mFGColor.set(col); }
    inline void setBlendMode(ofBlendMode mode) { mBlendMode = mode; }
    
    inline void draw(int x, int y)
    {
        if (mFbo.isAllocated()) {
            rendering();
            ofEnableBlendMode(mBlendMode);
            ofSetColor(mFGColor);
            mFbo.draw(x, y);
        } else draw();
    }
    inline void draw(int x, int y, int w, int h)
    {
        if (mFbo.isAllocated()) {
            rendering();
            ofEnableBlendMode(mBlendMode);
            ofSetColor(mFGColor);
            mFbo.draw(x, y, w, h);
        } else draw();
    }
    
    virtual void startPlaying() {}
    virtual void endPlaying() {}
    
    inline void play() { bPlay = true; startPlaying(); }
    inline void stop() { bPlay = false; endPlaying(); }
    inline bool isPlay() { return bPlay; }
    inline BaseContentsInterface * getThis() { return this; }
};



class BaseContentsController
{
    bool isOver(int n)
    {
        if (n < 0 || n >= instances.size()) return true;
        return false;
    }
    
protected:
    
    vector<BaseContentsInterface *> instances;
    
    template <typename T>
    T* setupInstance(T* o)
    {
        instances.push_back(o);
        return o;
    }
    
public:
    
    void update(float tick = ofGetLastFrameTime())
    {
        for (int i = 0; i < instances.size(); i++)
        {
            BaseContentsInterface *o = instances[i];
            if (o->isPlay()) o->update();
        }
    }
    
    void draw(int x, int y, int w, int h)
    {
//        glPushAttrib(GL_ALL_ATTRIB_BITS);
//        ofPushMatrix();
//        ofPushStyle();
        drawInstance(x, y, w, h);
//        ofPopStyle();
//        ofPopMatrix();
//        glPopAttrib();
    }
    
    void drawInstance(int x, int y, int w, int h)
    {
        vector<BaseContentsInterface *>::iterator it = instances.begin();
        while (it != instances.end())
        {
            BaseContentsInterface *o = *it;
            if (o->isPlay()) o->draw(x, y, w, h);
            it++;
        }
    }
    
    void play(int n)
    {
        if (isOver(n)) return;
        instances[n]->play();
    }
    
    void stop(int n)
    {
        if (isOver(n)) return;
        instances[n]->stop();
    }
    
    void togglePlay(int n)
    {
        if (isOver(n)) return;
        instances[n]->isPlay() ? instances[n]->stop() : instances[n]->play();
    }
    
    bool isPlay(int n)
    {
        if (isOver(n)) return false;
        return instances[n]->isPlay();
    }
    
    void playAll()
    {
        vector<BaseContentsInterface *>::iterator it = instances.begin();
        while (it != instances.end())
        {
            BaseContentsInterface *o = *it;
            o->play();
            it++;
        }
    }
    
    void stopAll()
    {
        vector<BaseContentsInterface *>::iterator it = instances.begin();
        while (it != instances.end())
        {
            BaseContentsInterface *o = *it;
            o->stop();
            it++;
        }
    }
    
    void sendMessageAll(int msg)
    {
        vector<BaseContentsInterface *>::iterator it = instances.begin();
        while (it != instances.end())
        {
            BaseContentsInterface *o = *it;
            o->gotMessage(msg);
            it++;
        }
    }
    
    void sendMessage(int n, int msg)
    {
        if (isOver(n)) return;
        instances[n]->gotMessage(msg);
    }
    
    void setFboAllocate(ofFbo::Settings settings)
    {
        vector<BaseContentsInterface *>::iterator it = instances.begin();
        while (it != instances.end())
        {
            BaseContentsInterface *o = *it;
            o->setupFbo(settings);
            it++;
        }
    }
    
    void setBackgroundColor(ofColor col)
    {
        vector<BaseContentsInterface *>::iterator it = instances.begin();
        while (it != instances.end())
        {
            BaseContentsInterface *o = *it;
            o->setBackgroundColor(col);
            it++;
        }
    }
    
    void setForegroundColor(ofColor col)
    {
        vector<BaseContentsInterface *>::iterator it = instances.begin();
        while (it != instances.end())
        {
            BaseContentsInterface *o = *it;
            o->setForegroundColor(col);
            it++;
        }
    }
    
    void setBlendMode(ofBlendMode mode)
    {
        vector<BaseContentsInterface *>::iterator it = instances.begin();
        while (it != instances.end())
        {
            BaseContentsInterface *o = *it;
            o->setBlendMode(mode);
            it++;
        }
    }
    
    int getInstancesSize() { return instances.size(); }

    
public:
    
    template <typename T>
    T* createInstance()
    {
        return setupInstance(new T);
    }
    
    /*
     ref ofxAnimationPrimitives
    
     http://nedbatchelder.com/code/cog/
     $ cog.py -r SomoSourceCode.h
     
     [[[cog
     import cog
     
     tmpl = '''template <typename T, %(A)s>
     T* createInstance(%(B)s)
     {
     return setupInstance<T>(new T(%(C)s));
     }
     '''
     
     cog.outl('')
     for i in xrange(1, 18):
     a = ', '.join(['typename A%i' % x for x in range(i)])
     b = ', '.join(['const A%i& a%i' % (x, x) for x in range(i)])
     c = ', '.join(['a%i' % x for x in range(i)])
     cog.outl(tmpl % {'A':a, 'B':b, 'C':c})
     
     ]]]*/
    
    template <typename T, typename A0>
    T* createInstance(const A0& a0)
    {
        return setupInstance<T>(new T(a0));
    }
    
    template <typename T, typename A0, typename A1>
    T* createInstance(const A0& a0, const A1& a1)
    {
        return setupInstance<T>(new T(a0, a1));
    }
    
    template <typename T, typename A0, typename A1, typename A2>
    T* createInstance(const A0& a0, const A1& a1, const A2& a2)
    {
        return setupInstance<T>(new T(a0, a1, a2));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3>
    T* createInstance(const A0& a0, const A1& a1, const A2& a2, const A3& a3)
    {
        return setupInstance<T>(new T(a0, a1, a2, a3));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4>
    T* createInstance(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4)
    {
        return setupInstance<T>(new T(a0, a1, a2, a3, a4));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
    T* createInstance(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5)
    {
        return setupInstance<T>(new T(a0, a1, a2, a3, a4, a5));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
    T* createInstance(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6)
    {
        return setupInstance<T>(new T(a0, a1, a2, a3, a4, a5, a6));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
    T* createInstance(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7)
    {
        return setupInstance<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
    T* createInstance(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8)
    {
        return setupInstance<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7, a8));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
    T* createInstance(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9)
    {
        return setupInstance<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
    T* createInstance(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10)
    {
        return setupInstance<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
    T* createInstance(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11)
    {
        return setupInstance<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
    T* createInstance(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11, const A12& a12)
    {
        return setupInstance<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
    T* createInstance(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11, const A12& a12, const A13& a13)
    {
        return setupInstance<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
    T* createInstance(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11, const A12& a12, const A13& a13, const A14& a14)
    {
        return setupInstance<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15>
    T* createInstance(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11, const A12& a12, const A13& a13, const A14& a14, const A15& a15)
    {
        return setupInstance<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15));
    }
    
    template <typename T, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14, typename A15, typename A16>
    T* createInstance(const A0& a0, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10, const A11& a11, const A12& a12, const A13& a13, const A14& a14, const A15& a15, const A16& a16)
    {
        return setupInstance<T>(new T(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16));
    }
    
    //[[[end]]]
};
