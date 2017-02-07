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

    void videoPreProcess();
    void draw_vid();
    void draw_info();
    
    
    // app
    const int       total_time_ms = 3 * 60 * 1000; // 3 mim
    const int       target_fps = 60;
    
    bool            bHandy;
    bool            bStart;
    bool            bLog;
        
    // video
    ofVideoGrabber  grabber;
    vector<ofImage> imgs;
    int grab_cnt = 0;
    
    // grid
    int nRow;
    int nCol;
    float vW;
    float vH;
    
    ofPoint start_pos;
    
#ifdef RENDER
    // exp
    ofxExportImageSequence exp;
#endif
};
