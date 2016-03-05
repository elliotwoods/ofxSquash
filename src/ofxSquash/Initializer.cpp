#include "Initializer.h"

#include "ofMain.h"
#include <squash/squash.h>

namespace ofxSquash {
	//----------
	void initialize() {
		static bool initialized = false;
		if (!initialized) {
			
			string platform, arch = sizeof(void*) == 8 ? "x64" : "x86";
#if defined(_MSC_VER)
			platform = "vs";
#elif defined(__MACOSX_CORE__)
			platform = "osx";
#endif
			
			string pluginDir = ofToDataPath("../../../../ofxSquash/libs/squash/bin/" + platform + "/" + arch + "/plugins/squash/");
			
			if (!ofDirectory(pluginDir).exists()) {
				pluginDir = ofToDataPath("../plugins/squash", true);
			}
			
			ofStringReplace(pluginDir, "\\", "/");
			squash_set_default_search_path(pluginDir.c_str());
			initialized = true;
		}
	}
}