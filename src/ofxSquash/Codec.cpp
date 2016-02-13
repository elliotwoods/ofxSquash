#include "Codec.h"
#include "Constants.h"

#include "ofLog.h"
#include <iostream>

namespace ofxSquash {
#define IF_NOT_OK_RETURN(X) if (!this->isValid()) { \
	OFXSQUASH_ERROR << "Cannot call function. Codec [" << this->getName() << "] is not initialised"; \
	return X; \
}

	//----------
	Codec::Codec() {
		this->name = "uninitialized";
		this->valid = false;
	}

	//----------
	Codec::Codec(SquashCodec * squashCodec) {
		this->name = squash_codec_get_name(squashCodec);
		this->valid = squash_codec_init(squashCodec) == SQUASH_OK;
		this->squashCodec = squashCodec;
	}

	//----------
	Codec::Codec(string codecName) {
		this->name = codecName;
		this->squashCodec = squash_get_codec(codecName.c_str());
		if (this->squashCodec) {
			this->valid = true;
		}
		else {
			OFXSQUASH_WARNING << "Codec [" << this->name << "] cannot be initialized";
			this->valid = false;
		}
	}

	//----------
	bool Codec::isValid() const {
		return this->valid;
	}
	//----------
	const string & Codec::getName() const {
		return this->name;
	}

	//----------
	size_t Codec::getMaxCompressedSize(size_t uncompressedSize) const {
		IF_NOT_OK_RETURN(0);
		return squash_codec_get_max_compressed_size(this->squashCodec, uncompressedSize);
	}

	//----------
	size_t Codec::getUncompressedSize(void * compressed, size_t compressedSize) const {
		IF_NOT_OK_RETURN(0);
		return squash_codec_get_uncompressed_size(this->squashCodec, compressedSize, (uint8_t*) compressed);
	}

	//----------
	size_t Codec::getUncompressedSize(const string & compressed) const {
		IF_NOT_OK_RETURN(0);
		const auto compressedData = (uint8_t*) & compressed[0];
		auto size = squash_codec_get_uncompressed_size(this->squashCodec, compressed.size(), compressedData);
		if (size == 0) {
			OFXSQUASH_WARNING << "Codec [" << this->getName() << "] doesn't support getUncompressedSize (sorry! you'll need to allocate 'enough' yourself)";
		}
		return size;
	}

	//----------
	size_t Codec::compress(void * compressed, size_t compressedSize, const void * uncompressed, size_t uncompressedSize) const {
		IF_NOT_OK_RETURN(0);

		auto newCompressedSize = compressedSize;
		auto status = squash_codec_compress(this->squashCodec, &newCompressedSize, (uint8_t*) compressed, uncompressedSize, (uint8_t*) uncompressed, NULL);
		if (status != SQUASH_OK) {
			OFXSQUASH_WARNING << "[" << this->getName() << "] Compression failed : " << squash_status_to_string(status);
		}
		return newCompressedSize;
	}

	//----------
	void Codec::compress(string & compressed, const string & uncompressed) const {
		IF_NOT_OK_RETURN();

		compressed.resize(this->compress(& compressed[0], compressed.size(), uncompressed.data(), uncompressed.size()));
	}

	//----------
	string Codec::compress(const string & uncompressed) const {
		IF_NOT_OK_RETURN(string());

		string compressed;
		compressed.resize(this->getMaxCompressedSize(uncompressed.size()));
		this->compress(compressed, uncompressed);
		return compressed;
	}

	//----------
	size_t Codec::decompress(void * uncompressed, size_t uncompressedSize, const void * compressed, size_t compressedSize) const {
		IF_NOT_OK_RETURN(0);

		auto newUncompressedSize = uncompressedSize;
		auto status = squash_codec_decompress(this->squashCodec, &newUncompressedSize, (uint8_t*) uncompressed, compressedSize, (uint8_t*) compressed, NULL);
		if (status != SQUASH_OK) {
			OFXSQUASH_WARNING << "[" << this->getName() << "] Decompression failed : " << squash_status_to_string(status);
		}
		return newUncompressedSize;
	}

	//----------
	void Codec::decompress(string & uncompressed, const string & compressed) const {
		IF_NOT_OK_RETURN();

		const auto uncompressedData = (uint8_t*)(&uncompressed[0]);
		size_t uncompressedSize = static_cast<size_t>(uncompressed.size());
		const auto compressedData = (uint8_t*)(&compressed[0]);
		const size_t compressedSize = static_cast<size_t>(compressed.size());

		uncompressed.resize(this->decompress(uncompressedData, uncompressedSize, compressedData, compressedSize));
	}

	//----------
	string Codec::decompress(const string & compressed) const {
		IF_NOT_OK_RETURN(string());

		const auto uncomprsesedSize = this->getUncompressedSize(compressed);
		if (uncomprsesedSize == 0) {
			OFXSQUASH_WARNING << "decompress(const string & compressed) with this codec. Please use decompress(string & uncompressed, const string & compressed) and pre-allocate first.";
		}

		string uncompressed;
		uncompressed.resize(uncomprsesedSize);
		this->decompress(uncompressed, compressed);

		return uncompressed;
	}

	//----------
	SquashCodec * Codec::getSquashCodec() const {
		return this->squashCodec;
	}
}