#pragma once

#include "BaseContentsInterface.h"
#include "ofxPostGlitch.h"
#include "ofxAnimationPrimitives.h"

class PostGlitch : public BaseContentsInterface, public ofxPostGlitch
{
private:
    typedef struct _ofxPostGlitchStack
    {
        ofxPostGlitchType type;
        float duration;
        _ofxPostGlitchStack(ofxPostGlitchType t, float d): type(t), duration(d){}
    } ofxPostGlitchStack;
    vector<shared_ptr<ofxPostGlitchStack> > mPostGlitchStacks;
    
    ofTexture & mTex;
    
public:
    PostGlitch(ofFbo * tex): mTex(tex->getTextureReference())
    {
        setup(tex);
        for (int i = 0; i < GLITCH_NUM; i++) bShading[i] = false;
    }
    
    void update()
    {
        vector<shared_ptr<ofxPostGlitchStack> >::iterator it = mPostGlitchStacks.begin();
        while (it != mPostGlitchStacks.end()) {
            if ((*it)->duration < 0) {
                bShading[(*it)->type] = false;
                it = mPostGlitchStacks.erase(it);
            } else {
                bShading[(*it)->type] = true;
                (*it)->duration -= ofGetLastFrameTime();
                it++;
            }
        }
    }
    
    void draw()
    {
        generateFx();
    }
    
    void setFxAsTime(ofxPostGlitchType type, float duration)
    {
        mPostGlitchStacks.push_back(shared_ptr<ofxPostGlitchStack>(new ofxPostGlitchStack(type, duration)));
    }
    
    
    // overrideed
    void generateFx()
    {
        if (targetBuffer == NULL){
            ofLog(OF_LOG_WARNING, "ofxFboFX --- Fbo is not allocated.");
            return;
        }
        
        static int step = ofRandom(4,15);
        float v[2];
        v[0] = ofRandom(3);v[1] = ofRandom(3);
        if (ofGetFrameNum() % step == 0){
            step = ofRandom(10,30);
            ShadeVal[0] = ofRandom(100);
            ShadeVal[2] = ofRandom(100);
            ShadeVal[3] = ofRandom(100);
        }
        
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofSetColor(255);
        glClearColor(0, 0, 0, 0.0);
        
        for (int i = 0;i < GLITCH_NUM;i++){
            if (bShading[i]){
                shader[i].begin();
                shader[i].setUniformTexture	("image"		,*targetBuffer,0);
                shader[i].setUniform1i		("trueWidth"	,buffer_size.x);
                shader[i].setUniform1i		("trueHeight"	,buffer_size.y);
                shader[i].setUniform1f		("rand"			,ofRandom(1));
                shader[i].setUniform1f		("mouseX"		,ofGetMouseX());
                shader[i].setUniform1f		("mouseY"		,ofGetMouseY());
                shader[i].setUniform1f		("val1"			,ShadeVal[0]);
                shader[i].setUniform1f		("val2"			,ShadeVal[1]);
                shader[i].setUniform1f		("val3"			,ShadeVal[2]);
                shader[i].setUniform1f		("val4"			,ShadeVal[3]);
                shader[i].setUniform1f		("timer"		,ofGetElapsedTimef());
                shader[i].setUniform2fv		("blur_vec"		,v);
                
                ShadingBuffer.begin();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                ofRect(0, 0, buffer_size.x, buffer_size.y);
                ShadingBuffer.end();
                shader[i].end();
                
                targetBuffer->begin();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                ShadingBuffer.draw(0, 0,buffer_size.x,buffer_size.y);
                targetBuffer->end();
            }
        }
    }

};