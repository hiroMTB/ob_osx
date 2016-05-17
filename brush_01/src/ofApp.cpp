#include "ofApp.h"
#include "obGeoDrawer.hpp"

void ofApp::setup(){
    
    ofSetFrameRate(targetFps);
    ofSetWindowShape(1920, 1080);
    ofSetWindowPosition(0, 0);
    
    bStart = true;

    // sound
    soundStream.printDeviceList();
    bufferSize = 256;
    nCh = 2;
    sampleRate = 8000;
    currentSamplePos = 0;
    prevSamplePos = 0;
    soundStream.setup(this, 0, nCh, sampleRate, bufferSize, 4);
    audioData.assign( nCh, vector<float>() );

    
    // visual
    int w = ofGetWindowWidth();
    int h = ofGetWindowHeight();
    bHandy = w<h;

    float longside = bHandy ? h:w;
    float shortside = bHandy ? w:h;
    canvas.set(0, 0, longside, shortside);

    track_len = longside * 0.9f;
    track_offset = longside * 0.05;
    start_point.x = track_offset;
    start_point.y = shortside/2;
    indicator.set(0, 0);

    // set update speed
    float speed_ms = track_len / (float)total_time_ms;
    indicator_speed = speed_ms * 1000.0f/(float)targetFps;
    
    
    // vbo
    vPoints.setUsage(GL_DYNAMIC_DRAW);
    vLines.setUsage(GL_DYNAMIC_DRAW);
    vPoints.setMode(OF_PRIMITIVE_POINTS);
    vLines.setMode(OF_PRIMITIVE_LINES);
    
    vAudio.setUsage(GL_DYNAMIC_DRAW);
    vAudio.setMode(OF_PRIMITIVE_POINTS);
     
    
}

void ofApp::update(){

    if( !bStart ) return;
    
    int totalSampleNum = total_time_ms/1000 * sampleRate;
    indicator.x = (float)currentSamplePos/totalSampleNum * track_len;
    

    // draw dot wave
    int start = prevSamplePos;
    int end = currentSamplePos;
    ob::draw_dot_wave(this, start, end-start, canvas.height/2);

    if(0){

        int start = vAudio.getNumVertices()/nCh;
        int end = audioData[0].size();

        for( int c=0; c<nCh; c++){
            float audioSamplePrev = end==0 ? 0 : audioData[c][end-1];
            // draw just the simple wave
            for(int i=start; i<end; i++){
                float val = audioData[c][i]*0.01 + audioSamplePrev*0.99;
                float amp = 1.00;
                int totalSampleNum = total_time_ms/1000 * sampleRate;
                float x = i/(float)totalSampleNum * track_len;
                float y = val * canvas.height/2 * amp;
                
                y += c==0 ? 50 : -50;
                vAudio.addVertex( ofVec3f(x, y, 0) );
                vAudio.addColor( ofFloatColor(0.1f, 0.1f, 1.0f, 0.8f) );
                
                audioSamplePrev = val;
            }
        }
    }
    
    if (indicator.x >= track_len) {
        cout << "tracking finished : " << ofGetElapsedTimef() << endl;
        bStart = false;
    }
    
    prevSamplePos = currentSamplePos;
}


void ofApp::draw(){

    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetLineWidth(1);

    ofBackground(255);
    ofPushMatrix();
    
    if(bHandy){
        ofRotateZ(90.0);
        ofTranslate(0, -ofGetWindowWidth());
    }

    ofTranslate(start_point);

    vPoints.draw();
    vLines.draw();
    vAudio.draw();
    
    ofSetColor(0, 0, 0);
    ofFill();
    ofDrawLine(          0,            -5,           0,            +5);
    ofDrawLine(indicator.x, indicator.y-5, indicator.x, indicator.y+5);

    
    ofPopMatrix();
    
    int y = 10;
    int os = 15;
    ofSetColor(0);
    ofDrawBitmapString("fps  " + ofToString(ofGetFrameRate()), 10, y);
    ofDrawBitmapString("time " + ofToString(ofGetElapsedTimef()), 10, y+=os);
    ofDrawBitmapString("vPoints numVertices " + ofToString(vPoints.getNumVertices()), 10, y+=os);
    ofDrawBitmapString("vLines  numVertices " + ofToString(vLines.getNumVertices()), 10, y+=os);
    ofDrawBitmapString("vAudio  numVertices " + ofToString(vAudio.getNumVertices()), 10, y+=os);

}


void ofApp::audioIn(float * input, int bufferSize, int nChannels){

    if( !bStart ) return;
    
    for(int i=0; i<bufferSize; i++){
        for( int c=0; c<nChannels; c++){
            float val = input[i*nChannels + c];
            audioData[c].push_back( val );
        }        
        currentSamplePos++;
    }
}


void ofApp::keyPressed(int key){

}


void ofApp::keyReleased(int key){

}


void ofApp::mouseMoved(int x, int y){

}


void ofApp::mouseDragged(int x, int y, int button){

}


void ofApp::mousePressed(int x, int y, int button){

}


void ofApp::mouseReleased(int x, int y, int button){

}


void ofApp::mouseEntered(int x, int y){

}


void ofApp::mouseExited(int x, int y){

}


void ofApp::windowResized(int w, int h){

}


void ofApp::gotMessage(ofMessage msg){

}


void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::exit(ofEventArgs & args){
    ofLogVerbose("", "exit called");
}