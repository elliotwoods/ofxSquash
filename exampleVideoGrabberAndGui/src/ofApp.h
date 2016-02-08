#pragma sonce

#include "ofMain.h"
#include "ofxSquash.h"
#include "ofxCvGui.h"
#include "ofxProfiler.h"

class ofApp : public ofBaseApp{
	struct Test {
		ofxSquash::Codec codec;
		ofPixels compressed;

		float compressionTime = 0.0f;
		float decompressionTime = 0.0f;
		size_t compressedSize = 0;

		float compressionSpeed = 0.0f;
		float compressionRatio = 1.0f;

		ofVec2f graphPosition;

		thread thread;
	};

	public:
		void setup();
		void update();
		void draw();
		void exit() override;

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofxCvGui::Builder gui;

		ofVideoGrabber video;
		ofTexture compressed;

		vector<shared_ptr<Test>> tests;
		vector<shared_ptr<Test>>::iterator testIterator;
		weak_ptr<Test> selectedTest;

		bool testThreadRunning = true;
		thread testThread;

		float maxCompressionRatio = 1.0f;
		float maxCompressionSpeed = 1.0f;
};
