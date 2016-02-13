#pragma once

#include "ofxSquash/Codec.h"
#include "ofxSquash/Stream.h"

#include <vector>

using namespace std;

namespace ofxSquash {
	typedef map<string, Codec> CodecList;
	map<string, Codec> getCodecList(bool returnOnlyValid = true);
}