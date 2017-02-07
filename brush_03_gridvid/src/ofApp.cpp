#include "ofApp.h"
#include "obGeoDrawer.hpp"
#include "obStats.hpp"

void ofApp::setup(){
    
    ofSetFrameRate(target_fps);
    ofSetVerticalSync(true);
    ofSetWindowShape(1080*0.65, 1920*0.65);
    ofSetWindowPosition(0, 0);
    ofEnableAlphaBlending();
    ofDisableAntiAliasing();
    ofDisableSmoothing();

    bStart = true;
    bLog = true;

    // visual
    int w = ofGetWindowWidth();
    int h = ofGetWindowHeight();
    bHandy = w<h;

    float longside = bHandy ? h:w;
    float shortside = bHandy ? w:h;

    // video
    grabber.setup(420, 320);
    grabber.setDesiredFrameRate(60);
    grabber.setUseTexture(true);
    grabber.setPixelFormat( OF_PIXELS_RGB );
    grabber.setDesiredFrameRate( target_fps );
    
    // grid
    float aspect = 420/320;
    
    nCol = 4;
    vW = ofGetWindowWidth() / nCol;
    vH = vW / aspect;
    nRow = (float) ofGetWindowHeight() / vH;
    
    start_pos.x = 1;
    start_pos.y = fmodf(ofGetWindowHeight(), vH) * 0.5f;
    
#ifdef RENDER
    exp.setup(1920, 1080, 30);
    exp.setFrameRange(0, 3*60*30+1);
    exp.setFilePattern("out/ob_%05i.png");
    exp.setOverwriteSequence(true);
    exp.setAutoExit(true);
    exp.startExport();
#endif
    
}

void ofApp::videoPreProcess(){

    // make vbo from video
    grabber.update();
    
    if(grabber.isFrameNew()){

        ofPixels & pix = grabber.getPixels();

        if( imgs.size() < nRow*nCol ){
            imgs.push_back(ofImage(pix));
        }else{
            imgs[grab_cnt].setFromPixels(pix);
        }
        grab_cnt++;
        grab_cnt %= nRow*nCol;
    }
}

void ofApp::update(){

    if( !bStart ) return;
    videoPreProcess();
}

void ofApp::draw(){
    
#ifdef RENDER
    exp.begin();
#endif
    ofSetLineWidth(1);
    glPointSize(1);
    ofBackground( 15, 0, 0 );
    ofPushMatrix();{
        
        ofTranslate( start_pos );
        draw_vid();
        
    }ofPopMatrix();

   // draw_info();

#ifdef RENDER
    exp.end();
    exp.draw(0, 0);
#endif
        
}

void ofApp::draw_vid(){

    ofSetRectMode(OF_RECTMODE_CORNER);
    
    int max = nRow * nCol;
    
    for (int i=0; i<imgs.size(); i++) {
        
        ofImage & img = imgs[i];

        int x = i % nCol * vW;
        int y = i / nCol * vH;

        ofSetColor(255);
        img.draw( x, y, vW, vH );
    }
}

void ofApp::draw_info(){
    int y = 10;
    int x = 10;
    int os = 200;
    
    ofSetColor(0);
    ofFill();
    ofDrawRectangle(0, 0, ofGetWindowWidth(), 20);

    ofSetColor(255);
    ofDrawBitmapString("fps        " + ofToString(ofGetFrameRate()), x, y);
    ofDrawBitmapString("nCol       " + ofToString(nCol), x+=os, y);
    ofDrawBitmapString("nRow       " + ofToString(nRow), x+=os, y);

    
}

void ofApp::keyPressed(int key){
    switch (key) {
        case ' ': bStart = !bStart;     break;
        case 'l': bLog   = !bLog;       break;
    };
}

void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){ }
void ofApp::exit(ofEventArgs & args){}