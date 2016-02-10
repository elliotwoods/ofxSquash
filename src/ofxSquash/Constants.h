#pragma once

#include "ofLog.h"

#ifndef __func__
#define __func__ __FUNCTION__
#endif
#define OFXSQUASH_NOTICE ofLogNotice(string(__func__))
#define OFXSQUASH_WARNING ofLogWarning(string(__func__))
#define OFXSQUASH_ERROR ofLogError(string(__func__))
#define OFXSQUASH_FATAL ofLogFatalError(string(__func__))

namespace ofxSquash {
	enum Direction {
		Compress,
		Decompress
	};
}
