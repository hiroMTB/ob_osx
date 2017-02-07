#pragma once
#include "ofMain.h"
#include "ofxExportImageSequence.h"

#ifndef TARGET_OPENGLES
//#define RENDER
#endif

class ofApp : public ofBaseApp{

public:
    void setup() override;
    void update() override;
    void draw() override;
    void keyPressed(int key) override;
    void keyReleased(int key) override;
    void mouseMoved(int x, int y) override;
    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseEntered(int x, int y) override;
    void mouseExited(int x, int y) override;
    void windowResized(int w, int h) override;
    void dragEvent(ofDragInfo dragInfo) override;
    void gotMessage(ofMessage msg) override;
    void exit(ofEventArgs & args) override;
    void audioIn( ofSoundBuffer& buffer ) override;
    //void audioIn(float * input, int bufferSize, int nChannels) override;
    
    void draw_bg();
    void draw_info();
    
    const int total_time_ms = 3 * 60 * 1000; // 3 mim
    const int targetFps = 60;
    
    bool bHandy;
    bool bStart;
    
    // visual
    ofRectangle canvas;
    float track_len;
    float track_offset;
    float indicator_speed;
    
    ofVec2f start_point; // absolute pix pos
    ofVec2f indicator;   // relative pix pos(dist from start_point)
    
    ofVboMesh vPoints;
    ofVboMesh vLines;
    ofVboMesh vAudio;
    ofVboMesh vCam;
    
    // sound
    int currentSamplePos;
    int prevSamplePos;
    vector<float> audioData;
    ofSoundStream soundStream;
    float amp;
    
    // video
    //ofVideoGrabber grabber;

#ifdef RENDER
    // exp
    ofxExportImageSequence exp;
#endif
};
