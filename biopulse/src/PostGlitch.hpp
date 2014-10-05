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
    

};