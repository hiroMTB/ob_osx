#include "ofApp.h"

void ofApp::setup(){
    // ofRegisterURLNotification(this);
}

void ofApp::loadFile(){
    
    std::string file = "test.json";
    
    // Now parse the JSON
    bool parsingSuccessful = result.open(file);
    
    if (parsingSuccessful){
        ofLogNotice("ofApp::setup") << result.getRawString();
        
        // now write pretty print
        if (!result.save("example_output_pretty.json", true)){
            ofLogNotice("ofApp::setup") << "example_output_pretty.json written unsuccessfully.";
        }else{
            ofLogNotice("ofApp::setup") << "example_output_pretty.json written successfully.";
        }
        
        // now write without pretty print
        if (!result.save("example_output_fast.json", false)){
            ofLogNotice("ofApp::setup") << "example_output_pretty.json written unsuccessfully.";
        }else{
            ofLogNotice("ofApp::setup") << "example_output_pretty.json written successfully.";
        }
    }else{
        ofLogError("ofApp::setup")  << "Failed to parse JSON" << endl;
        ofExit();
    }
}

void ofApp::requestBear(){

    string baseurl = "https://api.developer.oralb.com/v1";
    string id = "fac078cd-1cf0-4e04-82ab-385a98359d09";
    string key = "6bb594ec-2860-427e-8e34-891fdb33995d";

    
    // 1.request
    {
        ofURLFileLoader loader;
        ofHttpRequest req;
        ofHttpResponse response;
        ofxJSONElement json;
        
        string url = baseurl + "/bearertoken/" + id + "?key=" + key;
        req.url = url;
        req.name = "Request Bearertoken";
        response = loader.handleRequest(req);
        if(response.status != 200) {
            cout << "ERROR " << response.status << " : " << response.error << endl;
            return;
        }
        
        ofBuffer buf = response.data;
        string raw = ofToString(buf);
        bool ok = json.parse(raw);
        if(ok) bear = json["bearerToken"].asString();
        
        result = json;
    }
    
    
    // 2.request
    {
        ofURLFileLoader loader;
        ofHttpResponse response;
        ofHttpRequest req;
        ofxJSONElement json;
        string url = baseurl + "/authorize";
        req.url = url;
        req.headers["Authorization"] = "Bearer " + bear;
        req.name = "Request Authorize";
        response = loader.handleRequest(req);
        if(response.status != 200) {
            cout << "ERROR " << response.status << " : " << response.error << endl;
            return;
        }
        
        ofBuffer buf = response.data;
        string raw = ofToString(buf);
        bool ok = json.parse(raw);
        
        if(ok){
            string authUrl = json["url"].asString();
            ofLoadURL( authUrl );
            cout << authUrl << endl;
        }
        
        result = json;
    }
    
}

void ofApp::draw(){
    ofBackground(0);
    ofPushMatrix();{
        ofTranslate(20, 20);
        ofSetHexColor(0x00FF00);
         draw_json(result);
    }ofPopMatrix();
    
    
    ofPushMatrix();{
        ofTranslate(0, ofGetHeight()/2);
        ofSetColor(125);
        ofFill();
        ofDrawRectangle(0, 0, 0, ofGetWidth(), ofGetHeight()/2);
        ofSetColor(0);

        ofDrawBitmapString("RAW json", 20, 20);
        ofDrawBitmapString(result.getRawString(), 20, 40);
    }ofPopMatrix();
}

void ofApp::keyPressed(int key){

    switch(key){
        case 'w':
            requestBear();
            break;

        
        case 'f':
            loadFile();
            break;
    }
}

/*
 *      parse everything with iterator
 *      alphabetical order because of map container
 */
void ofApp::draw_json( ofxJSONElement & elem){

    std::stringstream ss;

    Json::Value::iterator itrA = elem.begin();
        
    for( ; itrA!=elem.end(); itrA++){

        string name = itrA.memberName();
        string data = (*itrA).asString();
        ss << name << " : " << data << "\n";
        
        Json::Value::iterator itrB = (*itrA).begin();
        
        for( ; itrB!=(*itrA).end(); itrB++){
            string name = itrB.memberName();
            string data = (*itrB).asString();
            ss << name << " : " << data << "\n";
        }
        
        ss << "\n\n";
    }
    
    ofDrawBitmapString(ss.str(), 0, 0);
}







void ofApp::draw_example(){
    std::stringstream ss;

    //    {
    //        "firstName": "John",
    //        "lastName": "Smith",
    
    ss << "firstName          = " << result["firstName"].asString() << std::endl;
    ss << "lastName           = " << result["lastName"].asString() << std::endl;
    
    //        "age": 25,
    
    ss << "age                = " << result["age"].asDouble() << endl;
    
    //        "address": {
    //            "streetAddress": "21 2nd Street",
    //            "city": "New York",
    //            "state": "NY",
    //            "postalCode": "10021"
    //        },
    
    ss << "streetAddress      = " << result["address"]["streetAddress"].asString() << std::endl;
    ss << "city               = " << result["address"]["city"].asString() << std::endl;
    ss << "state              = " << result["address"]["state"].asString() << std::endl;
    ss << "postalCode         = " << result["address"]["postalCode"].asString() << std::endl;
    
    //        "phoneNumber": [
    //                        {
    //                        "type": "home",
    //                        "number": "212 555-1234"
    //                        },
    //                        {
    //                        "type": "fax",
    //                        "number": "646 555-4567"
    //                        }
    //                        ]
    //    }
    
    int i = 0;
    
    ss << "phoneNumber:type   = " << result["phoneNumber"][i]["type"].asString() << std::endl;
    ss << "phoneNumber:number = " << result["phoneNumber"][i]["number"].asString() << std::endl;
    ss << "phoneNumber:type   = " << result["phoneNumber"][i+1]["type"].asString() << std::endl;
    ss << "phoneNumber:number = " << result["phoneNumber"][i+1]["number"].asString() << std::endl;
    
    ofDrawBitmapString(ss.str(), 10, 14);
    
}
