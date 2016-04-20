#include "ofApp.h"

void ofApp::setup() {
	this->gui.init();
	auto widgets = this->gui.addWidgets();

	widgets->addTitle("ofxSquash Stream Stress Test");
	widgets->addFps();
	widgets->addMemoryUsage();
	widgets->addParameterGroup(this->parameters);
}

void ofApp::update() {
	//every frame we perform the compression

	ofxSquash::Codec codec(this->parameters.codec);
	
	this->parameters.compressedSize = 0;
	this->parameters.decompressedSize = 0;

	//setup the decompressor (output is just ignored)
	ofxSquash::WriteFunction decompressWriteFunction = [this](const ofxSquash::WriteFunctionArguments & args) {
		this->parameters.decompressedSize += args.size;
	};
	ofxSquash::Stream decompressor(codec, ofxSquash::Decompress, decompressWriteFunction);

	//setup the compressor (output is fed into decompressor)
	ofxSquash::WriteFunction compressWriteFunction = [&](const ofxSquash::WriteFunctionArguments & args) {
		this->parameters.compressedSize += args.size;

		decompressor.read(args.data, args.size);
		if (args.isFinished) {
			decompressor << ofxSquash::Stream::Finish();
		}
	};
	ofxSquash::Stream compressor(codec, ofxSquash::Compress, compressWriteFunction);

	//create a payload
	ofPixels pixels(ofImage("test.png"));
	this->parameters.originalSize = pixels.getTotalBytes();

	//read payload into stream
	compressor.read(pixels.getData(), pixels.getTotalBytes());

	//what happens when the stream is not properly closed? (e.g. dropped packets)
	if (!this->parameters.cutOffCompression) {
		compressor << ofxSquash::Stream::Finish();
	}
}
