#pragma once

#include "Codec.h"
#include "Constants.h"

namespace ofxSquash {
	struct WriteFunctionArguments {
		const uint8_t * data;
		size_t size;
		bool isFinished;
	};

	typedef function<void(const WriteFunctionArguments &)> WriteFunction;

	class Stream {
	public:
		struct Finish {};

		Stream(Codec codec, Direction direction, WriteFunction writeFunction = WriteFunction(), size_t bufferSize = 8 * 1024);
		virtual ~Stream();

		void setWriteFunction(const WriteFunction &);
		const WriteFunction & getWriteFunction() const;

		void read(const void * data, size_t size);

		Stream & operator<<(const string &);
		Stream & operator<<(const Finish &);
	protected:
		Codec codec;
		Direction direction;
		SquashStream * squashStream = nullptr;
		WriteFunction writeFunction;

		size_t bufferSize;
		vector<uint8_t> buffer;
	};


}