#include "ofMain.h"
#include "ofApp.h"

namespace ob {
    
    
    void draw_dot_wave( ofApp * app, int start, int n, float amp ){
        
        ofVec2f & indicator = app->indicator;
        vector<vector<float>> & data = app->audioData;
        int ch = app->nCh;
        ofVboMesh & vp = app->vPoints;
        ofVboMesh & vl = app->vLines;
        ofVboMesh & va = app->vAudio;
        
        for( int i=0; i<n; i++){
            int id = start + i;
            
            for( int c=0; c<ch; c++){
                float d = data[c][id];
                float x = indicator.x;
                float y = d * amp + c*50;
                
                vp.addVertex( ofVec3f(x,y,0) );
                vp.addColor( ofFloatColor(0,0,0,1) );
            }
            
        }
    }
    
    
}