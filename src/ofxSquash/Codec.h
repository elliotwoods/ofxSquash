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
		
		size_t getUncompressedSize(void * compressed, size_t compressedSize) const;
		size_t getUncompressedSize(const string & compressed) const;

		size_t compress(void * compresed, size_t compressedSize, const void * uncompressed, size_t uncompressedSize) const;
		void compress(string & compressed, const string & uncompressed) const;
		string compress(const string & uncompressed) const;

		size_t decompress(void * uncompressed, size_t uncompressedSize, const void * compressed, size_t compressedSize) const;
		void decompress(string & uncompressed, const string & compressed) const;
		string decompress(const string & compressed) const;

		template<typename UncompressedType>
		size_t compress(string & compressed, const UncompressedType & uncompressed) {
			return this->compress(&compressed[0], compressed.size(), uncompressed.getData(), uncompressed.size());
		}

		template<typename CompressedType, typename UncompressedType>
		size_t compress(CompressedType & compressed, const UncompressedType & uncompressed) {
			auto compressedSize = compressed.size();
			auto uncompressedSize = uncompressed.size();
			return this->compress(compressed.getData(), compressedSize, uncompressed.getData(), uncompressedSize);
		}

		SquashCodec * getSquashCodec() const;
	protected:
		string name;
		SquashCodec * squashCodec;
		bool valid = false;
	};
}