#pragma once

#include "ofxSquash/Codec.h"
#include "ofxSquash/Stream.h"
#include "ofxSquash/Initializer.h"

#include <vector>
#include <map>

using namespace std;

namespace ofxSquash {
	typedef std::map<std::string, Codec> CodecList;
	std::map<std::string, Codec> getCodecList(bool returnOnlyValid = true);
}