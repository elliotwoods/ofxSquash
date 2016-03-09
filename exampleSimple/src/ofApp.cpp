#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(0);
	
	//Copy codecs (don't do this normally!)
	{
		//We really don't want to do this inside our program.
		//You should do this once by hand by copying the plugins yourself.
		auto dir = ofDirectory(ofxSquash::getPluginOriginDir());
		dir.copyTo(ofxSquash::getPluginDir(), true);
		
		string whereThePluginsAreKeptInTheAddon = ofxSquash::getPluginOriginDir();
		
		string whereYouShouldPutThePlugins = ofxSquash::getPluginDir();
		
		//Note that if the folder `getPluginDir()` does not exist,
		// then ofxSquash will also check `getPluginOriginDir()`
		// to find plugins.
	}

	
	//List codecs
	{
		auto codecs = ofxSquash::getCodecList();
		cout << "Squash found " << codecs.size() << " codecs." << endl;
		
		if(codecs.empty()) {
			ofSystemAlertDialog("No squash codecs found! Did you copy the squash plugins?");
			ofExit();
		}
		
	}
	
	
	//Compress and decompress
	{
		//Get a codec
		auto codecs = ofxSquash::getCodecList();
		auto codec = codecs["snappy"];
		
		//If it's not valid, then maybe the plugin is faulty (not all plugins work on all platforms).
		if(!codec.isValid()){
			
			//An alternative way to get a codec is to use the ofxSquash::Codec constructor.
			codec = ofxSquash::Codec("lz4");
			
		}
	
		string text = "\
		I'm going to be squashed! \n\
		I'm going to be squashed! \n\
		I'm going to be squashed! \n\
		I'm going to be squashed! \n\
		I'm going to be squashed! \n\
		I'm going to be squashed! \n\
		";
		
		//COMPRESS
		string compressedText = codec.compress(text);
		
		//DECOMPRESS (recover the original)
		string decompressedText;
		auto uncompressedSize = codec.getUncompressedSize(compressedText);
		if(uncompressedSize == 0) {
			// Not all codecs support getting the uncompressed size from the compressed data.
			
			// In this case, we just have to hope we've allocated enough.
			// (don't worry, the decompression will fail safely if we don't allocate enough).
			decompressedText.resize(1000);
			
			codec.decompress(decompressedText, compressedText);
		}
		
		cout << "Original (Uncompressed)" << endl;
		cout << "=======================" << endl;
		cout << text << endl;
		
		cout << "Compressed" << endl;
		cout << "========" << endl;
		cout << compressedText << endl;
		
		cout << "Decompressed" << endl;
		cout << "============" << endl;
		cout << decompressedText << endl;
		
		cout << endl;
		cout << text.size() << "B uncompressed -> " << compressedText.size() << "B compressed" << endl;
		cout << endl;
	}
	
	ofSystemAlertDialog("This example only outputs to the console. Please check there.");
	ofExit();
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){

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
