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
		
		size_t getUncompressedSize(uint8_t * compressed, size_t compressedSize) const;
		size_t getUncompressedSize(const string & compressed) const;

		size_t compress(uint8_t * compresed, size_t compressedSize, uint8_t * uncompressed, size_t uncompressedSize) const;
		void compress(string & compressed, const string & uncompressed) const;
		string compress(const string & uncompressed) const;

		size_t decompress(uint8_t * uncompressed, size_t uncompressedSize, uint8_t * compressed, size_t compressedSize) const;
		void decompress(string & uncompressed, const string & compressed) const;
		string decompress(const string & compressed) const;

		template<typename UncompressedType>
		size_t compress(string & compressed, const UncompressedType & uncompressed) {
			return this->compress((uint8_t*)&compressed[0], compressed.size(), (uint8_t*)uncompressed.getData(), uncompressed.size());
		}

		template<typename CompressedType, typename UncompressedType>
		size_t compress(CompressedType & compressed, const UncompressedType & uncompressed) {
			auto compressedSize = compressed.size();
			auto uncompressedSize = uncompressed.size();
			return this->compress((uint8_t*)compressed.getData(), compressedSize, (uint8_t*)uncompressed.getData(), uncompressedSize);
		}
	protected:
		string name;
		SquashCodec * squashCodec;
		bool valid;
	};
}