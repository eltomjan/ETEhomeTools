// ConsolePlayground.cpp : Defines the entry point for the console application.
//
　
#include "stdafx.h"
#include <map>
#include <string>
using namespace std;
　
#define uchar_t              unsigned char
#define uint16_t             unsigned __int16
#define uint32_t             unsigned __int32
#define NBBY   CHAR_BIT
　
#include <sys/types.h>
　
#define MATCH_BITS 6
#define MATCH_MIN 3
#define MATCH_MAX ((1 << MATCH_BITS) + (MATCH_MIN - 1))
#define OFFSET_MASK ((1 << (2*NBBY - MATCH_BITS)) - 1)
　
size_t
compress(uchar_t *s_start, uchar_t *d_start, size_t s_len)
{
	uchar_t *src = s_start;
	uchar_t *dst = d_start;
	uchar_t *cpy, *copymap;
	int copymask = 1 << (NBBY - 1); // 1<<(8-1) = 128
	int mlen, offset;
	std::pair <std::multimap<uint16_t,uchar_t*>::iterator, std::multimap<uint16_t,uchar_t*>::iterator> hp;
	multimap<uint16_t,uchar_t*> lempel;
　
	int packBlocks = 0,maxMaxLen=0;
	int maxmaxlen=0,maxmaxoffset=0;
	while (src < (uchar_t *)s_start + s_len) { // end check
		if ((copymask <<= 1) == (1 << NBBY)) {
			if (dst >= (uchar_t *)d_start + s_len - 1 - 2 * NBBY) { // -1 -2*8 = -17 posledni bajty
				mlen = s_len;
				for (src = s_start, dst = d_start; mlen; mlen--)
					*dst++ = *src++;
				return (s_len); // nezkompresovano
			}
			copymask = 1;
			copymap = dst;
			*dst++ = 0;
		}
		if (src > (uchar_t *)s_start + s_len - MATCH_MAX) { // zbyva jen max delka, nekompresuj
			*dst++ = *src++;
			continue;
		}
		hp = lempel.equal_range(src[0] + (src[1]<<8));
		lempel.insert(make_pair(src[0] + (src[1]<<8), src));
		int maxLen=0,maxOffset;
		multimap<uint16_t,uchar_t*>::iterator maxIt;
		for (multimap<uint16_t,uchar_t*>::iterator it=hp.first; it!=hp.second; ++it) {
			if(it != lempel.end()) {
				offset = (intptr_t)(src - it->second) & OFFSET_MASK;
				if(!offset) offset++;
			} else {
				continue;
			}
			if(offset > OFFSET_MASK) { // too far
				lempel.erase(it);
				continue;
			}
			cpy = src - offset;
			if (cpy >= (uchar_t *)s_start && cpy != src &&
				src[0] == cpy[0] && src[1] == cpy[1] && src[2] == cpy[2]) {
					*copymap |= copymask;
					for (mlen = MATCH_MIN; mlen < MATCH_MAX; mlen++) {
						if (src[mlen] != cpy[mlen])
							break;
					}
				if(maxLen < mlen) {
					maxLen = mlen;
					maxIt = it;
					maxOffset = offset;
				}
			}
		}
		if(maxLen >= 3) {
			packBlocks++;
			printf("len %i off %i\t",maxLen,maxOffset,src-maxOffset);
			for(int i=0;i<maxLen;i++) printf(" %02x",*(src-maxOffset+i));
			printf("\n");
			*dst++ = ((maxLen - MATCH_MIN) << (NBBY - MATCH_BITS)) |
				(maxOffset >> NBBY);
			*dst++ = (uchar_t)maxOffset;
			src += maxLen;
			if(maxmaxlen<maxLen) maxmaxlen=maxLen;
			if(maxmaxoffset<maxOffset) maxmaxoffset=maxOffset;
		} else {
//			printf(" %02x",*(src));
			*dst++ = *src++;
		}
	}
	printf("%i len %i off %i", packBlocks, maxmaxlen, maxmaxoffset);
	return (dst - (uchar_t *)d_start);
}
　
size_t
decompress(uchar_t *s_start, uchar_t *d_start, size_t s_len, size_t d_len)
{
	uchar_t *src = s_start;
	uchar_t *dst = d_start;
	uchar_t *s_end = (uchar_t *)s_start + s_len;
	uchar_t *d_end = (uchar_t *)d_start + d_len;
	uchar_t *cpy, copymap;
	int copymask = 1 << (NBBY - 1);
　
	if (s_len >= d_len) {
		size_t d_rem = d_len;
		while (d_rem-- != 0)
			*dst++ = *src++;
		return (d_len);
	}
　
	while (src < s_end && dst < d_end) {
		if ((copymask <<= 1) == (1 << NBBY)) {
			copymask = 1;
			copymap = *src++;
		}
		if (copymap & copymask) {
			int mlen = (src[0] >> (NBBY - MATCH_BITS)) + MATCH_MIN;
			int offset = ((src[0] << NBBY) | src[1]) & OFFSET_MASK;
			src += 2;
			if ((cpy = dst - offset) >= (uchar_t *)d_start) {
				while (--mlen >= 0 && dst < d_end)
					*dst++ = *cpy++;
			} else
				/*
				* offset before start of destination buffer
				* indicates corrupt source data
				*/
				return (dst - (uchar_t *)d_start);
		} else {
			*dst++ = *src++;
		}
	}
	return (dst - (uchar_t *)d_start);
}
#define fWidth 7
#include "font.h"
#define chars 10
#define width 7
#define height 71
/*#define chars 94
#define width 3
#define height 36
/*#define chars 10
#define width 6
#define height 76*/
/*#define chars 10
#define width 5
#define height 59*/
　
int _tmain(int argc, _TCHAR* argv[])
{
	//print RAW chars
	if(0) {
	setFont(lucon);
	string out;
	for(int i=48;i<58;i++)
		out += printProportionalChar(i,0,0);
	return 0;
	}
	unsigned char *src;
	unsigned char big[] = {//glc
//#include "uncompressedSrc.txt"
#include "glc1.txt"
	};
  src = (unsigned char*)big;
  int size = sizeof(big);//10152;
	unsigned char destC[250000];
	unsigned char dest2[150000];
	int lng;
	int nums[256];
	memset(nums,0,sizeof(nums));
	unsigned char *charPos = src;
//	unsigned char *b = src;
	int less = 0;
	int ms=0,me=200;
/*	for(int i=0;i<chars;i++) {
		a=src + i*width*height;
		int s=0;
		while(!(*(a+s) || *(a+s+1) || *(a+s+2))) {
			s+=3;
		}
		memcpy(b,a+s,width*height);
//		less++;
		less -= s;
		if(s>ms) ms=s;
		int e=105-s;
		while(!(*(a+e) || *(a+e+1) || *(a+e+2))) {
			e-=3;
		}
		b+=e-s;
//		less++;
		if(!(*(a+e) || *(a+e+1) || *(a+e+2)))
			less -= (width*height-e);
		if(e<me) me=e;
	}*/
	string inp,xor;
	for(int i=0;i<height*chars;i++) {
		for(int j=0;j<width;j++) {
			for(int x=0;x<8;x++) inp += (src[i*width+j] & (128>>x))?"Ű":".";
		}
		inp += "\r\n";
	}
//#undef	chars
//#define	chars 2
	printf("%s\n",inp.c_str());
	for(int j=0;j<chars;j++) { // loop thru all chars
		for(int i=width*height*(j+1)-width-1;i>width*height*j+width-1;i--) { // from last byte xor with previous
			src[i] ^= src[i-width];
		}
	}
	for(int i=0;i<height*chars;i++) {
		for(int j=0;j<width;j++) {
			for(int x=0;x<8;x++) xor += (src[i*width+j] & (128>>x))?"Ű":" ";
		}
		xor += "\r\n";
		if(!((i+1)%height)) {
	printf("%s\n",xor.c_str());
	xor = "";
		}
	}
/*	for(int j=0;j<chars;j++) {
		for(int i=width*height*j+width;i<width*height*(j+1)-width;i++)
			src[i] ^= src[i-width];
	}
*/	for(int i=0;i<height*chars;i++) {
		for(int j=0;j<width;j++) {
			for(int x=0;x<8;x++) xor += (src[i*width+j] & (128>>x))?"Ű":".";
		}
		xor += "\r\n";
		if(!((i+1)%height)) {
	printf("%s\n",xor.c_str());
//	xor = "";
		}
	}
//	printf("%s",xor.c_str());
	int r=0;
	string radkyBM,sloupceBM,binRadek;
	int binHodnota = 1;
	const char hex[]="0123456789ABCDEF";
	int binSloupec = 1,binSloupecNo=0,sloupceNo=0;
	string nzData;
	for(int i=0;i<chars;i++) {
		charPos=src + i*width*height;
		for(int radekRel=0;radekRel<width*height;radekRel+=width) { // radek
			binHodnota <<= 1;
			bool zeros = true;
			for(int sloupec=0;sloupec<width;sloupec++) zeros &= !*(charPos+radekRel+sloupec);
			if(zeros) { // whole row of zeros
				r++;
				radkyBM+="0"; // mark it
			} else {
				binHodnota++;
				radkyBM+="1"; // mark non-zero row
				sloupceNo++;
				for(int sloupec=0;sloupec<width;sloupec++) {
					binSloupec <<= 1;
					if(*(charPos+radekRel+sloupec)) {
						binSloupec++; // set current bit
						nzData += "0x";
						nzData += hex[*(charPos+radekRel+sloupec)>>4];
						nzData += hex[*(charPos+radekRel+sloupec)&15];
						nzData += ",";
					}
					if(binSloupec & 256) {
						binSloupec &= 255;
						sloupceBM += "0x";
						sloupceBM += hex[binSloupec>>4];
						sloupceBM += hex[binSloupec&15];
						sloupceBM += ',';
						binSloupec = 1;
						binSloupecNo++;
					}
				}
			}
			if(binHodnota & 256) {
				binHodnota &= 255;
				binRadek += "0x";
				binRadek += hex[binHodnota>>4];
				binRadek += hex[binHodnota&15];
				binRadek += ',';
				binHodnota = 1; //binRadek.length()<20?1:16;
			}
		}
		radkyBM = radkyBM.substr(0,radkyBM.length() - height) + binRadek + " // " + radkyBM.substr(radkyBM.length() - height);
		printf("%s %0x\n", binRadek.c_str(), binHodnota);
		binRadek = "";
		radkyBM+="\n";
	}
	if(binSloupec > 1) {
		int mask = 128;
		while(!(binSloupec & mask)) mask >>= 1;
		binSloupec ^= mask;
		sloupceBM += "0x";
		sloupceBM += hex[(binSloupec>>4)&15];
		sloupceBM += hex[binSloupec&15];
		sloupceBM += ",";
		binSloupecNo++;
	}
	binRadek = "//RowBMP height*chars = 3384/8 = 423 - starts with CollBM size H,L\r\n0x";
	binRadek += hex[binSloupecNo>>12]; // non-zero data size
	binRadek += hex[(binSloupecNo>>8)&15];
	binRadek += ", 0x";
	binRadek += hex[(binSloupecNo>>4)&15];
	binRadek += hex[binSloupecNo&15];
	binRadek += ", 0x";
	binRadek += hex[(chars>>4)&15]; // no of chars
	binRadek += hex[chars&15];
	binRadek += ", 0x";
	binRadek += hex[(width>>4)&15]; // chars width
	binRadek += hex[width&15];
	binRadek += ", 0x";
	binRadek += hex[(height>>4)&15]; // chars height
	binRadek += hex[height&15];
	binRadek += ",\r\n";
	binRadek += radkyBM; // non-zero rows bit array
	binRadek += "//CollBM\r\n";
	binRadek += sloupceBM; // non-zero columns bit array
	binRadek += "\r\n//Non-zero font data\r\n";
	binRadek += nzData; // remaining (non-zero) bitmap data
	size+=less;
	for(int i=0;i<size;i++) nums[src[i]]++;
	lng = compress(src,destC,size);
	string s;
	for(int i=0;i<lng;i++) {
		s+="0x";
		s += hex[destC[i]>>4];
		s += hex[destC[i]&15];
		s+=", ";
	}
	decompress(destC,dest2,lng,size);
	for(int i=0;i<size;i++)
		if(src[i] == dest2[i]);
		else
			break;
}
