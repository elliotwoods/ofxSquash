#pragma once

#include "ofFileUtils.h"

#include <squash/squash.h>
#include <string>

using namespace std;

namespace ofxSquash {
	class Codec {
	public:
		Codec();
		Codec(SquashCodec *);
		Codec(string codecName);

		bool isValid() const;
		const string & getName() const;

		size_t getMaxCompressedSize(size_t uncompressedSize) const;

		size_t compress(uint8_t * compresed, size_t compressedSize, uint8_t * uncompressed, uint8_t uncompressedSize) const;
		void compress(string & compressed, const string & uncompressed) const;
		string compress(const string & uncompressed) const;

	protected:
		string name;
		SquashCodec * squashCodec;
		bool valid;
	};
}