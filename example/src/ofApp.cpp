#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	kanaKey.setup("NotoSansCJKjp-Regular.otf", 22);
}

//--------------------------------------------------------------
void ofApp::update(){
	float now = ofGetElapsedTimef();
	kanaKey.update(now);
}

//--------------------------------------------------------------
void ofApp::draw(){
	kanaKey.drawSentence();
	kanaKey.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	kanaKey.toggle();
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (button == 0) {
		kanaKey.setPressed(x, y);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
