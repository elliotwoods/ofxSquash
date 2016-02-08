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
			OFXSQUASH_WARNING << "Codec [" << this->name << "] cannot be initialised";
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
	size_t Codec::getUncompressedSize(uint8_t * compressed, size_t compressedSize) const {
		IF_NOT_OK_RETURN(0);
		return squash_codec_get_uncompressed_size(this->squashCodec, compressedSize, compressed);
	}

	//----------
	size_t Codec::getUncompressedSize(const string & compressed) const {
		IF_NOT_OK_RETURN(0);
		const auto compressedData = (uint8_t*) & compressed[0];
		auto size = squash_codec_get_uncompressed_size(this->squashCodec, compressed.size(), compressedData);
		if (size == 0) {
			OFXSQUASH_WARNING << "Codec [" << this->getName() << "] doesn't support getUncompressedSize (sorry! you'll need to just allocate 'enough' yourself)";
		}
	}

	//----------
	size_t Codec::compress(uint8_t * compressed, size_t compressedSize, uint8_t * uncompressed, uint8_t uncompressedSize) const {
		IF_NOT_OK_RETURN(0);

		auto newCompressedSize = compressedSize;
		if (SQUASH_OK != squash_codec_compress(this->squashCodec, &newCompressedSize, compressed, uncompressedSize, uncompressed, NULL)) {
			OFXSQUASH_WARNING << "Compression failed";
		}
		return newCompressedSize;
	}

	//----------
	void Codec::compress(string & compressed, const string & uncompressed) const {
		IF_NOT_OK_RETURN();

		const auto compressedData = (uint8_t*) (&compressed[0]);
		size_t compressedSize = static_cast<size_t>(compressed.size());
		const auto uncompressedData = (uint8_t*) (&uncompressed[0]);
		const size_t uncompressedSize = static_cast<size_t>(uncompressed.size());
		
		auto status = squash_codec_compress(this->squashCodec, &compressedSize, compressedData, uncompressedSize, uncompressedData, NULL);
		if(status != SQUASH_OK) {
			OFXSQUASH_WARNING << "Compression failed : " << squash_status_to_string(status);
		}
		else {
			compressed.resize(compressedSize);
		}
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
	void Codec::uncompress(string & uncompressed, const string & compressed) const {
		IF_NOT_OK_RETURN();

		const auto uncompressedData = (uint8_t*)(&uncompressed[0]);
		size_t uncompressedSize = static_cast<size_t>(uncompressed.size());
		const auto compressedData = (uint8_t*)(&compressed[0]);
		const size_t compressedSize = static_cast<size_t>(compressed.size());

		auto status = squash_codec_decompress(this->squashCodec, &uncompressedSize, uncompressedData, compressedSize, compressedData, NULL);
		if (status != SQUASH_OK) {
			OFXSQUASH_WARNING << "Decompression failed : " << squash_status_to_string(status);
		}
		else {
			uncompressed.resize(uncompressedSize);
		}		
	}

	//----------
	string Codec::uncompress(const string & compressed) const {
		IF_NOT_OK_RETURN(string());

		const auto uncomprsesedSize = this->getUncompressedSize(compressed);
		if (uncomprsesedSize == 0) {
			OFXSQUASH_WARNING << "uncompress(const string & compressed) with this codec. Please use uncompress(string & uncompressed, const string & compressed) and pre-allocate first.";
		}

		string uncompressed;
		uncompressed.resize(uncomprsesedSize);
		this->uncompress(uncompressed, compressed);

		return uncompressed;
	}
}