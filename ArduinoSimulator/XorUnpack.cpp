#include "XorUnpack.h"
#include <cstring>

BitStreamsReader::BitStreamsReader(uint8_t *arr, uint8_t startOffset, uint8_t maxSize, uint16_t bSize) {
	blockSize = maxSize;
	bitMask = 0x80 >> startOffset;
	bytePtr = arr;
	zerosCount = 0;
	remainingBits = 0;
	bitSize = bSize;
	markBitInit();
}

BitStreamsReader::operator bool() {
	if (!blockSize) { // unpacked stream
		bool retVal = !!(pgm_read_byte(bytePtr) & bitMask);
		next();
		return retVal;
	}
	else if (remainingBits) {
		bool retVal = !!(pgm_read_byte(bytePtr) & bitMask);
		next();
		if (!--remainingBits) {
			markBitInit();
		}
		return retVal;
	}
	else if (zerosCount) {
		if (!--zerosCount) {
			markBitInit();
		}
		return 0;
	}
	else if (!remainingBits)
		return false;
	throw "??";
	return 0; //throw "??";
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

BitXorBuffer::BitXorBuffer(int bufSize) {
	bufSize = bufSize / 8 + !!(bufSize & 7);
	bitBuf = new uint8_t[bufSize];
	memset(bitBuf, 0, bufSize);
	rewind();
}

BitXorBuffer::~BitXorBuffer() {
	delete[] bitBuf;
}

void BitXorBuffer::rewind() {
	bitMask = 128;
	bytePtr = bitBuf;
}

bool BitXorBuffer::xorMove(bool by) {
	if (bitMask & *bytePtr) by ^= true;
	*bytePtr |= bitMask;
	if (!by) {
		*bytePtr ^= bitMask;
	}
	nextBit();
	return by;
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
