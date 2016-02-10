#include "ofApp.h"

void ofApp::setup() {
	// Usually when we zip or unzip a file, we:
	//	* Read from a file
	//	* Compress
	//	* Write to a file
	//
	// We could read the whole file into memory, then
	//	compress/decompress to another part of memory,
	//	and then write to disk. But this requires a lot
	//	of memory.
	//
	// With streaming, we perform each action 'a bit at
	//	a time', moving data through through the stream.
	//
	// One huge advantage to this appraoch is latency
	//	when sending over a network. Because we can send
	//	the data whilst we are still compressing it
	//	(rather than waiting for each process to finish
	//	before starting the next one.



	// Choose a codec to use for compression and decompression
	auto codec = ofxSquash::getCodecList()["density"];



	// Here we initialize the streams. Usually you might
	//	only want to either a compressor or a decompresser
	//	at one time.
	ofxSquash::Stream compressStream(codec, ofxSquash::Direction::Compress);
	ofxSquash::Stream decompressStream(codec, ofxSquash::Direction::Decompress);



	// Setup a WriteFunction for compression.
	// This function is called whenever the stream wants
	//	to write to an output (e.g. a file or a network
	//	stream). This function will be called periodically
	//	as the stream compresses the data.
	compressStream.setWriteFunction([&decompressStream](const uint8_t* data, size_t size) {
		cout << "Compressed" << endl;
		cout << "==========" << endl;

		auto compressedPacketString = string((const char*)data, size);
		cout << compressedPacketString << endl;

		// In this example, our 'write' code for the compressor
		//	feeds data into the decompressor's read funciton.
		decompressStream << compressedPacketString;

		cout << size << endl;
	});

	// Setup a WriteFunction for our decompressor
	//	(same as above).	
	decompressStream.setWriteFunction([](const uint8_t* data, size_t size) {
		cout << "Decompressed" << endl;
		cout << "============" << endl;
		cout << string((const char*)data, size) << endl;
	});



	// Stream something into the compressor.
	// 
	string text = " \n\
		....XXXXXXXX \n\
		..XXX..ooooXXX \n\
		..X....oooo..X \n\
		.X....oooooo..X \n\
		XX...oo....oo.XX \n\
		Xooooo......oooX \n\
		Xo..oo......oooX \n\
		X....o......oo.X \n\
		X....oo....oo..X \n\
		Xo..ooooooooo..X \n\
		XoooXXXXXXXXoo.X \n\
		.XXX..X..X..XXX \n\
		..X...X..X...X \n\
		..X..........X \n\
		...X........X \n\
		....XXXXXXXX \n\
		";
	for (int i = 0; i < 10; i++)
	{
		compressStream << text << std::to_string(i);
	}



	// Let the compression stream know that we've reached
	//	the end. This will compress whatever is remaining
	//	in the stream's buffer and close the stream.
	compressStream << ofxSquash::Stream::Finish();

	// Same again for decompressor
	decompressStream << ofxSquash::Stream::Finish();



	// Note that in this example:
	//	* The stream's buffer is set to default (8 KB), which
	//		is much larger than the message. Therefore most of
	//		the action will happen when we Finish the stream.

	ofSystemAlertDialog("The results have been posted to the console (sorry, no gui!).");
	ofExit();
}