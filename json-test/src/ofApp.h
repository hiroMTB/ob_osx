#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include "ofURLFileLoader.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void draw();
    void draw_example();
    void draw_json( ofxJSONElement & elem);
    
    void keyPressed( int key );
    void loadFile();
    void requestBear();
    
    ofxJSONElement result;    
    string bear;
};
