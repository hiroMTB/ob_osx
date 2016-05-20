#include "ofApp.h"
#include "obGeoDrawer.hpp"
#include "obStats.hpp"

//
//  if n<0 -1
//  if n=0  0
//  if n>0  1
//
template<typename T>
inline int signval(T n){
    return (n > 0) - (n < 0);
}

void ofApp::setup(){
    
    ofSetFrameRate(targetFps);
    ofSetVerticalSync(true);
    ofSetWindowShape(1920, 1080);
    ofSetWindowPosition(0, 0);
    ofEnableAlphaBlending();
    ofDisableAntiAliasing();
    ofDisableSmoothing();

    bStart = true;
    bLog = true;

    // sound
    int bufferSize = 1920;
    int nCh = 1;
    int sampleRate = 48000;
    currentSamplePos = 0;
    prevSamplePos = 0;

    soundStream.setup(this, 0, nCh, sampleRate, bufferSize, 4);
    audioData.assign( soundStream.getBufferSize(), 0.0f );

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
    
    
#ifdef RENDER
    exp.setup(1920, 1080, 30);
    exp.setFrameRange(0, 3*60*30+1);
    exp.setFilePattern("out/ob_%05i.png");
    exp.setOverwriteSequence(true);
    exp.setAutoExit(true);
    exp.startExport();
#endif
}

void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    if( bStart ){
        audioData.clear();
        audioData.insert(audioData.begin(), input, input+bufferSize*nChannels);
        currentSamplePos += soundStream.getBufferSize();
    }
}

void ofApp::audioPreProcess(){
    
    // LOG scale
    if(bLog){
        float strength = 3;
        float base = log10(1+strength);
        
        for_each(audioData.begin(), audioData.end(), [&](float &val){
            float sign = signval<float>(val);
            float log = log10(1+abs(val)*strength) / base;
            val = log * sign;
        });
    }

    // lowpass
    if(1){
        float rate = 0.5;
        float prev = 0;
        for(int i=2; i<audioData.size()-2; i++){
            float v1 = audioData[i-2] * 0.1;
            float v2 = audioData[i-1] * 0.3;
            float v3 = audioData[i-0] * 0.5;
            float v4 = audioData[i+1] * 0.3;
            float v5 = audioData[i+2] * 0.1;
            audioData[i] = (v1+v2+v3+v4+v5)/1.3f;
        }
    }
    
    // stats
    if( ofGetFrameNum() % (targetFps/20) == 0){
        //ob::clear();
        ob::calc(audioData);
    }else{
        ob::dim();        
    }
}

void ofApp::update(){

    if( !bStart ) return;
    
    audioPreProcess();

    float sampleRate = soundStream.getSampleRate();
    int totalSampleNum = total_time_ms/1000 * sampleRate;
    indicator.x = (float)currentSamplePos/totalSampleNum * track_len;
    
// make vbo from video
//    grabber.update();
//    if(grabber.isFrameNew()){
//        ofPixels & pix = grabber.getPixels();
//        int w = pix.getWidth();
//        int h = pix.getHeight();
//        for(int y=0; y<h; y++){
//            for(int x=0; x<w; x++){
//                int index = x + y*w;
//                ofFloatColor c = pix.getColor(index);
//                
//            }
//        }
//    }
    
    if (indicator.x >= track_len) {
        cout << "tracking finished : " << ofGetElapsedTimef() << endl;
        //bStart = false;
    }
    
    prevSamplePos = currentSamplePos;
}

void ofApp::draw(){
    
#ifdef RENDER
    exp.begin();
#endif
    ofSetLineWidth(1);
    glPointSize(1);
    ofBackground(255);
    ofPushMatrix();{
    
        if(bHandy){
            ofRotateZ(90.0);
            ofTranslate(0, -ofGetWindowWidth());
        }

        ofTranslate(start_point);

        draw_bg();
        draw_wave();
        draw_audioStats();
        
    }ofPopMatrix();
    
    draw_info();

#ifdef RENDER
    exp.end();
    exp.draw(0, 0);
#endif
        
}

void ofApp::draw_bg(){
    
    ofSetColor(0, 0, 0);
    float yy = canvas.height/2*0.8;
    ofDrawLine(indicator.x, +yy+10, indicator.x, +yy);
    ofDrawLine(indicator.x, -yy-10, indicator.x, -yy);
    ofDrawLine(0, +yy+10, 0, +yy);
    ofDrawLine(0, -yy-10, 0, -yy);
    
    ofDrawLine(          0,    -5,         0,      +5);
    ofDrawLine(  track_len,    -5, track_len,      +5);
    
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(255,255,0,255);
    ofFill();
    ofDrawRectangle(0, -yy, indicator.x, yy*2);
    
    // text sec
    ofSetColor(0);
    ofDrawBitmapString(ofToString(ofGetElapsedTimef()), indicator.x, yy+40);
    
}

void ofApp::draw_wave(){

    if(audioData.size()!=0){
        
        ob::DrawerSettings & s = ob::dset;
        s.app = this;
        s.indicator = indicator;
        s.data = &audioData;
        s.track_len = track_len;
        s.buffer_size = soundStream.getBufferSize();
        s.xrate = track_len/s.buffer_size;
        s.global_amp = canvas.height/2 * 0.8;
        
        int start = 0;
        const int end = s.buffer_size;
        bool loop = true;
        
        while( loop ){
            
            float n1 = ofNoise( ofGetDay(), ofGetElapsedTimef(), start );
            int type_max = 7;
            int type = round(n1 * type_max);
            
            float n2 = ofNoise( ofGetHours() , ofGetFrameNum()*2.0, start );
            n2 = pow(n2, 8) * ofRandom(1.0f,10.0f);
            
            int num_min = s.buffer_size * 0.01;
            int num_max = s.buffer_size * 0.05;
            int num = num_min + n2*num_max;
            
            if(type == 3) num*=0.25;
            
            if((start+num)>=end){
                num =  end-start-1;
                loop = false;
                if(num<=2) break;
            }
            
            switch (type) {
                case 0: ob::draw_line_wave(start, num); break;
                case 1: ob::draw_dot_wave(start, num); break;
                case 2: ob::draw_prep_line(start, num); break;
                case 3: ob::draw_circle(start, num); break;
                case 4: ob::draw_rect(start, num); break;
                case 5: ob::draw_log_wave(start, num); break;
                case 6: ob::draw_arc(start, num, 0.5); break;
                case 7: ob::draw_prep_line_inv(start, num, 0.33f); break;
                    
                default: break;
            }
            
            start += num;
        }
    }
}

void ofApp::draw_audioStats(){
    
    if(bStart && ob::audioStats.min!=numeric_limits<float>::max()){
        const int bufferSize = soundStream.getBufferSize();
        const float xrate = track_len/bufferSize;
        const float amp = ob::dset.global_amp;

        {
            // max
            float x = ob::audioStats.index_max * xrate;
            float y = ob::audioStats.max * amp;
            ofFill();
            ofSetColor(0, 0, 200);
            ofDrawLine(track_len, y, track_len+10, y);
        }

        {
            // min
            float x = ob::audioStats.index_min * xrate;
            float y = ob::audioStats.min * amp;
            ofFill();
            ofSetColor(0, 0, 200);
            ofDrawLine(track_len, y, track_len+10, y);
        }

    }
}

void ofApp::draw_info(){
    int y = 10;
    int x = 10;
    int os = 200;
    ofSetColor(0);
    ofDrawBitmapString("fps        " + ofToString(ofGetFrameRate()), x, y);
    ofDrawBitmapString("nChannels  " + ofToString(soundStream.getNumInputChannels() ), x+=os, y);
    ofDrawBitmapString("bufferSize " + ofToString(soundStream.getBufferSize()), x+=os, y);
    ofDrawBitmapString("sampleRate " + ofToString(soundStream.getSampleRate()), x+=os, y);
}

void ofApp::keyPressed(int key){
    switch (key) {
        case ' ': bStart = !bStart;     break;
        case 'l': bLog   = !bLog;       break;
    };
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