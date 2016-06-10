#pragma once
#include "ofMain.h"

#ifndef TARGET_OPENGLES
//#define RENDER
#include "ofxExportImageSequence.h"
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
    void audioIn(float * input, int bufferSize, int nChannels) override;
    
    void audioPreProcess();
    void draw_wave();
    void draw_bg();
    void draw_info();
    void draw_audioStats();
    
    const int total_time_ms = 3 * 60 * 1000; // 3 mim
    const int targetFps = 60;
    
    bool bHandy;
    bool bStart;
    bool bLog;
    
    // visual
    ofRectangle canvas;
    float track_len;
    float track_offset;
    float indicator_speed;
    
    ofVec2f start_point; // absolute pix pos
    ofVec2f indicator;   // relative pix pos(dist from start_point)
        
    // sound
    int currentSamplePos;
    int prevSamplePos;
    vector<float> audioData;
    ofSoundStream soundStream;
    
    // video
    ofVideoGrabber grabber;

#ifdef RENDER
    // exp
    ofxExportImageSequence exp;
#endif
};
