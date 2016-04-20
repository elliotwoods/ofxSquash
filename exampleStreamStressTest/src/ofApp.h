#pragma once

#include "ofMain.h"
#include "ofxProfiler.h"
#include "ofxSquash.h"

#include "ofxCvGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();

		ofxCvGui::Builder gui;

		struct : ofParameterGroup {
			ofParameter<string> codec{ "Codec", "density" };
			ofParameter<bool> cutOffCompression{ "Cut off compression", false };
			ofParameter<int> originalSize{ "Original size", 0 };
			ofParameter<int> compressedSize{ "Compressed size", 0 };
			ofParameter<int> decompressedSize{ "Decompressed size", 0 };
			PARAM_DECLARE("Parameters", codec, cutOffCompression, compressedSize, decompressedSize);
		} parameters;
};
