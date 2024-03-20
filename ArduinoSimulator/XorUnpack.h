#include <stdint.h>
#include <avr/pgmspace.h>

class BitStreamsReader {
public:
	BitStreamsReader(uint8_t *arr, uint8_t startOffset, uint8_t maxSize, uint16_t bSize);
	operator bool();

private:
	void markBitInit();
	void next();

	uint8_t *bytePtr, bitMask, zerosCount, remainingBits, blockSize;
	uint16_t bitSize;
};

class BitXorBuffer {
public:
	BitXorBuffer(int bufSize);
	~BitXorBuffer();
	void rewind();
	bool xorMove(bool by);

private:
	void nextBit();

	uint8_t *bytePtr, *bitBuf, bitMask;
};
