#include "Initializer.h"

#include "ofMain.h"
#include <squash/squash.h>

namespace ofxSquash {
	//----------
	void initialize() {
		static bool initialized = false;
		if (!initialized) {
			string pluginDir = getPluginDir();
			
			if (!ofDirectory(pluginDir).exists()) {
				pluginDir = getPluginOriginDir();
			}
			
			pluginDir = ofToDataPath(pluginDir);
			
			ofStringReplace(pluginDir, "\\", "/");
			squash_set_default_search_path(pluginDir.c_str());
			initialized = true;
		}
	}

	//----------
	string getPluginDir() {
		return "../plugins/squash";
	}

	//----------
	string getPluginOriginDir() {
		string platform, arch = sizeof(void*) == 8 ? "x64" : "x86";
#if defined(_MSC_VER)
		platform = "vs";
#elif defined(__MACOSX_CORE__)
		platform = "osx";
#endif
		return "../../../../ofxSquash/libs/squash/bin/" + platform + "/" + arch + "/plugins/squash/";
	}
}