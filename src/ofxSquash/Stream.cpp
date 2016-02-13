#include "Stream.h"

#include "Constants.h"

namespace ofxSquash {
	//----------
	Stream::Stream(Codec codec, Direction direction, WriteFunction writeFunction, size_t bufferSize)
		: codec(codec)
		, direction(direction)
		, writeFunction(writeFunction)
		, buffer(bufferSize)
	{
		if (!codec.isValid()) {
			OFXSQUASH_ERROR << "Please specify a valid codec when initializing a stream";
		}
		else {
			this->codec = codec;
		}
	}

	//----------
	Stream::~Stream() {
		if (this->squashStream) {
			squash_object_unref(this->squashStream);
		}
		this->squashStream = NULL;
	}

	//----------
	void Stream::setWriteFunction(const WriteFunction & writeFunction) {
		this->writeFunction = writeFunction;
	}

	//----------
	const WriteFunction & Stream::getWriteFunction() const {
		return this->writeFunction;
	}

	//----------
	void Stream::read(const void * dataRaw, size_t size) {
		if (!this->squashStream) {
			if (!this->codec.isValid()) {
				OFXSQUASH_ERROR << "Cannot read stream. Codec is not initialized";
				return;
			}

			auto squashDirection = direction == Direction::Compress ? SQUASH_STREAM_COMPRESS : SQUASH_STREAM_DECOMPRESS;

			this->squashStream = squash_stream_new(this->codec.getSquashCodec(), squashDirection, nullptr);
			if (!squashStream) {
				OFXSQUASH_ERROR << "Failed to initialize stream for codec [" << codec.getName() << "]";
			}
		}

		const uint8_t* data = (const uint8_t*) dataRaw;
		int64_t incomingBytesRemaining = size;

		SquashStatus status;

		this->squashStream->next_in = data;
		this->squashStream->avail_in = size;

		do
		{
			this->squashStream->next_out = this->buffer.data();
			this->squashStream->avail_out = this->buffer.size();

			status = squash_stream_process(this->squashStream);

			if (status != SQUASH_OK && status != SQUASH_PROCESSING)
			{
				OFXSQUASH_ERROR << "Processing stream failed: " << squash_status_to_string(status);
				return;
			}

			const size_t outputSize = this->buffer.size() - this->squashStream->avail_out;
			if (outputSize > 0)
			{
				if (this->writeFunction) {
					WriteFunctionArguments args{ this->buffer.data(), outputSize, false };
					this->writeFunction(args);
				}
				else {
					OFXSQUASH_WARNING << "Cannot write stream output. No WriteFunction has been set";
				}
			}
		} while (status == SQUASH_PROCESSING);
	}

	//----------
	Stream & Stream::operator<<(const string & data) {
		this->read(data.data(), data.size());
		return *this;
	}

	//----------
	Stream & Stream::operator<<(const Finish &) {
		if (this->squashStream == nullptr)
		{
			OFXSQUASH_WARNING << "Stream already finished. You need to make a new one.";
			return* this;
		}

		SquashStatus status;

		do {
			this->squashStream->next_out = this->buffer.data();
			this->squashStream->avail_out = this->buffer.size();

			status = squash_stream_finish(this->squashStream);

			if (status != SQUASH_OK && status != SQUASH_PROCESSING) {
				OFXSQUASH_ERROR << "Processing failed: " << squash_status_to_string(status);
				return * this;
			}

			const size_t outputSize = this->buffer.size() - this->squashStream->avail_out;
			if (outputSize > 0)
			{
				if (this->writeFunction) {
					WriteFunctionArguments args{ this->buffer.data(), outputSize, false };
					this->writeFunction(args);
				}
				else {
					OFXSQUASH_WARNING << "Cannot write stream output. No WriteFunction has been set";
				}
			}
		} while (status == SQUASH_PROCESSING);

		if (this->writeFunction) {
			WriteFunctionArguments args{ nullptr, 0, true };
			this->writeFunction(args);
		}

		squash_object_unref(this->squashStream);
		this->squashStream = nullptr;

		return * this;
	}
}