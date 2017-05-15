#define TestingCstring
#ifndef CSTRING
#define CSTRING
#include <string.h>
#include <iostream>
#ifdef WIN32
#pragma warning (disable : 4996)
#endif

using namespace std;

template<unsigned short int c_maxSize>
class cString;
template<unsigned short int c_maxSize>
class cStringU {
public:
	cStringU() {
		m_size = 0; *m_data = 0;
	}
	template<unsigned short int c_maxSizeU>
	cStringU (const cString<c_maxSizeU>& src) {
		unsigned char *srcPtr = (unsigned char*)src.c_str();//(unsigned char *)(((char*)&src) + 2);
		short int maxSize = c_maxSize;
		if(*srcPtr) encodeUtf((unsigned char *)m_data, srcPtr, maxSize); else *m_data=0;
		if(maxSize <= 0) {
			*(m_data + c_maxSize - 1) = 0;
			if(maxSize < -1) throw "cString Internal error";
			maxSize = 0;
		}
		m_size = c_maxSize - maxSize;
	}
	operator char*() {
		return m_data;
	}
	unsigned short int getMaxSize() { return c_maxSize; }
private:
	unsigned short int m_size : 16;
	char m_data[c_maxSize];
    void encodeUtf(unsigned char *dest, unsigned char *src, short int &maxSize) {
	const static unsigned char utf2w[] = {
  //0x80,    ,0x82,    ,0x84,0x85,0x86,0x87,    ,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,    ,0x91,0x92,0x93,0x94,0x95,0x96,0x97,    ,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f
  //     0x81      0x83                     0x88                                    0x90                                    0x98 dle Win / Notepad (ANSI->UTF)
	0xac,0x81,0x9a,0x83,0x9e,0xa6,0xa0,0xa1,0x88,0xb0,0xa0,0xb9,0x9a,0xa4,0xbd,0xb9,0x90,0x98,0x99,0x9c,0x9d,0xa2,0x93,0x94,0x98,0xa2,0xa1,0xba,0x9b,0xa5,0xbe,0xba,
	0xa0,0x87,0x98,0x81,0xa4,0x84,0xa6,0xa7,0xa8,0xa9,0x9e,0xab,0xac,0xad,0xae,0xbb,0xb0,0xb1,0x9b,0x82,0xb4,0xb5,0xb6,0xb7,0xb8,0x85,0x9f,0xbb,0xbd,0x9d,0xbe,0xbc,
	0x94,0x81,0x82,0x82,0x84,0xb9,0x86,0x87,0x8c,0x89,0x98,0x8b,0x9a,0x8d,0x8e,0x8e,0x90,0x83,0x87,0x93,0x94,0x90,0x96,0x97,0x98,0xae,0x9a,0xb0,0x9c,0x9d,0xa2,0x9f,
	0x95,0xa1,0xa2,0x83,0xa4,0xba,0x87,0xa7,0x8d,0xa9,0x99,0xab,0x9b,0xad,0xae,0x8f,0x91,0x84,0x88,0xb3,0xb4,0x91,0xb6,0xb7,0x99,0xaf,0xba,0xb1,0xbc,0xbd,0xa3,0x99,

  //     0xC2      0xC2                     0xC2                                    0xC2                                    0xC2 dle Win / Notepad (ANSI->UTF)
	0xe2,0xC2,0xe2,0xC2,0xe2,0xe2,0xe2,0xe2,0xC2,0xe2,0xc5,0xe2,0xc5,0xc5,0xc5,0xc5,0xC2,0xe2,0xe2,0xe2,0xe2,0xe2,0xe2,0xe2,0xC2,0xe2,0xc5,0xe2,0xc5,0xc5,0xc5,0xc5,
	0xc2,0xcb,0xcb,0xc5,0xc2,0xc4,0xc2,0xc2,0xc2,0xc2,0xc5,0xc2,0xc2,0xc2,0xc2,0xc5,0xc2,0xc2,0xcb,0xc5,0xc2,0xc2,0xc2,0xc2,0xc2,0xc4,0xc5,0xc2,0xc4,0xcb,0xc4,0xc5,
	0xc5,0xc3,0xc3,0xc4,0xc3,0xc4,0xc4,0xc3,0xc4,0xc3,0xc4,0xc3,0xc4,0xc3,0xc3,0xc4,0xc4,0xc5,0xc5,0xc3,0xc3,0xc5,0xc3,0xc3,0xc5,0xc5,0xc3,0xc5,0xc3,0xc3,0xc5,0xc3,
	0xc5,0xc3,0xc3,0xc4,0xc3,0xc4,0xc4,0xc3,0xc4,0xc3,0xc4,0xc3,0xc4,0xc3,0xc3,0xc4,0xc4,0xc5,0xc5,0xc3,0xc3,0xc5,0xc3,0xc3,0xc5,0xc5,0xc3,0xc5,0xc3,0xc3,0xc5,0xcb,

	0x82,  0 ,0x80,  0 ,0x80,0x80,0x80,0x80,  0 ,0x80,  0 ,0x80,  0 ,  0 ,  0 ,  0 ,  0 ,0x80,0x80,0x80,0x80,0x80,0x80,0x80,  0 ,0x84,  0 ,0x80
	};
		while(*src && maxSize--) {
			if(*src >= 128) {
				if((*dest = utf2w[*src]) == (unsigned char)0xE2) { // 3B
					if(maxSize > 2) {
						if(*src < 156) { // delka tabulky 2. byte 3-bytove sekvence
							*++dest = utf2w[*src + 128]; // 2. - 3. blok tabulky
							*++dest = utf2w[*src - 128]; // 3. - 1. blok tabulky
							dest++; maxSize -= 2;
						} else *dest++ = '?';
					} else break; // nevesel se
				} else {
					if(maxSize > 1) {
						if(*dest && maxSize--) { // 2B - 1. z 2. bloku tabulky
							dest++;
							*dest++ = utf2w[*src - 128]; // 2. z 1. blok tabulky
						}
					} else {
						*dest = 0;
						maxSize++;
						return;
					}
				}
			} else *dest++ = *src;
			src++;
		}
		if(maxSize >= 0) *dest++ = 0;
	}
};
template<unsigned short int c_maxSize>
class cString {
public:
	cString () {
		*m_data = 0;
//		m_size = c_maxSize;
//		char *ptr = m_data;
//		while(m_size--) *ptr++ = (a=a>'8'?'0':a+1); // inicializace
		*m_data = 0; // neinicializovany string by se nemel pouzivat, takze maly chytak bude v nem - 1234567890...
		m_size = 0;
	}
	cString (const cString<c_maxSize>& src) { // copy constructor
		m_size = src.length();
		memcpy(m_data, src.m_data, m_size + 1);
	}
	cString (const char* src) {
		m_size = 0;
		operator =(src);
	}
	cString (const char src) {
		m_size = 1;
		*m_data = src;
		*(m_data+1) = 0;
	}
	cString& operator =(const char* src) {
		char *ptr = m_data;
		unsigned short int maxSize = c_maxSize;
		if(src && m_size && !*src) {
			m_size = 0;
			*m_data = 0;
			return *this;
		}
		if(src && *src) {
			m_size = 0;
			while((maxSize-- > 0) && (*ptr++ = *src++)) m_size++;
		}
		if(src) *(m_data + m_size) = 0;
		return *this;
	}
	cString fromUtf(const char *src) {
		char *ptr = m_data;
		unsigned char *srcPtr = (unsigned char *)src;
		short int maxSize = c_maxSize;
		if(*src) while((*ptr++ = decodeUtf(&srcPtr)) && (maxSize-- > 0)); else *ptr=0;
		if(maxSize <= 0) {
			*(m_data + c_maxSize + maxSize) = 0;
			if(maxSize < -1) throw "cString Internal error";
			maxSize = 0;
		} else *ptr = 0;
		m_size = c_maxSize - maxSize;
		return *this;
	}
	cString& operator +=(const char *src) { // "strcat"
		char *ptr = m_data + m_size;
		int maxSize = c_maxSize - m_size, maxSizeMem = maxSize;
		if(maxSize < 1) return *this;
		if(!src) return *this;
		while(((maxSize--) > 0) && (*ptr++ = *src++));
		if(maxSize < -1) throw "cString Internal error";
		if(maxSize <= 0) {
			if(maxSize < 0) *(m_data + c_maxSize) = 0;
			if(maxSize == 0 && !*(ptr-1)) maxSize = 1;
			else maxSize = 0;
		} else {
			*(m_data + maxSizeMem - maxSize + m_size - 1) = 0;
			maxSize++;
		}
		m_size = c_maxSize - maxSize;
		return *this;
	}
	const cString& operator +=(char src) { // "strcat"
		char *ptr = m_data + m_size;
		int maxSize = c_maxSize - m_size;
		if(maxSize < 1) return *this;
		*ptr++ = src;
		m_size++;
		*ptr = 0;
		return *this;
	}
	cString<21> num2str(long long number, int maxSize) {
		// number size ns >= log 2^bits
		// ns >= bits * log 2
		bool negative = number < 0;
		if(negative) {
			number = -number;
		}
		int i=0;
		cString<21> asciiNum = "                     ";
		asciiNum[maxSize+1] = 0;
		while(number && i < maxSize) {
			i++;
			asciiNum[maxSize-i+1] = (number % 10) + '0';
			number /= 10;
		}
		if(maxSize - negative >= i) {
			if(negative)
				asciiNum[maxSize-i++] = '-';
			if(maxSize-i+1)
				asciiNum = asciiNum.substr(maxSize-i+1);
			return asciiNum;
		}
		return "";
	}
	const cString& operator +=(int number) {
		cString<21> &retVal = num2str(number, 11);
		if(c_maxSize - m_size >= retVal.length() && retVal.length())
			operator += (retVal);
		return *this;
	}
	const cString& operator +=(long number) {
		cString<21> &retVal = num2str(number, 11);
		if(c_maxSize - m_size >= retVal.length() && retVal.length())
			operator += (retVal);
		return *this;
	}
	const cString& operator +=(long long number) {
		cString<21> &retVal = num2str(number, 20);
		if(c_maxSize - m_size >= retVal.length() && retVal.length())
			operator += (retVal);
		return *this;
	}
	template<unsigned short int T>
	cString& operator +=(const cString<T>& src) { // "strcat"
		unsigned short int remainingSize = c_maxSize - m_size;
		unsigned short int srcSize = src.length();
        const char *srcData = (char*)&src + (unsigned long long)&((cString*)0)->m_data;
		if(srcSize > remainingSize) {
			memcpy(m_data + m_size, srcData, remainingSize);
			m_size += remainingSize;
			*(m_data + m_size) = 0;
		} else
			if(srcSize > 0) {
				strcpy(m_data + m_size, srcData);
				m_size += srcSize;
			}
		return *this;
	}
	template<unsigned short int T>
	cString& operator =(const cString<T>& src) {
		char *ptr = m_data, *source = (char*)&src;
		source += (unsigned long long)&((cString*)0)->m_data;
		if(c_maxSize < src.length()) {
			m_size = c_maxSize;
			memcpy(m_data, source, m_size);
			*(m_data + m_size) = 0;
		} else {
			strcpy(ptr, source);
			m_size = src.length();
		}
		return *this;
	}
	bool operator  <(cString& nd) const { return strcmp(m_data, nd.m_data) < 0; }
	bool operator  >(cString& nd) const { return strcmp(m_data, nd.m_data) > 0; }
	bool operator <=(cString& nd) const { return strcmp(m_data, nd.m_data) <= 0; }
	bool operator >=(cString& nd) const { return strcmp(m_data, nd.m_data) >= 0; }
	bool operator ==(cString& nd) const {
		if(m_size != nd.m_size) return false;
		return !strcmp(m_data, nd.m_data);
	}
	bool operator !=(cString& nd) const {
		if(m_size != nd.m_size) return true;
		return (0 != strcmp(m_data, nd.m_data));
	}
	bool operator !=(const char* nd) const {
		if(!nd) return true;
		return (0 != strcmp(m_data, nd));
	}
	bool operator <(char* nd) const { return strcmp(m_data, nd) < 0; }
	bool operator >(char* nd) const { return strcmp(m_data, nd) > 0; }
	bool operator ==(const char* nd) const { return !strcmp(m_data, nd); }
	inline operator const char*() const { return (const char*)m_data; }
	inline const char *c_str() const { return const_cast<char*>(m_data); }
	inline unsigned short int max_size() const { return c_maxSize; }
	char& operator [] (const int pos) const {
		if (pos > m_size)
			throw "Index out of string.";

		return const_cast<char&>(m_data[pos]); // možno i pøepsat
	}
	size_t find(int chr, size_t pos = 0) {
		char* ptr;

		if(m_size <= pos)
			return string::npos;
		ptr = strchr(m_data + pos, chr);
		if (!ptr)
			return string::npos;

		return ptr - m_data;
	}
	inline size_t find(const char *chr, size_t pos = 0) {
		char* ptr;

		if(!chr || m_size <= pos)
			return string::npos;
		ptr = strstr(m_data + pos, chr);
		if (!ptr)
			return string::npos;

		return ptr - m_data;
	}
	template<unsigned short int T>
	size_t find(const cString<T>& str, size_t pos = 0) {
		char* ptr;

		if(m_size < pos || (m_size - pos) < str.length())
			return string::npos;
		ptr = strstr(m_data + pos, str.c_str());
		if (!ptr)
			return string::npos;

		return ptr - m_data;
	}
	cString substr(unsigned short int pos = 0, unsigned short int len = -1) const {
		cString<c_maxSize> res;
		if(pos > m_size) return res;
		if(pos+len < c_maxSize) {
			memcpy(res.m_data, m_data + pos, len);
			*(res.m_data+len) = 0;
			res = res.m_data;
		} else res = (m_data + pos);
		return res;
	}
	~cString() {
	}
	cString upperCase() {
		static const unsigned char c_upperCase[] = {
		// áÁèÈïÏéÉìÌíÍòÒóÓøØšŠúÚùÙýÝžŽ 0xe1,0xc1,0xe8,0xc8,0xef,0xcf,0xe9,0xc9,0xec,0xcc,0xed,0xcd,0xf2,0xd2,0xf3,0xd3,0xf8,0xd8,0x9a,0x8a,0x9d,0x8d,0xfa,0xda,0xf9,0xd9,0xfd,0xdd,0x9e,0x8e
		0xe1u,0xe8u,0xefu,0xe9u,0xecu,0xedu,0xf2u,0xf3u,0xf8u,0x9au,0x9du,0xfau,0xf9u,0xfdu,0x9eu, 0,
		0xc1u,0xc8u,0xcfu,0xc9u,0xccu,0xcdu,0xd2u,0xd3u,0xd8u,0x8au,0x8du,0xdau,0xd9u,0xddu,0x8eu, 0};
		char *pos;
		for(int i=0;i<m_size;i++) {
			if(m_data[i] >= 'a' && m_data[i] <= 'z')
				m_data[i] -= 32;
			else if((unsigned char)m_data[i] > 127) {
				pos = strchr((char*)&c_upperCase[0], m_data[i]);
				if(pos)
					m_data[i] = *(pos + sizeof(c_upperCase)/2);
			}
		}
		return *this;
	}

	unsigned short int getMaxSize() const { return c_maxSize; }
	unsigned short int length() const { return m_size; }
protected:
	unsigned short int m_size : 16;
	char m_data[c_maxSize+1];
private:
	char decodeUtf(unsigned char **src) {
		unsigned char znak = **src;
		if((znak < 0xC2) || ((znak > 0xC5) && (znak != 0xC8) && (znak != 0xCB) && (znak != 0xE2))) {
			(*src)++; // no prefix => nothing to encode - no need to search in table
			return znak;
		}
		static unsigned char w2utf[] = { // prefix length,suffix byte #, utf prefix, utf suffixes, win1250 ANSI bytes, ..., 0
		 1,18,0xC2,0xA0,0xA4,0xA6,0xA7,0xA8,0xA9,0xAB,0xAC,0xAD,0xAE,0xB0,0xB1,0xB4,0xB5,0xB6,0xB7,0xB8,0xBB,
				   0xA0,0xA4,0xA6,0xA7,0xA8,0xA9,0xAB,0xAC,0xAD,0xAE,0xB0,0xB1,0xB4,0xB5,0xB6,0xB7,0xB8,0xBB,
		 1,31,0xC3,0x81,0x82,0x84,0x87,0x89,0x8B,0x8D,0x8E,0x93,0x94,0x96,0x97,0x9A,0x9C,0x9D,0x9F,0xA1,0xA2,0xA4,0xA7,0xA9,0xAB,0xAD,0xAE,0xB3,0xB4,0xB6,0xB7,0xBA,0xBC,0xBD,
				   0xC1,0xC2,0xC4,0xC7,0xC9,0xCB,0xCD,0xCE,0xD3,0xD4,0xD6,0xD7,0xDA,0xDC,0xDD,0xDF,0xE1,0xE2,0xE4,0xE7,0xE9,0xEB,0xED,0xEE,0xF3,0xF4,0xF6,0xF7,0xFA,0xFC,0xFD,
		 1,20,0xC4,0x82,0x83,0x84,0x85,0x86,0x87,0x8C,0x8D,0x8E,0x8F,0x90,0x91,0x98,0x99,0x9A,0x9B,0xB9,0xBA,0xBD,0xBE,
				   0xC3,0xE3,0xA5,0xB9,0xC6,0xE6,0xC8,0xE8,0xCF,0xEF,0xD0,0xF0,0xCA,0xEA,0xCC,0xEC,0xC5,0xE5,0xBC,0xBE,
		 1,32,0xC5,0x81,0x82,0x83,0x84,0x87,0x88,0x90,0x91,0x94,0x95,0x98,0x99,0x9A,0x9B,0x9E,0x9F,0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xAE,0xAF,0xB0,0xB1,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,
				   0xA3,0xB3,0xD1,0xF1,0xD2,0xF2,0xD5,0xF5,0xC0,0xE0,0xD8,0xF8,0x8C,0x9C,0xAA,0xBA,0x8A,0x9A,0xDE,0xFE,0x8D,0x9D,0xD9,0xF9,0xDB,0xFB,0x8F,0x9F,0xAF,0xBF,0x8E,0x9E,
		  1,5,0xCB,0x87,0x98,0x99,0x9B,0x9D,
				   0xA1,0xA2,0xFF,0xB2,0xBD,
		 2,15,0xE2,0x80,0x93,0x94,0x98,0x99,0x9A,0x9C,0x9D,0x9E,0xA0,0xA1,0xA2,0xA6,0xB0,0xB9,0xBA,
						0x96,0x97,0x91,0x92,0x82,0x93,0x94,0x84,0x86,0x87,0x95,0x85,0x89,0x8B,0x9B,
		  2,1,0xE2,0x82,0xAC,
						0x80,
		  2,1,0xE2,0x84,0xA2,
						0x99,
		  1,1,0xC8,0x9A, 0xDE,
			0
		};
		int prefLen,dataLen,pos=0;
		do {
			prefLen = w2utf[pos++];
			dataLen = w2utf[pos++];
			if(!memcmp(*src,&w2utf[pos],prefLen)) { // compare prefix
				pos += prefLen; // skip prefix
				char *search = (char*)memchr(&w2utf[pos],*(*src+prefLen),dataLen); // find suffix
				if(!search)
					pos += (2 * dataLen); // not found - move to next block
				else {
					*src = *src + prefLen + 1; // 2 or 3 bytes
					return *(search + dataLen); // second half of data - Win-1250
				}
			} else {
				pos += prefLen + (2 * dataLen); // different prefix - move to next block
			}

		} while (w2utf[pos]);
		(*src)++; // move pointer
		return 0;
	}
};
template<unsigned short int T, unsigned short int U>
cString<T+U> operator + (const cString<T>& _res, const cString<U>& _src) {
	cString<T+U> res;
	memcpy(&res, &_res, sizeof(_res));
	res += _src;
	return res;
}
template<unsigned short int T>
cString<2*T+100> operator + (const cString<T>& _res, const char* _src) {
	cString<2*T+100> res;
	res = _res;
	res += _src;
	return res;
}
template<unsigned short int T>
cString<2*T+100> operator + (const char* _src, const cString<T>& _res) {
	cString<2*T+100> res(_src);
	res += _res;
	return res;
}
template<unsigned short int T>
cString<T+1> operator + (const cString<T>& _res, const char _src) {
	cString<T+1> res;
	res = _res;
	res += _src;
	return res;
}
template<unsigned short int T>
cString<T+1> operator + (const char _src, const cString<T>& _res) {
	cString<T+1> res;
	res += _src;
	res += _res;
	return res;
}
template<unsigned short int T>
ostream& operator << (ostream& os, const cString<T>& str) {
#if !defined (WIN32) || defined(__MINGW32__)
	cStringU<2*T> uni(str); // v Linuxu konverze Win1250 -> Utf-8
	os << uni;
#else
	os << str.c_str();
#endif
	return os;
}
template<unsigned short int T>
istream& operator >> (istream& os, cString<T>& str) {
	string tmp;
#if !defined (WIN32) || defined(__MINGW32__)
	os >> tmp;
	str.fromUtf(tmp.c_str()); // v Linuxu konverze Utf-8 -> Win1250
#else
	os >> tmp;
	str = tmp.c_str();
#endif
	return os;
}
#endif

#ifdef TestingCstring
static class Tester {
	void EXPECT_TRUE (bool args )
	{
		if (!(args)) { fprintf(stderr, "test failed (EXPECT_TRUE) in %s, line %d\n", __FILE__, __LINE__);
		  exit(1);
		}
	}
	// https://android.googlesource.com/platform/external/astl/+/donut-release2/tests/test_string.cpp
	/* -*- c++ -*- */ 
	/* 
	* Copyright (C) 2009 The Android Open Source Project 
	* All rights reserved. 
	* 
	* Redistribution and use in source and binary forms, with or without 
	* modification, are permitted provided that the following conditions 
	* are met: 
	* * Redistributions of source code must retain the above copyright 
	* notice, this list of conditions and the following disclaimer. 
	* * Redistributions in binary form must reproduce the above copyright 
	* notice, this list of conditions and the following disclaimer in 
	* the documentation and/or other materials provided with the 
	* distribution. 
	* 
	* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
	* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
	* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
	* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
	* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
	* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
	* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
	* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
	* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
	* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
	* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
	* SUCH DAMAGE. 
	*/
	bool testConstructorCString() 
	{ 
		cString<50> empty_str1; 
		EXPECT_TRUE(empty_str1.length() == 0); 

		cString<50> empty_str2(""); 
		EXPECT_TRUE(empty_str2.length() == 0); 

		const char empty_as_array[] = ""; 
		string empty_str3(empty_as_array); 
		EXPECT_TRUE(empty_str3.length() == 0); 

		const char literal[] = "scott mills cracks me up"; 
		cString<30> str1(literal); 
		EXPECT_TRUE(strcmp(literal, str1.c_str()) == 0); 

		cString<30> str2(literal);
		str2 = str2.substr(0, 11);
		EXPECT_TRUE(strcmp("scott mills", str2.c_str()) == 0); 

		cString<30> str3(literal); 
		EXPECT_TRUE(strcmp(literal, str3.c_str()) == 0); 

		const char text[] = {'l','a','d','y',' ','g','a','g','a'}; 

		cString<1> str7(""); 
		EXPECT_TRUE(empty_str1.length() == 0); 
		return true; 
	} 
	bool testConstructorString() 
	{ 
		cString<1> empty_str1; 
		cString<1> empty_str2; 

		cString<1> empty_str3(empty_str2); 
		EXPECT_TRUE(empty_str3.length() == 0); 

		const char string_with_nulls[] = "contains 2 \0 bytes \0.";
		cString<24>& str1=*(cString<24>*)"\x15\0contains 2 \0 bytes \0."; // (string_with_nulls, 21);
		EXPECT_TRUE(str1.length() == 21); 

		cString<24> str2 (str1);  // default undefined copy constructor
		EXPECT_TRUE(str1.length() == 21); 

		const char literal[] = "scott mills cracks me up"; 
		const cString<24> str3(literal); 
		EXPECT_TRUE(str3 == literal); 

		return true; 
	} 
	bool testConstructorRepeatChar() 
	{ 
		cString<1>& str01 = *(cString<1>*)"\0\0c"; 

		EXPECT_TRUE(str01.length() == 0); 

		cString<1>& str02 = *(cString<1>*)"\xA\0c"; 

		EXPECT_TRUE(str02 != 0); 
		EXPECT_TRUE(str02.length() == 10); 

		return true; 
	} 
	bool testConstructorPointers() 
	{ 
		const cString<0> empty; 
		char data[] = "a 16 char string"; 

		cString<16> str01(data + sizeof(data) - 1); 

		cString<16> str02(data[0]); 
		EXPECT_TRUE(str02 == "a"); 

		cString<16> str03(data + 2);
		str03 = str03.substr(0, 16-2);
		EXPECT_TRUE(str03 == "16 char string"); 

		cString<16> str04(data + 15); 
		str04 = str04.substr(0, 1);
		EXPECT_TRUE(str04 == "g"); 

		cString<16> str05(data + 16); 
		EXPECT_TRUE(str05 == ""); 

		return true; 
	} 
	bool testConstructorInvalidValues() 
	{ 
		const string empty; 
		const cString<16> str01("a 16 char string"); 

		EXPECT_TRUE(str01.length() == 16); 

		return true; 
	} 
	bool testSize() 
	{ 
		cString<10> str01; 
		EXPECT_TRUE(str01.length() == 0); 
		EXPECT_TRUE(str01.length() == 0); 

		str01 += "a string."; 

		EXPECT_TRUE(str01.length() == 9); 
		EXPECT_TRUE(str01.length() == 9); 

		return true; 
	} 
	bool testCString() 
	{ 
		cString<10> str01; 
		cString<1> str02; 

		// Should point to the same empty string. 
		EXPECT_TRUE(str01.c_str() == str01.operator const char *()); 
		EXPECT_TRUE(!*str01); 

		const char text[] = "a string"; 
		str01 += text; 
		EXPECT_TRUE(strcmp(str01.c_str(), text) == 0); 
		EXPECT_TRUE(strcmp(str01.operator const char *(), text) == 0); 
		EXPECT_TRUE(!!*str01); 

		return true; 
	} 
	bool testAppend() 
	{ 
		cString<40> str1; 
		const char *text = "You spin my head right round."; 

		str1 += text; 
		EXPECT_TRUE(str1 == text); 

		str1 += " Flo Rida."; 
		EXPECT_TRUE(str1 == "You spin my head right round. Flo Rida."); 

		cString<40> str2; 
		str2 += str1; 
		EXPECT_TRUE(str2 == "You spin my head right round. Flo Rida."); 

		cString<40> str3("You spin "); 
		str3 += "my head right round."; 
		EXPECT_TRUE(str3 == "You spin my head right round."); 

		cString<40> str4("You spin "); 
		cString<40> str5("my head right round."); 
		str4 += str5; 
		EXPECT_TRUE(str4 == "You spin my head right round."); 

		cString<40> str6(""); 
		cString<40> str7(""); 
		str6 += str7; 
		EXPECT_TRUE(str6 == ""); 

		cString<40> str8; 
		str8 += "a"; 
		EXPECT_TRUE(str8 == "a"); 

		const char more_text[] = {'l','a','d','y',' ','g','a','g','a'}; 

		cString<50> str10; 
		str10 += ""; 
		EXPECT_TRUE(str10.length() == 0 ); 
		str10 += text; 
		EXPECT_TRUE(str10 == "You spin my head right round."); 

		cString<50> str11; 
		str11 = str10.substr(5,11);

		EXPECT_TRUE(str11 == "pin my head"); 

		return true; 
	}
	bool testAppendOperator() 
	{ 
		cString<50> str1; 
		const char *text = "You spin my head right round."; 

		str1 += text; 
		EXPECT_TRUE(str1 == text); 

		str1 += " Flo Rida."; 
		EXPECT_TRUE(str1 == "You spin my head right round. Flo Rida."); 

		cString<50> str2; 
		str2 += str1; 
		EXPECT_TRUE(str2 == "You spin my head right round. Flo Rida."); 

		cString<50> str3("You spin "); 
		str3 += "my head right round."; 
		EXPECT_TRUE(str3 == "You spin my head right round."); 

		cString<50> str4("You spin "); 
		cString<50> str5("my head right round."); 
		str4 += str5; 
		EXPECT_TRUE(str4 == "You spin my head right round."); 

		cString<50> str6(""); 
		cString<50> str7(""); 
		str6 += str7; 
		EXPECT_TRUE(str6 == ""); 
		EXPECT_TRUE(!*str6); 

		cString<50> str8; 
		str8 += "a"; 
		EXPECT_TRUE(str8 == "a"); 

		const char more_text[] = {'l','a','d','y',' ','g','a','g','a'}; 

		cString<20> str9; 
		for (size_t i = 0; i < sizeof(more_text); ++i) 
		{ 
			str9 += more_text[i]; 
		} 
		EXPECT_TRUE(str9 == "lady gaga"); 

		str9 += (const char *)NULL; 
		EXPECT_TRUE(str9 == "lady gaga"); 

		return true; 
	}
	bool testConcat() 
	{ 
		cString<20> str01("The full"); 
		cString<20> str02(" sentence."); 
		cString<20> str03; 

		str03 = str01 + str02; 
		EXPECT_TRUE(str03 == "The full sentence."); 

		str03 = str02 + str01; 
		EXPECT_TRUE(str03 == " sentence.The full"); 


		str03 = str01 + " sentence."; 
		EXPECT_TRUE(str03 == "The full sentence."); 

		str03 = "The full" + str02; 
		EXPECT_TRUE(str03 == "The full sentence."); 

		str03 = 'l' + str02; 
		str03 = 'l' + str03; 
		str03 = 'u' + str03; 
		str03 = 'f' + str03; 
		str03 = ' ' + str03; 
		str03 = 'e' + str03; 
		str03 = 'h' + str03; 
		str03 = 'T' + str03; 
		EXPECT_TRUE(str03 == "The full sentence."); 

		str03 = "The full "; 
		str03 = str03 + 's'; 
		str03 = str03 + 'e'; 
		str03 = str03 + 'n'; 
		str03 = str03 + 't'; 
		str03 = str03 + 'e'; 
		str03 = str03 + 'n'; 
		str03 = str03 + 'c'; 
		str03 = str03 + 'e'; 
		str03 = str03 + '.'; 
		EXPECT_TRUE(str03 == "The full sentence."); 

		// Check the new string buffer is not the same as the original one. 
		cString<20> str04("left and"); 
		cString<20> str05(" right"); 
		cString<20> str06(str04 + str05); 

		EXPECT_TRUE(str06 == "left and right"); 
		EXPECT_TRUE(str06.c_str() != str04.c_str()); 
		EXPECT_TRUE(str06.c_str() != str05.c_str()); 

		str06 = str04 + str05; 
		EXPECT_TRUE(str06 == "left and right"); 
		EXPECT_TRUE(str06.c_str() != str04.c_str()); 
		EXPECT_TRUE(str06.c_str() != str05.c_str()); 
		return true; 
	}
	bool testAssignment() 
	{ 
		const char *literal = "Need to buy a full face helmet for Lilie."; 
		const cString<50> str01 = literal; 

		EXPECT_TRUE(str01.length() == strlen(literal)); 
		EXPECT_TRUE(str01.length() == strlen(literal)); 
		EXPECT_TRUE(str01 == literal); 

		cString<40> str02; 

		str02 = str01.substr(8, 33); 
		EXPECT_TRUE(str02 == "buy a full face helmet for Lilie."); 

		str02 = str01.substr(8,0);//.assign(str01, 8, 0); 
		EXPECT_TRUE(str02 == ""); 

		str02 = str01.substr(0, 7); 
		EXPECT_TRUE(str02 == "Need to"); 

		str02 = str01.substr(35,6);//.assign(str01, 35, 6); 
		EXPECT_TRUE(str02 == "Lilie."); 


		str02 = str01.c_str() + 35;//, 5); 
		str02 = str02.substr(0,5);
		EXPECT_TRUE(str02 == "Lilie"); 

		cString<50> str03; 

		str03 = literal; 
		EXPECT_TRUE(str03 == "Need to buy a full face helmet for Lilie."); 

		cString<50> str04; 

		str04 = str03.c_str(); 
		EXPECT_TRUE(str04 == "Need to buy a full face helmet for Lilie."); 

		str04 = str03.c_str() + 5;
		str04 = str04.substr(0, 10); 
		EXPECT_TRUE(str04 == "to buy a f"); 

		str04 = "noop"; 
		str04 = NULL; 
		EXPECT_TRUE(str04 == "noop"); 

		str04 = str01.c_str() + str01.length() - 1; 
		EXPECT_TRUE(str04 == "."); 

		str04 = "unchanged"; 
//		str04 = str01.c_str()+str01.length(); 
		str04 = NULL;
		EXPECT_TRUE(str04 == "unchanged"); 

		return true; 
	}
	bool testCompare() 
	{ 
		cString<20> str01("bell helmet"); 
		cString<20> str02("bell moto"); 
		cString<20> str03("bell"); 
		cString<20> str04("bell pants"); 
		cString<20> str05; 

		str05 = str01; 
		// Compare with self. 
		EXPECT_TRUE(str01 == str01); 
		EXPECT_TRUE(!(str01 != str01)); 

		EXPECT_TRUE(str01 == str05); 
		EXPECT_TRUE(str05 == str01); 
		EXPECT_TRUE(!(str01 != str05)); 
		EXPECT_TRUE(!(str05 != str01)); 

		EXPECT_TRUE(str01 != str02); 
		EXPECT_TRUE(str01 != str03); 
		EXPECT_TRUE(str01 != str04); 

		// Compare with literals. 
		EXPECT_TRUE(str01 == "bell helmet"); 
		EXPECT_TRUE(!(str01 != "bell helmet")); 

		// Compare with char array. 
		char array[] = { 'a', ' ', 'b', 'u', 'g', '\0'}; 
		str01 = "a bug"; 

		EXPECT_TRUE(strcmp("a bug", "a bugg") < 0); 

		char array2[] = { 'a', 'b', 'u', 'g', 'g' }; 
		EXPECT_TRUE(str01 < array2); 

		cString<1> str06; 
		EXPECT_TRUE(str06 != NULL); 
		return true; 
	}
	bool testAccessor() 
	{ 
		cString<10> str01 = "earmarks"; 

		EXPECT_TRUE(str01[0] == 'e'); 
		EXPECT_TRUE(str01[7] == 's'); 

		str01[0] = 'E'; 
		str01[7] = 'S'; 
		EXPECT_TRUE(str01 == "EarmarkS"); 

		return true; 
	}
	bool testFind() 
	{ 
		cString<30> haystack("one two three one two three"); 

		// Don't die on null strings 
		EXPECT_TRUE(haystack.find((char*)NULL) == string::npos); 
		EXPECT_TRUE(haystack.find((char*)NULL, 10) == string::npos); 

		// C strings. 
		EXPECT_TRUE(haystack.find("one") == 0); 
		EXPECT_TRUE(haystack.find("two") == 4); 
		EXPECT_TRUE(haystack.find("t") == 4); 
		EXPECT_TRUE(haystack.find("four") == string::npos); 
		EXPECT_TRUE(haystack.find("one", string::npos) == string::npos); 

		// with offset 
		EXPECT_TRUE(haystack.find("one", 13) == 14); 
		EXPECT_TRUE(haystack.find("one", 14) == 14); 
		EXPECT_TRUE(haystack.find("one", 15) == string::npos); 
		EXPECT_TRUE(haystack.find("e", haystack.length() - 1) == haystack.length() - 1); 
		EXPECT_TRUE(haystack.find("e", haystack.length()) == string::npos); 
		EXPECT_TRUE(haystack.find("one", string::npos) == string::npos); 

		// std::string 
		EXPECT_TRUE(haystack.find(cString<3>("one")) == 0); 
		EXPECT_TRUE(haystack.find(cString<3>("two")) == 4); 
		EXPECT_TRUE(haystack.find(cString<1>("t")) == 4); 
		EXPECT_TRUE(haystack.find(cString<4>("four")) == string::npos); 
		EXPECT_TRUE(haystack.find(cString<3>("one"), -1) == -1); 

		// with offset 
		EXPECT_TRUE(haystack.find(cString<3>("one"), 13) == 14); 
		EXPECT_TRUE(haystack.find(cString<3>("one"), 14) == 14); 
		EXPECT_TRUE(haystack.find(cString<3>("one"), 15) == string::npos); 
		EXPECT_TRUE(haystack.find(cString<1>("e"), haystack.length() - 1) == haystack.length() - 1); 
		EXPECT_TRUE(haystack.find(cString<1>("e"), haystack.length()) == string::npos); 
		EXPECT_TRUE(haystack.find(cString<3>("one"), string::npos) == string::npos); 

		// Emtpy string should be found at every position in a string except 
		// past the end. 
		EXPECT_TRUE(string().find("", 0) == 0); 
		EXPECT_TRUE(string().find(string(), 0) == 0); 
		EXPECT_TRUE(string().find(string(), 10) == string::npos); 

		string foo = "foo"; 
		EXPECT_TRUE(foo.find("", 0) == 0); 
		EXPECT_TRUE(foo.find(string(), 0) == 0); 
		EXPECT_TRUE(foo.find(string(""), 0) == 0); 

		EXPECT_TRUE(foo.find("", 1) == 1); 
		EXPECT_TRUE(foo.find(string(), 1) == 1); 
		EXPECT_TRUE(foo.find(string(""), 1) == 1); 

		EXPECT_TRUE(foo.find("", foo.length()) == foo.length()); 
		EXPECT_TRUE(foo.find(string(), foo.length()) == foo.length()); 
		EXPECT_TRUE(foo.find(string(""), foo.length()) == foo.length()); 

		EXPECT_TRUE(foo.find("", foo.length() + 1) == string::npos); 
		EXPECT_TRUE(foo.find(string(), foo.length() + 1) == string::npos); 
		EXPECT_TRUE(foo.find(string(""), foo.length() + 1) == string::npos); 

		// Find on an empty string a non empty one should fail 
		EXPECT_TRUE(string().find("f", 0) == string::npos); 
		EXPECT_TRUE(string().find(string("f"), 0) == string::npos); 
		return true; 
	}
	bool testSubstr() 
	{ 
		const char literal[] = "basement jaxx"; 
		const cString<20> str01(literal); 
		cString<20> str02; 

		str02 = str01.substr(0, 5); 
		EXPECT_TRUE(str02 == "basem"); 

		str02 = str01.substr(0, 8); 
		EXPECT_TRUE(str02 == "basement"); 

		str02 = str01.substr(0, string::npos); 
		EXPECT_TRUE(str02 == "basement jaxx"); 

		str02 = str01.substr(); 
		EXPECT_TRUE(str02 == "basement jaxx"); 

		str02 = str01.substr(9); 
		EXPECT_TRUE(str02 == "jaxx"); 

		str02 = str01.substr(9, string::npos); 
		EXPECT_TRUE(str02 == "jaxx"); 
		return true; 
	}
public:
	Tester() {
#define FAIL_UNLESS(a) a()
		FAIL_UNLESS(testConstructorCString);
		FAIL_UNLESS(testConstructorString);
		FAIL_UNLESS(testConstructorRepeatChar);
		FAIL_UNLESS(testConstructorPointers);
		FAIL_UNLESS(testConstructorInvalidValues);
		FAIL_UNLESS(testSize);
		FAIL_UNLESS(testCString);
		FAIL_UNLESS(testAppend);
		FAIL_UNLESS(testAppendOperator);
		FAIL_UNLESS(testConcat);
		FAIL_UNLESS(testAssignment);
		FAIL_UNLESS(testCompare);
		FAIL_UNLESS(testAccessor);
		FAIL_UNLESS(testFind);
		FAIL_UNLESS(testSubstr);

		FAIL_UNLESS(ownTests);
	}
	bool incrTest(char *oneP, unsigned char lastByteMem, unsigned char lastByte) {
		unsigned char incr = *(oneP+0+11); // check rest of object end marker's low byte
		for(int i=1;i<10;i++) { // 
			if(*(unsigned char*)(oneP+i+11) - incr != i)
				return false;
		}
		if((*(void**)(oneP+10+10) != oneP+10+10) || (lastByteMem != lastByte)) // check last object marker
			return false;
		return true;
	}
	bool ownTests() {
		cString<10> one, two;
		char *oneP = const_cast<char*>(one.c_str());
		int res = memcmp(&one, &two, sizeof(cString<10>)); // empty strings are equal
		if(res) return false;
		*oneP = 'x'; // check mark end of string
		one = "";
		res = memcmp(&one, &two, sizeof(cString<10>)); // empty strings are equal
		if(res) return false;
		one = '1';
		two = "1";
		one = ""; // no change 4 empty string
		res = memcmp(&one, &two, sizeof(cString<10>)); // empty strings are equal
//	cString& operator =(const char* src)
		cString<20> *space = new cString<20>;
		char *spaceP = const_cast<char*>(space->c_str());
		unsigned __int16 &spaceL = *(unsigned __int16*)space;
		cString<23> _one2;
		cString<10>& one2 = *(cString<10>*)&_one2;
		oneP = const_cast<char*>(one2.c_str());
		unsigned char& lastByte = *(unsigned char*)(oneP+10+10+4);
		unsigned char lastByteMem = lastByte;
		for(int i=1;i<20;i++) {
			*(spaceP+i-1) = i+'0'-1;
			*(spaceP+i) = 0;
			*((void**)(oneP+i+1)) = oneP+i+1; // object end marker
			one2 = spaceP;
			spaceL = i<=10?i:10;
			res = memcmp(&one2, space, 2+(i<=10?i:10));
			res |= *((void**)(oneP+i+1)) != oneP+i+1;
			if(res)
				return false;
		}
		if(!incrTest(oneP, lastByteMem, lastByte))
			return false;
//	cString& operator +=(const char *src) { // "strcat"
		one2 = "";
		for(int i=1;i<20;i++) {
			*(spaceP+i-1) = i+'0'-1;
			*(spaceP+i) = 0;
			*((void**)(oneP+i+1)) = oneP+i+1; // object end marker
			one2 += spaceP+i-1;
			spaceL = i<=10?i:10;
			res = memcmp(&one2, space, 2+(i<=10?i:10));
			res |= *((void**)(oneP+i+1)) != oneP+i+1;
			if(res)
				return false;
		}
		if(!incrTest(oneP, lastByteMem, lastByte))
			return false;
		one2 += "12345678901234567890";
		one2 = "";
		one2 += "12345678901234567890";
		if(!incrTest(oneP, lastByteMem, lastByte))
			return false;
//	const cString& operator +=(char src) { // "strcat"
		one2 = "";
		lastByteMem = lastByte;
		for(int i=1;i<20;i++) {
			*(spaceP+i-1) = i+'0'-1;
			*(spaceP+i) = 0;
			*((void**)(oneP+i+1)) = oneP+i+1; // object end marker
			one2 += char(i+'0'-1);
			spaceL = i<=10?i:10;
			res = memcmp(&one2, space, 2+(i<=10?i:10));
			res |= *((void**)(oneP+i+1)) != oneP+i+1;
			if(res)
				return false;
		}
		if(!incrTest(oneP, lastByteMem, lastByte))
			return false;
		delete space;
		return true;
	}
};
Tester t;
#undef TestingCstring
#endif