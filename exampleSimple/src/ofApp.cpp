#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(0);

	this->codec = ofxSquash::getCodecList()["snappy"];
}

//--------------------------------------------------------------
void ofApp::update(){
	string text = "\
		I'm going to be squashed! \n\
		I'm going to be squashed! \n\
		I'm going to be squashed! \n\
		I'm going to be squashed! \n\
		I'm going to be squashed! \n\
		I'm going to be squashed! \n\
		";

	string compressedText = this->codec.compress(text);
	
	string uncompressedText = this->codec.decompress(compressedText);

	cout << "Original" << endl;
	cout << "========" << endl; 
	cout << text << endl;

	cout << "Compressed" << endl;
	cout << "========" << endl;
	cout << compressedText << endl;

	cout << "Uncompressed" << endl;
	cout << "============" << endl;
	cout << uncompressedText << endl;

	cout << endl;
	cout << text.size() << "bytes->" << compressedText.size() << "bytes" << endl;
	cout << endl;
	cout << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofDrawBitmapString("Check the console", 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

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
