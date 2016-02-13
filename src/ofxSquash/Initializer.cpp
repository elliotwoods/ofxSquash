#include "Initializer.h"

#include "ofMain.h"
#include <squash/squash.h>

namespace ofxSquash {
	//----------
	void initialize() {
		static bool initialized = false;
		if (!initialized) {
			auto pluginDir = ofToDataPath("../plugins/squash", true);
			ofStringReplace(pluginDir, "\\", "/");
			squash_set_default_search_path(pluginDir.c_str());
			initialized = true;
		}
	}
}