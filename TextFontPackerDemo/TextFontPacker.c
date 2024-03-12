#include <stdio.h>
#include "FileOperations.h"
#include "CharacterOperations.h"

int main(void)
{
	fontData font = { "FreeSansBold24pt7b.txt" };

	if (readFontFile(&font) && convertChars(&font))
	{
		FILE *dest;
		fopen_s(&dest, "webLog.htm", "wt");
		fprintf(dest, "<style>br{line-height: 10px;}div{float:left;height: 10px;width: 10px;}div#\\30{background-color: #bfb;}div#\\30 0e{background-color: #dfd;}div#\\31{background-color: #f00;}div#\\30 0{background-color: #bdb;}div#\\31 0{background-color: #f88;}div#\\30 00{background-color: #0f0;}</style>");
		fprintf(dest, "Char\n  Block\tOrig->Packed\tRatio\n<br>");
		unsigned long originalSize = 0;
		unsigned long packedSize = 0;
		for (unsigned char i = 0; i < 93; i++)
		{
			charBox *b = GetNthChar(i, &font);
			XorZeros(b);
			blockStats block = FindBestBlock(b);
			b->blockSize = block.blockSize;
			HtmlLog(b, dest);

			unsigned int bufBits = b->height * b->width;
			originalSize += bufBits;
			packedSize += block.bestBlockSize;
			fprintf(dest, "%c %u\t%u -> %u\t%.3f%%<br>\n", (char)(i + 33), (unsigned)b->blockSize,
				bufBits, block.bestBlockSize, 100.0 * block.bestBlockSize / bufBits);
		}
		fprintf(dest, "Sum \t%lu -> %lu\t%.3f%%\n", originalSize, packedSize, 100.0 * packedSize / originalSize);
		fclose(dest);
	}

	cleanUpFont(&font);

	return 0;
}
