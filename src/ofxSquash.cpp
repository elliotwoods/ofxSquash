#include "ofxSquash.h"

#include "ofMain.h"
#include <squash/squash.h>

//----------
void populateCodecListCallback(SquashCodec * squashCodec, void * data) {
	auto codecList = static_cast<ofxSquash::CodecList *>(data);
	auto inserter = make_pair(string(), ofxSquash::Codec(squashCodec));
	inserter.first = inserter.second.getName();
	codecList->insert(inserter);
}

namespace ofxSquash {
	//----------
	SquashInitializer::SquashInitializer() {
		auto pluginDir = ofToDataPath("../plugins/squash", true);
		ofStringReplace(pluginDir, "\\", "/");
		squash_set_default_search_path(pluginDir.c_str());
	}

	//----------
	SquashInitializer squashInitializer;

	//----------
	map<string, Codec> getCodecList(bool returnOnlyValid) {
		CodecList codecs;

		squash_foreach_codec(populateCodecListCallback, &codecs);

		if (returnOnlyValid) {
			//erase invalid codecs
			for (auto it = codecs.begin(); it != codecs.end(); ) {
				if (it->second.isValid()) {
					it++;
				}
				else {
					it = codecs.erase(it);
				}
			}
		}
		
		return codecs;
	}
}

