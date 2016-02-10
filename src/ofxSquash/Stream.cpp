#include "Stream.h"

#include "Constants.h"

namespace ofxSquash {
	//----------
	Stream::Stream(Codec codec, Direction direction, WriteFunction writeFunction, size_t bufferSize)
		: writeFunction(writeFunction)
		, bufferSize(bufferSize)
		, buffer(codec.getMaxCompressedSize(bufferSize))
	{
		auto squashDirection = direction == Direction::Compress ? SQUASH_STREAM_COMPRESS : SQUASH_STREAM_DECOMPRESS;

		if (!codec.isValid()) {
			OFXSQUASH_ERROR << "Please specify a valid codec when initializing a stream";
		}
		else {
			this->codec = codec;
			this->squashStream = squash_stream_new(codec.getSquashCodec(), squashDirection, nullptr);
			if (!squashStream) {
				OFXSQUASH_ERROR << "Failed to initialize stream for codec [" << codec.getName() << "]";
			}
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
			OFXSQUASH_ERROR << "Cannot read because Stream is expired.";
			return;
		}

		const uint8_t* data = (const uint8_t*) dataRaw;
		int64_t incomingBytesRemaining = size;

		SquashStatus status;

		while (incomingBytesRemaining > 0)
		{
			size_t input_size = std::min<size_t>(this->bufferSize, incomingBytesRemaining);

			this->squashStream->next_in = data;
			this->squashStream->avail_in = input_size;
			this->squashStream->next_out = this->buffer.data();
			this->squashStream->avail_out = this->buffer.size();

			do
			{
				status = squash_stream_process(this->squashStream);

				if (status != SQUASH_OK && status != SQUASH_PROCESSING)
				{
					OFXSQUASH_ERROR << "Processing stream failed: " << squash_status_to_string(status);
					return;
				}

			} while (status == SQUASH_PROCESSING);

			const size_t outputSize = this->buffer.size() - this->squashStream->avail_out;
			if (outputSize > 0)
			{
				if (this->writeFunction) {
					this->writeFunction(this->buffer.data(), outputSize);
				}
				else {
					OFXSQUASH_WARNING << "Cannot write stream output. No WriteFunction has been set";
				}
			}

			incomingBytesRemaining -= this->bufferSize;
			data += this->bufferSize;
		}
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
			this->squashStream->avail_out = this->bufferSize;

			status = squash_stream_finish(this->squashStream);

			if (status != SQUASH_OK && status != SQUASH_PROCESSING) {
				OFXSQUASH_ERROR << "Processing failed: " << squash_status_to_string(status);
				return * this;
			}

			const size_t outputSize = this->bufferSize - this->squashStream->avail_out;
			if (outputSize > 0)
			{
				if (this->writeFunction) {
					this->writeFunction(this->buffer.data(), outputSize);
				}
				else {
					OFXSQUASH_WARNING << "Cannot write stream output. No WriteFunction has been set";
				}
			}

		} while (status == SQUASH_PROCESSING);

		squash_object_unref(this->squashStream);
		this->squashStream = nullptr;

		return * this;
	}
}