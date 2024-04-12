#include "XorUnpack.hpp"
#include <string.h>

BitStreamsReader::BitStreamsReader(uint8_t *arr, uint8_t startOffset, uint8_t maxSize, uint16_t bSize) {
	blockSize = maxSize;
	bitMask = 0x80 >> startOffset;
	bytePtr = arr;
	zerosCount = 0;
	remainingBits = 0;
	bitSize = bSize;
	markBitInit();
}

bool BitStreamsReader::GetBit() {
	bool retVal;
	do {
		if (!blockSize) { // unpacked stream
			retVal = !!(pgm_read_byte(bytePtr) & bitMask);
			next();
			break;
		}
		else if (remainingBits) {
			retVal = !!(pgm_read_byte(bytePtr) & bitMask);
			next();
			if (!--remainingBits) {
				markBitInit();
			}
			break;
		}
		else if (zerosCount) {
			if (!--zerosCount) {
				markBitInit();
			}
			retVal = false;
			break;
		}
		else if (!remainingBits) {
			retVal = false;
			break;
		} else
			return -1; // Error
	} while (false);

	return retVal;
}

void BitStreamsReader::markBitInit() {
	if (bitSize && blockSize) {
		if (pgm_read_byte(bytePtr) & bitMask) {
			remainingBits = blockSize;
		}
		else {
			zerosCount = blockSize;
		}
		next();
	}
}

void BitStreamsReader::next() {
	if (bitSize) bitSize--;
	if (bitMask == 0x01) {
		bitMask = 0x80;
		bytePtr++;
	}
	else {
		bitMask >>= 1;
	}
};

BitXorBuffer::BitXorBuffer(uint8_t bufSize) {
	bufSize / 8 + !!(bufSize & 7);
	bitBuf = new uint8_t[bufSize];
	memset(bitBuf, 0, bufSize);
	this->bufSize = bufSize;
	// rewind(); // row starts by to
}

void BitXorBuffer::ReAlloc(uint8_t bufSize)
{
	bufSize = bufSize / 8 + !!(bufSize & 7);
	if (bufSize > this->bufSize) {
		delete[] bitBuf;
		bitBuf = new uint8_t[bufSize];
		memset(bitBuf, 0, bufSize);
		this->bufSize = bufSize;
	}
	memset(bitBuf, 0, bufSize);
	// rewind();
}

BitXorBuffer::~BitXorBuffer() {
	delete[] bitBuf;
}

void BitXorBuffer::rewind() {
	bitMask = 128;
	bytePtr = bitBuf;
}

uint8_t BitXorBuffer::xorMove(bool by) {
	if (bitMask & *bytePtr) by ^= true;
	*bytePtr |= bitMask;
	if (!by) {
		*bytePtr ^= bitMask;
	}
	nextBit();
	return -by; // false/true -> 0/0xFF
}
void BitXorBuffer::nextBit() {
	if (bitMask > 1) {
		bitMask >>= 1; // move bit in XOR buffer
	}
	else {
		bitMask = 128;
		bytePtr++;
	}
}
