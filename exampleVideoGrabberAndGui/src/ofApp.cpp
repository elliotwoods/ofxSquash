#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//Setup the video grabber
	this->video.initGrabber(640, 480);

	//--
	//Setup the gui
	//--
	//
	this->gui.init();
	
	auto verticalStrip = ofxCvGui::makeStrip(ofxCvGui::Panels::Groups::Strip::Direction::Vertical);
	auto horizontalStrip = ofxCvGui::makeStrip();

	auto videoPanel = ofxCvGui::makePanel(this->video, "Camera");
	auto compressedPanel = ofxCvGui::makePanel(this->compressed, "Compressed");
	auto graphPanel = ofxCvGui::makeBlank("Results");

	horizontalStrip->add(videoPanel);
	horizontalStrip->add(compressedPanel);
	horizontalStrip->setCellSizes({666, 333});

	verticalStrip->add(horizontalStrip);
	verticalStrip->add(graphPanel);

	this->gui.add(verticalStrip);
	//
	//--


	//--
	//Setup the tests
	//--
	//
	auto codecs = ofxSquash::getCodecList();
	for (auto it : codecs) {
		auto test = make_shared<Test>();
		test->codec = it.second;
		test->compressed.allocate(this->video.getWidth(), this->video.getHeight() * 2, OF_IMAGE_COLOR);

		this->tests.push_back(test);
	}
	this->testIterator = this->tests.begin();
	//
	//--


	//--
	//Setup the test thread
	//--
	//
	this->testThread = thread([this]() {
		string decompressed;
		auto & videoPixels = this->video.getPixels();

		while (this->testThreadRunning) {
			if (testIterator == this->tests.end()) {
				testIterator = this->tests.begin();
			} else {
				auto test = *testIterator;
				auto & codec = test->codec;

				decompressed.resize(this->video.getWidth() * this->video.getHeight() * 6);
				size_t compressedSize;

				ofxProfiler::Activity compressionProfiler;
				{
					auto scopedActivity = ofxProfiler::Scope(compressionProfiler);

					test->compressed.set(255);

					compressedSize = codec.compress(test->compressed, videoPixels);
				}

				ofxProfiler::Activity decompressionProfiler;
				{
					auto scopedActivity = ofxProfiler::Scope(decompressionProfiler);

					codec.decompress((uint8_t*)&decompressed[0], decompressed.size(), test->compressed.getData(), compressedSize);
				}

				if (compressedSize != 0) {
					test->compressionTime = compressionProfiler.getDuration();
					test->decompressionTime = decompressionProfiler.getDuration();
					test->compressedSize = compressedSize;

					auto uncompressedSize = (float)videoPixels.size();
					test->compressionSpeed = uncompressedSize / test->compressionTime / 1e6;
					test->compressionRatio = (float)compressedSize / uncompressedSize;
				}

				testIterator++;
			}
		}
	});


	//--
	//Setup the graph
	//--
	//
	graphPanel->onDraw += [this](ofxCvGui::DrawArguments & args) {
		
		//limits
		this->maxCompressionRatio = 0.00001;
		this->maxCompressionSpeed = 0.00001;
		for (auto test : this->tests) {
			if (test->compressionRatio > this->maxCompressionRatio) {
				this->maxCompressionRatio = test->compressionRatio;
			}

			if (test->compressionSpeed > this->maxCompressionSpeed) {
				this->maxCompressionSpeed = test->compressionSpeed;
			}
		}

		this->maxCompressionRatio += 0.1f;
		this->maxCompressionSpeed += 0.1f;

		//to graph coords function
		auto toGraph = [this, args](float compressionRatio, float compressionSpeed) {
			ofVec2f pos;
			
			pos.x = ofMap(log(compressionRatio), log(0.1), log(this->maxCompressionRatio), 40, args.localBounds.width - 40);
			pos.y = ofMap(log(compressionSpeed), log(1), log(this->maxCompressionSpeed), args.localBounds.height - 40, 40);

			return pos;
		};

		//axes
		ofDrawLine(30, args.localBounds.height - 40, args.localBounds.width - 40, args.localBounds.height - 40); //x
		ofDrawLine(40, 40, 40, args.localBounds.height - 30); //y
		//
		ofDrawBitmapString("Compressed ratio", 45, args.localBounds.height - 25); // x axis
		ofPushMatrix();
		{
			ofTranslate(40, args.localBounds.height - 40);
			ofRotate(-90);
			ofSetDrawBitmapMode(ofDrawBitmapMode::OF_BITMAPMODE_MODEL);
			ofDrawBitmapString("Compression speed", 5, -5); // y axis
		}
		ofPopMatrix();

		//gridlines
		ofPushStyle();
		ofSetColor(100);
		{
			for (float compressionRatio = 0.1; compressionRatio < this->maxCompressionRatio; compressionRatio += 0.1f) {
				auto pos = toGraph(compressionRatio, 0);
				ofLine(pos.x, 40, pos.x, args.localBounds.height - 40);
				ofDrawBitmapString(ofToString(compressionRatio) + "x", pos.x + 2, args.localBounds.height - 40 - 2);
			}
			for (float compressionSpeed = 50; compressionSpeed < this->maxCompressionSpeed; compressionSpeed += 50.0f) {
				auto pos = toGraph(0, compressionSpeed);
				ofLine(40, pos.y, args.localBounds.width - 40, pos.y);
				ofDrawBitmapString(ofToString(compressionSpeed) + "MB/s", 40 + 2, pos.y - 2);
			}
		}
		ofPopStyle();

		//draw datapoints
		for (auto test : this->tests) {
			test->graphPosition = toGraph(test->compressionRatio, test->compressionSpeed);
			auto & x = test->graphPosition.x;
			auto & y = test->graphPosition.y;

			ofPushMatrix();
			{
				ofTranslate(x, y);
				ofDrawBitmapString(test->codec.getName(), 3, -3);

				ofCircle(0, 0, 3.0f);
			}
			ofPopMatrix();
		}

		//draw selected datapoint (on top
		if (!this->selectedTest.expired()) {
			auto selectedTest = this->selectedTest.lock();
			ofPushStyle();
			ofPushMatrix();
			{
				ofTranslate(selectedTest->graphPosition);
				ofDrawBitmapStringHighlight(selectedTest->codec.getName(), 5, -5, ofColor(255, 100, 100));
				ofSetColor(255, 100, 100);
				ofCircle(0, 0, 8.0f);
			}
			ofPopMatrix();
			ofPopStyle();
		}
	};

	graphPanel->onMouse += [this](ofxCvGui::MouseArguments & args) {
		if (args.isLocal()) {
			float distance = std::numeric_limits<float>::max();
			shared_ptr<Test> newSelection = nullptr;
			for (auto test : this->tests) {
				auto distanceToTest = (test->graphPosition - args.local).lengthSquared();
				if (distanceToTest < distance) {
					distance = distanceToTest;
					newSelection = test;
				}
			}
			if (newSelection) {
				this->selectedTest = newSelection;
			}
		}
	};
	//
	//--


	//--
	//Setup compressed
	//--
	//
	compressedPanel->onDraw += [this](ofxCvGui::DrawArguments & args) {
		if (!this->selectedTest.expired()) {
			auto selectedTest = this->selectedTest.lock();

			stringstream status;
			status << "Codec : " << selectedTest->codec.getName() << endl;
			status << endl;
			status << "Compressed size : " << selectedTest->compressedSize << endl;
			status << "Compression time : " << selectedTest->compressionTime << endl;
			status << "Decompression time : " << selectedTest->decompressionTime << endl;
			status << endl;
			status << "Compression ratio : " << selectedTest->compressionRatio << endl;
			status << "Compression speed [MB/s] : " << selectedTest->compressionSpeed << endl;

			ofSetDrawBitmapMode(ofDrawBitmapMode::OF_BITMAPMODE_SIMPLE);
			ofDrawBitmapStringHighlight(status.str(), ofVec2f(50, args.localBounds.height - 150), ofColor(100));
		}
	};
	compressedPanel->onDrawCropped += [this](ofxCvGui::Panels::Draws::DrawCroppedArguments & args) {
		ofPushStyle();
		ofSetDrawBitmapMode(ofDrawBitmapMode::OF_BITMAPMODE_MODEL_BILLBOARD);
		for (float percent = 0; percent <= 200; percent += 20) {
			ofPushMatrix();
			{
				ofTranslate(0, percent / 200.0f * args.drawSize.y);
				ofLine(0, 0, 5, 0);
				ofDrawBitmapStringHighlight(ofToString(percent) + "%", ofPoint(10, 5));
			}
			ofPopMatrix();
		}
		ofPopStyle();

		if (!this->selectedTest.expired()) {
			auto selectedTest = this->selectedTest.lock();
			float sizeLineY = selectedTest->compressionRatio * args.drawSize.y / 2.0f;

			ofPushStyle();
			{
				ofSetLineWidth(5.0f);
				ofSetColor(0);
				ofLine(0, sizeLineY, args.drawSize.x, sizeLineY);
				ofSetLineWidth(2.0f);
				ofSetColor(255);
				ofLine(0, sizeLineY, args.drawSize.x, sizeLineY);
			}
			ofPopStyle();
		}
	};
	//
	//--
}

//--------------------------------------------------------------
void ofApp::update(){
	this->video.update();

	if (!this->selectedTest.expired()) {
		auto selectedTest = this->selectedTest.lock();
		this->compressed.loadData(selectedTest->compressed);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::exit() {
	this->testThreadRunning = false;
	testThread.join();
	
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
