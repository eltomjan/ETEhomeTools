#include <limits.h>
#include <avr/pgmspace.h>

class BitStreamsReader {
public:
	BitStreamsReader(uint8_t *arr, uint8_t startOffset, uint8_t maxSize, uint16_t bSize);
	bool GetBit();

private:
	void markBitInit();
	void next();

	uint8_t *bytePtr, bitMask, zerosCount, remainingBits, blockSize;
	uint16_t bitSize;
};

class BitXorBuffer {
public:
	BitXorBuffer(uint8_t bufSize);
	void ReAlloc(uint8_t bufSize);
	~BitXorBuffer();
	void rewind();
	uint8_t xorMove(bool by);

private:
	void nextBit();

	uint8_t *bytePtr, *bitBuf, bitMask;
	uint8_t bufSize;
};
