//#define TestingCstring
#ifndef CSTRING
#define CSTRING
#include <string.h>
#include <iostream>
#ifdef WIN32
#pragma warning (disable : 4996)
#endif
#if defined (WIN32) && !defined(__MINGW32__)
typedef unsigned __int16 U16;
typedef __int16 I16;
#else
#include <sys/types.h>
typedef u_int16_t U16;
typedef int16_t I16;
#endif

using namespace std;

template<U16 c_maxSize>
class cString;
template<U16 c_maxSize>
class cStringU {
public:
	cStringU() {
		m_size = 0; *m_data = 0;
	}
	template<U16 c_maxSizeU>
	explicit cStringU (const cString<c_maxSizeU>& src) {
		unsigned char *srcPtr = (unsigned char*)src.c_str();//(unsigned char *)(((char*)&src) + 2);
		I16 maxSize = c_maxSize;
		if(*srcPtr) encodeUtf((unsigned char *)m_data, srcPtr, maxSize); else *m_data=0;
		if(maxSize <= 0) {
			*(m_data + c_maxSize - 1) = 0;
			if(maxSize < -1) throw "cString Internal error";
			maxSize = 0;
		}
		m_size = c_maxSize - maxSize;
	}
	cStringU& operator =(const char* srcPtr) {
		I16 maxSize = c_maxSize;
		if(srcPtr && *srcPtr)
			encodeUtf(m_data, (unsigned char *)srcPtr, maxSize);
		else *m_data=0;
		if(maxSize <= 0) {
			*(m_data + c_maxSize - 1) = 0;
			if(maxSize < -1) throw "cString Internal error";
			maxSize = 0;
		}
		m_size = c_maxSize - maxSize;
		return *this;
	}
	inline operator const char*() const { return (char*)m_data; }
	inline const unsigned char *c_str() const { return const_cast<unsigned char*>(m_data); }
	inline U16 max_size() const { return c_maxSize; }
	unsigned char& operator [] (const int pos) const {
		if (pos > m_size)
			throw "Index out of string.";

		return const_cast<unsigned char&>(m_data[pos]); // možno i přepsat
	}
	U16 length() const { return m_size; }
private:
	U16 m_size;
	unsigned char m_data[c_maxSize];
	void encodeUtf(unsigned char *dest, unsigned char *src, I16 &maxSize) {
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
#define cStringC(name,content) cString<sizeof(content)-1> name;{char cS[sizeof(content)+2] = {(sizeof(content)-1)&255,(sizeof(content)-1)>>8,content};name=*(cString<sizeof(content)-1>*)&cS;}
template<U16 c_maxSize>
class cString {
public:
	cString () {
//		*m_data = 0;
//		m_size = c_maxSize;
//		char *ptr = m_data;
//		while(m_size--) *ptr++ = (a=a>'8'?'0':a+1); // inicializace
		*m_data = 0; // neinicializovany string by se nemel pouzivat, takze maly chytak bude v nem - 1234567890...
		m_size = 0;
	}
	template<U16 T>
	explicit cString (const cString<T>& src) { // copy constructor
		if(src.length() > c_maxSize) {
			memcpy(this, &src, &m_data[0] - (char*)this + c_maxSize);
			m_data[c_maxSize] = 0;
		} else
			memcpy(this, &src, &m_data[0] - (char*)this + src.length() + 1);
	}
	cString (const cString<c_maxSize>& src) { // copy constructor
		memcpy(this, &src, &m_data[0] - (char*)this + src.length() + 1);
	}
	cString (const char* src) {
		operator =(src);
	}
	cString (const char src) {
		bool notEmpty = c_maxSize > 0;
		if(notEmpty) {
			m_size = 1;
			*m_data = src;
			*(m_data+1) = 0;
		}
	}
	cString& operator =(const char* src) {
		m_size = 0;
		if(!src || !*src) {
			*m_data = 0;
			return *this;
		}
		U16 remainingSize = c_maxSize;
		char *ptr = &m_data[0];
		for(;remainingSize;remainingSize--) {
			if((*ptr++ = *src++)) {
				m_size++;
			} else return *this;
		}
		*ptr = 0;
		return *this;
	}
	cString& fromUtf(const unsigned char *src) {
		char *ptr = m_data;
		unsigned char *srcPtr = (unsigned char *)src;
		I16 maxSize = c_maxSize;
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
		if(!src ||!*src) return *this;
		int remainingSize = c_maxSize - m_size;
		if(remainingSize < 1) return *this;
		char *ptr = &m_data[0] + m_size;
		for(;remainingSize;remainingSize--) {
			if((*ptr++ = *src++)) {
				m_size++;
			} else return *this;
		}
		*ptr = 0;
		return *this;
	}
	const cString& operator +=(char src) { // "strcat"
		if(c_maxSize - m_size < 1) return *this;
		char *ptr = m_data + m_size;
		*ptr++ = src;
		m_size++;
		*ptr = 0;
		return *this;
	}
	cString& num2str(long long number, unsigned short maxSize) {
		// number size ns >= log 2^bits
		// ns >= bits * log 2
		bool negative = number < 0;
		if(negative) {
			number = -number;
		}
		unsigned short i=0;
		cString<21> asciiNum = "                     ";
		asciiNum = asciiNum.substr(0, maxSize);
		if(!number) asciiNum[maxSize- ++i] = '0';
		while(number && i < maxSize) {
			i++;
			asciiNum[maxSize-i] = (number % 10) + '0';
			number /= 10;
		}
		if(maxSize - negative >= i) {
			if(negative)
				asciiNum[maxSize- ++i] = '-';
			if(maxSize-i)
				asciiNum = asciiNum.substr(maxSize-i);
			operator =(asciiNum);
			return *this;
		}
		m_size = 0;
		*m_data = 0;
		return *this;
	}
	const cString& operator +=(int number) {
		cString<20> retVal;
		retVal.num2str(number, 11);
		if(c_maxSize - m_size >= retVal.length() && retVal.length())
			operator += (retVal);
		return *this;
	}
	const cString& operator +=(long number) {
		cString<20> retVal;
		retVal.num2str(number, 11);
		if(c_maxSize - m_size >= retVal.length() && retVal.length())
			operator += (retVal);
		return *this;
	}
	const cString& operator +=(long long number) {
		cString<20> retVal;
		retVal.num2str(number, 20);
		if(c_maxSize - m_size >= retVal.length() && retVal.length())
			operator += (retVal);
		return *this;
	}
	template<U16 T>
	cString& operator +=(const cString<T>& src) { // "strcat"
		U16 remainingSize = c_maxSize - m_size;
		U16 srcSize = src.length();
        const char *srcData = (char*)&src + (&m_data[0] - (char*)this);
		if(srcSize > remainingSize) {
			if(!remainingSize) return *this;
			memcpy(m_data + m_size, srcData, remainingSize);
			m_size += remainingSize;
			*(m_data + m_size) = 0;
		} else
			if(srcSize > 0) {
				char *ptr = &m_data[0] + m_size;
				for(;remainingSize;remainingSize--) {
					if((*ptr++ = *srcData++)) {
						m_size++;
					} else return *this;
				}
				*ptr = 0;
				return *this;
			}
		return *this;
	}
	template<U16 T>
	cString& operator =(const cString<T>& src) {
		if(c_maxSize < src.length()) {
			char *source = (char*)&src;
			source += &m_data[0] - (char*)this;
			m_size = c_maxSize;
			memcpy(m_data, source, m_size);
			*(m_data + m_size) = 0;
		} else {
			memcpy(this, &src, &m_data[0] - (char*)this + src.length() + 1);
		}
		return *this;
	}
	cString& operator =(const cString& src) {
		memcpy(this, &src, &m_data[0] - (char*)this + src.length() + 1);
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
	inline U16 max_size() const { return c_maxSize; }
	char& operator [] (const int pos) const {
		if (pos > m_size || (m_size && (m_size == pos)))
			throw "Index out of string.";

		return const_cast<char&>(m_data[pos]); // možno i přepsat
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
	inline size_t find4test(const char *chr, size_t pos = 0, size_t len = -1) {
		char* ptr;

		if(!chr || m_size <= pos)
			return string::npos;
		if(len < 0) {
			ptr = strstr(m_data + pos, chr);
		} else {
			if(memcmp(m_data + pos, chr, len) == 0) {
				ptr = m_data;
			} else ptr = NULL;
		}
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
	template<U16 T>
	size_t find(const cString<T>& str, size_t pos = 0) {
		char* ptr;

		if(m_size < pos || (m_size - pos) < str.length())
			return string::npos;
		ptr = strstr(m_data + pos, str.c_str());
		if (!ptr)
			return string::npos;

		return ptr - m_data;
	}
	cString substr(U16 pos = 0, U16 len = -1) const {
		cString<c_maxSize> res;
		if(pos > m_size) return res;
		char *ptr = &res[0];
		const char *src = &m_data[0] + pos;
		U16 remainingSize = c_maxSize - pos;
		if(remainingSize > len) remainingSize = len;
		for(;remainingSize;remainingSize--) {
			if((*ptr++ = *src++)) {
				res.m_size++;
			} else
				return res;
		}
		*ptr = 0;
		return res;
	}
	~cString() {
	}
	cString& upperCase() {
		static const unsigned char c_upperCase[] = {
		// áÁčČďĎéÉěĚíÍňŇóÓřŘšŠťŤúÚůŮýÝžŽ śŚźŹ 0xe1,0xc1,0xe8,0xc8,0xef,0xcf,0xe9,0xc9,0xec,0xcc,0xed,0xcd,0xf2,0xd2,0xf3,0xd3,0xf8,0xd8,0x9a,0x8a,0x9d,0x8d,0xfa,0xda,0xf9,0xd9,0xfd,0xdd,0x9e,0x8e
		0xe1u,0xe8u,0xefu,0xe9u,0xecu,0xedu,0xf2u,0xf3u,0xf8u,0x9au,0x9du,0xfau,0xf9u,0xfdu,0x9eu, 0x9cu,0x9fu,0xe0u,0xe2u,0xe3u,0xe4u,0xe5u,0xe6u,0xe7u,0xeau,0xebu,0xeeu,0xf0u,0xf1u,0xf4u,0xf5u,0xf6u,0xfbu,0xfcu,0xfeu,0xb3u,0xb9u,0xbau,0xbeu,0xbfu, 0,
		0xc1u,0xc8u,0xcfu,0xc9u,0xccu,0xcdu,0xd2u,0xd3u,0xd8u,0x8au,0x8du,0xdau,0xd9u,0xddu,0x8eu, 0x8cu,0x8fu,0xc0u,0xc2u,0xc3u,0xc4u,0xc5u,0xc6u,0xc7u,0xcau,0xcbu,0xceu,0xd0u,0xd1u,0xd4u,0xd5u,0xd6u,0xdbu,0xdcu,0xdeu,0xa3u,0xa5u,0xaau,0xbcu,0xafu, 0};
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
	bool cpy2(char *dest, U16 size) {
		if(m_size < size) {
			memcpy(dest, m_data, m_size + 1);
			return true;
		} else {
			memcpy(dest, m_data, size - 1);
			*(dest + size - 1) = 0;
			return false;
		}
	}
	bool append(const char *src, U16 size) {
		if(c_maxSize - m_size >= size) {
			memcpy(m_data + m_size, src, size);
			m_size += size;
			*(m_data + m_size) = 0;
			return true;
		} else {
			memcpy(m_data + m_size, src, c_maxSize - m_size);
			m_size = c_maxSize;
			*(m_data + m_size) = 0;
			return false;
		}
	}

	U16 getMaxSize() const { return c_maxSize; }
	U16 length() const { return m_size; }
protected:
	U16 m_size;
	char m_data[c_maxSize+1];
private:
	char decodeUtf(unsigned char **src) {
		unsigned char znak = **src;
		if((znak < 0xC2) || ((znak > 0xC5) && (znak != 0xC8) && (znak != 0xCB) && (znak != 0xE2))) {
			(*src)++; // no prefix => nothing to encode - no need to search in table
			return znak;
		}
		static unsigned char w2utf[] = { // prefix length,suffix byte #, utf prefix, utf suffixes, win1250 ANSI bytes, ..., 0
		 1,23,0xC2,0x81,0x83,0x88,0x90,0x98,0xA0,0xA4,0xA6,0xA7,0xA8,0xA9,0xAB,0xAC,0xAD,0xAE,0xB0,0xB1,0xB4,0xB5,0xB6,0xB7,0xB8,0xBB,
				   0x81,0x83,0x88,0x90,0x98,0xA0,0xA4,0xA6,0xA7,0xA8,0xA9,0xAB,0xAC,0xAD,0xAE,0xB0,0xB1,0xB4,0xB5,0xB6,0xB7,0xB8,0xBB,
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
		int pos=0;
		do {
			int prefLen,dataLen;
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
template<U16 T, U16 U>
cString<T+U> operator + (const cString<T>& _res, const cString<U>& _src) {
	cString<T+U> res;
	memcpy(&res, &_res, &_res[0] - (char*)&_res + _res.length() + 1);
	res += _src;
	return res;
}
template<U16 T>
cString<2*T+100> operator + (const cString<T>& _res, const char* _src) {
	cString<2*T+100> res;
	memcpy(&res, &_res, &_res[0] - (char*)&_res + _res.length() + 1);
	res += _src;
	return res;
}
template<U16 T>
cString<2*T+100> operator + (const char* _src, const cString<T>& _res) {
	cString<2*T+100> res(_src);
	res += _res;
	return res;
}
template<U16 T>
cString<T+1> operator + (const cString<T>& _res, const char _src) {
	cString<T+1> res;
	memcpy(&res, &_res, &_res[0] - (char*)&_res + _res.length()); // copy w/o useless ending 0
	res += _src;
	return res;
}
template<U16 T>
cString<T+1> operator + (const char _src, const cString<T>& _res) {
	cString<T+1> res;
	res[0] = _src;
	*(char*)&res = 1;
	res += _res;
	return res;
}
template<U16 T>
ostream& operator << (ostream& os, const cString<T>& str) {
#if !defined (WIN32) || defined(__MINGW32__)
	cStringU<2*T> uni(str); // v Linuxu konverze Win1250 -> Utf-8
	os << uni;
#else
	os << str.c_str();
#endif
	return os;
}
template<U16 T>
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
class Tester {
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

//		EXPECT_TRUE(strcmp("a bug", "a bugg") < 0); 

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

		str02 = str01.substr(0, (unsigned short)string::npos); 
		EXPECT_TRUE(str02 == "basement jaxx"); 

		str02 = str01.substr(); 
		EXPECT_TRUE(str02 == "basement jaxx"); 

		str02 = str01.substr(9); 
		EXPECT_TRUE(str02 == "jaxx"); 

		str02 = str01.substr(9, (unsigned short)string::npos); 
		EXPECT_TRUE(str02 == "jaxx"); 
		return true; 
	}
public:
	Tester() {
#define FAIL_UNLESS(a) a()
/*		FAIL_UNLESS(testConstructorCString);
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
*/
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
		U16 &spaceL = *(U16*)space;
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
//	cString& num2str(long long number, int maxSize)
		cString<19> nums;
		lastByte = *(unsigned char*)(19+1+&nums[0]);
		lastByteMem = lastByte;
		long long j, iLL = 0;
		do {
			iLL=iLL*10+iLL%10+1;
			nums.num2str(iLL,19);
			j = 0;
			for(int idx=0;idx<nums.length();idx++) {
				j = j*10 + (nums[idx]-'0');
			}
			if(iLL != j)
				return false;
		} while (iLL<1234567900123456789);
//	cString& num2str(long long number, int maxSize) -1234567900123456789 to 0
		iLL = -iLL;
		cString<20> nums2;
		lastByte = *(unsigned char*)(19+1+&nums2[0]);
		lastByteMem = lastByte;
		while(1) {
			nums2.num2str(iLL,20);
			if(nums2[0] != '-' && iLL)
				return false;
			j = 0;
			for(int idx=1;idx<nums2.length();idx++) {
				j = j*10 - (nums2[idx]-'0');
			}
			if(iLL != j)
				return false;
			if(!iLL) break;
			iLL /= 10;
		}
		if(lastByte != lastByteMem)
			return false;
//		const cString& operator +=(int number)
		cString<191> nums3 = "";
		lastByte = *(unsigned char*)(56+1+&nums3[0]);
		lastByte = (unsigned char)&lastByte;
		lastByteMem = lastByte;
		for(int i=0;i >= 0;i=i*10+i%10+1) { // last # 1234567900
			nums3 += i;
		}
		if(nums3 != "01121231234123451234561234567123456781234567891234567900")
			return false;
//	const cString& operator +=(long number)
		nums3 = "";
		for(long i=0;i >= 0;i=i*10+i%10+1) { // still int32
			nums3 += i;
		}
		if(nums3 != "01121231234123451234561234567123456781234567891234567900")
			return false;
//	const cString& operator +=(long long number) { // int64
		nums3 = "";
		for(long long i=0;i >= 0;i=i*10+i%10+1) { // last # 1234567900123456789
			nums3 += i;
		}
		lastByte = *(unsigned char*)(191+1+&nums3[0]);
		lastByteMem = lastByte;
		if(nums3 != "01121231234123451234561234567123456781234567891234567900123456790011234567900121234567900123123456790012341234567900123451234567900123456123456790012345671234567900123456781234567900123456789")
			return false;
		if(lastByte != lastByteMem)
			return false;
		//cString (const char* src)
		cString<10> fromChar("");
		lastByte = *(unsigned char*)(10+1+&fromChar[0]);
		if(lastByte != lastByteMem)
			return false;
		new(&fromChar)(cString<10>)("0123456789"); // placement new
		if(lastByte != lastByteMem)
			return false;
		new(&fromChar)(cString<10>)("0");
		new(&fromChar)(cString<10>)("012345678");
		cString<10> fromChar2 = "01234567890";
		lastByte = *(unsigned char*)(10+1+&fromChar2[0]);
		if(lastByte != lastByteMem)
			return false;
	//cString fromUtf(const char *src)
		cString<255*3> ansi;
		unsigned char *dst = (unsigned char *)&ansi[0];
		char hex[17] = "0123456789ABCDEF";
		for(int i=1;i<256;i++) { // generate 
			*dst++ = hex[i>>4];
			*dst++ = hex[i&15];
			*dst++ = i;
		}
		*dst = 0;
		ansi = ansi.c_str();
		// HEXbin
		const char *ansiStr = "010203040506070809	0A\n0B0C0D\r0E0F101112131415161718191A\0321B1C1D1E1F20 21!22\"23#24$25%26&27'28(29)"
			"2A*2B+2C,2D-2E.2F/3003113223333443553663773883993A:3B;3C<3D=3E>3F?40@41A42B43C44D45E46F47G48H49I4AJ4BK4CL4DM4EN4FO50P51Q52R53S54T55U56V"
			"57W58X59Y5AZ5B[5C\\5D]5E^5F_60`61a62b63c64d65e66f67g68h69i6Aj6Bk6Cl6Dm6En6Fo70p71q72r73s74t75u76v77w78x79y7Az7B{7C|7D}7E~7F80€8182‚83"
			"84„85…86†87‡8889‰8AŠ8B‹8CŚ8DŤ8EŽ8FŹ9091‘92’93“94”95•96–97—9899™9Aš9B›9Cś9Dť9Ež9FźA0 A1ˇA2˘A3ŁA4¤A5ĄA6¦A7§A8¨A9©AAŞAB«AC¬AD­AE®AFŻB0°"
			"B1±B2˛B3łB4´B5µB6¶B7·B8¸B9ąBAşBB»BCĽBD˝BEľBFżC0ŔC1ÁC2ÂC3ĂC4ÄC5ĹC6ĆC7ÇC8ČC9ÉCAĘCBËCCĚCDÍCEÎCFĎD0ĐD1ŃD2ŇD3ÓD4ÔD5ŐD6ÖD7×D8ŘD9ŮDAÚDBŰDCÜDDÝ"
			"DEŢDFßE0ŕE1áE2âE3ăE4äE5ĺE6ćE7çE8čE9éEAęEBëECěEDíEEîEFďF0đF1ńF2ňF3óF4ôF5őF6öF7÷F8řF9ůFAúFBűFCüFDýFEţFF˙";
		if(ansi != ansiStr)
			return false; // missing chars found by Win/Notepad
		cStringU<911> utf(ansi);
		// HEX\oct
		const char *utfStr = "01\00102\00203\00304\00405\00506\00607\00708\01009\0110A\0120B\0130C\0140D\0150E\0160F\01710\02011\02112\02213\02314\02415\025"
			"16\02617\02718\03019\0311A\0321B\0331C\0341D\0351E\0361F\03720 21!22\"23#24$25%26&27'28(29)2A*2B+2C,2D-2E.2F/3003113223333443553663773883993A:3B;"
			"3C<3D=3E>3F?40@41A42B43C44D45E46F47G48H49I4AJ4BK4CL4DM4EN4FO50P51Q52R53S54T55U56V57W58X59Y5AZ5B[5C\\5D]5E^5F_60`61a62b63c64d65e66f67g68h69i6Aj6Bk"
			"6Cl6Dm6En6Fo70p71q72r73s74t75u76v77w78x79y7Az7B{7C|7D}7E~7F80\342\202\25481\302\20182\342\200\23283\302\20384\342\200\23685\342\200\24686\342\200\240"
			"87\342\200\24188\302\21089\342\200\2608A\305\2408B\342\200\2718C\305\2328D\305\2448E\305\2758F\305\27190\302\22091\342\200\23092\342\200\23193\342\200\234"
			"94\342\200\23595\342\200\24296\342\200\22397\342\200\22498\302\23099\342\204\2429A\305\2419B\342\200\2729C\305\2339D\305\2459E\305\2769F\305\272A0\302\240"
			"A1\313\207A2\313\230A3\305\201A4\302\244A5\304\204A6\302\246A7\302\247A8\302\250A9\302\251AA\305\236AB\302\253AC\302\254AD\302\255AE\302\256AF\305\273"
			"B0\302\260B1\302\261B2\313\233B3\305\202B4\302\264B5\302\265B6\302\266B7\302\267B8\302\270B9\304\205BA\305\237BB\302\273BC\304\275BD\313\235BE\304\276"
			"BF\305\274C0\305\224C1\303\201C2\303\202C3\304\202C4\303\204C5\304\271C6\304\206C7\303\207C8\304\214C9\303\211CA\304\230CB\303\213CC\304\232CD\303\215"
			"CE\303\216CF\304\216D0\304\220D1\305\203D2\305\207D3\303\223D4\303\224D5\305\220D6\303\226D7\303\227D8\305\230D9\305\256DA\303\232DB\305\260DC\303\234"
			"DD\303\235DE\305\242DF\303\237E0\305\225E1\303\241E2\303\242E3\304\203E4\303\244E5\304\272E6\304\207E7\303\247E8\304\215E9\303\251EA\304\231EB\303\253"
			"EC\304\233ED\303\255EE\303\256EF\304\217F0\304\221F1\305\204F2\305\210F3\303\263F4\303\264F5\305\221F6\303\266F7\303\267F8\305\231F9\305\257FA\303\272"
			"FB\305\261FC\303\274FD\303\275FE\305\243FF\313\231";
/*		ofstream utfs("c:\\ElTom\\VS2k8\\FontConversions\\x\\vsechny\\Test\\utf8"),
			utfsB("c:\\ElTom\\VS2k8\\FontConversions\\x\\vsechny\\Test\\utf", ios::binary);
		const unsigned char *udst = utf.c_str();
		while(*udst) {
			if(*udst < 128 && *udst > 31) {
				utfsB << *udst;
				utfs << *udst++;
			}
			while(*udst >= 128 || (*udst && *udst < 32)) {
				utfsB << *udst;
				utfs << "\\" << *udst / 64 << (*udst / 8) % 8 << *udst % 8;
				udst++;
			}
		}
		utfs.close();*/
		if(strcmp((char*)utf.c_str(), utfStr))
			return false;
		ansi.fromUtf(utf.c_str());
		if(ansi != ansiStr)
			return false;
		unsigned char *map = new unsigned char[256/8]; // fe ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
		memset(map, 0, 256/8); // create map of ANSI chars (1-255)
		dst = (unsigned char *)&ansi[0];
		while(*dst) {
			dst+=2;
			map[((unsigned char)*dst)>>3] |= 1<<(((unsigned char)*dst)&7);
			dst++;
		}
		dst = &map[0];
		int mapSize=1;
		if(*dst++ != 0xFE) // skip useless ANSI 0
			return false;
		while(mapSize++ < 32) { // check for all others
			if(*dst++ != 0xFF) return false;
		}
		// cString (const cString<c_maxSize>& src) // copy constructor
		cString<255*3> ansi2(ansi); // same size is a must @ the moment
		if(ansi != ansi2 || ansi.length() != ansi2.length() || memcmp(ansi.c_str(), ansi2.c_str(), 255*3))
			return false;
		// template<U16 T>
		// cString& operator +=(const cString<T>& src)
		one += one;
		one += nums3;
		one += one2;
		if(one != "0112123123")
			return false;
		// template<U16 T>
		// cString& operator =(const cString<T>& src)
		one = nums;
		if(one != "1234567900")
			return false;
		nums = one;
		// bool operator ==(const char* nd) const
		if(!(nums == "1234567900"))
			return false;
		// bool operator ==(cString& nd) const
		if(one == one2)
			return false;
		// inline operator const char*() const
		if((one.operator const char *() - (char*)&one) != 2)
			return false;
		// inline const char *c_str() const
		if((one.c_str() - (char*)&one) != 2)
			return false;
		// char& operator [] (const int pos) const
		// U16 length() const
		// U16 getMaxSize() const
		if((&one[one.length()-1] - (char*)&one) != (one.getMaxSize() + 1))
			return false;
		// inline U16 max_size() const
		if(one.getMaxSize() != one.max_size())
			return false;
		// cString upperCase() no international upperCase
/*		ansiStr = "010203040506070809	0A\n0B0C0D\r0E0F101112131415161718191A\0321B1C1D1E1F20 21!22\"23#24$25%26&27'28(29)"
			"2A*2B+2C,2D-2E.2F/3003113223333443553663773883993A:3B;3C<3D=3E>3F?40@41A42B43C44D45E46F47G48H49I4AJ4BK4CL4DM4EN4FO50P51Q52R53S54T55U56V"
			"57W58X59Y5AZ5B[5C\\5D]5E^5F_60`61A62B63C64D65E66F67G68H69I6AJ6BK6CL6DM6EN6FO70P71Q72R73S74T75U76V77W78X79Y7AZ7B{7C|7D}7E~7F80€8182‚83"
			"84„85…86†87‡8889‰8AŠ8B‹8CŚ8DŤ8EŽ8FŹ9091‘92’93“94”95•96–97—9899™9AŠ9B›9CŚ9DŤ9EŽ9FŹA0 A1ˇA2˘A3ŁA4¤A5ĄA6¦A7§A8¨A9©AAŞAB«AC¬AD­AE®AFŻB0°"
			"B1±B2˛B3łB4´B5\265B6¶B7·B8¸B9ąBAşBB»BCĽBD˝BEľBFżC0ŔC1ÁC2ÂC3ĂC4ÄC5ĹC6ĆC7ÇC8ČC9ÉCAĘCBËCCĚCDÍCEÎCFĎD0ĐD1ŃD2ŇD3ÓD4ÔD5ŐD6ÖD7×D8ŘD9ŮDAÚDBŰDCÜDDÝ"
			"DEŢDFßE0ŕE1ÁE2âE3ăE4äE5ĺE6ćE7çE8ČE9ÉEAęEBëECĚEDÍEEîEFĎF0đF1ńF2ŇF3ÓF4ôF5őF6öF7÷F8ŘF9ŮFAÚFBűFCüFDÝFEţFF˙";*/
		ansiStr = "010203040506070809	0A\n0B0C0D\r0E0F101112131415161718191A\0321B1C1D1E1F20 21!22\"23#24$25%26&27'28(29)"
			"2A*2B+2C,2D-2E.2F/3003113223333443553663773883993A:3B;3C<3D=3E>3F?40@41A42B43C44D45E46F47G48H49I4AJ4BK4CL4DM4EN4FO50P51Q52R53S54T55U56V"
			"57W58X59Y5AZ5B[5C\\5D]5E^5F_60`61A62B63C64D65E66F67G68H69I6AJ6BK6CL6DM6EN6FO70P71Q72R73S74T75U76V77W78X79Y7AZ7B{7C|7D}7E~7F80€8182‚83"
			"84„85…86†87‡8889‰8AŠ8B‹8CŚ8DŤ8EŽ8FŹ9091‘92’93“94”95•96–97—9899™9AŠ9B›9CŚ9DŤ9EŽ9FŹA0 A1ˇA2˘A3ŁA4¤A5ĄA6¦A7§A8¨A9©AAŞAB«AC¬AD­AE®AFŻB0°"
			"B1±B2˛B3ŁB4´B5\265B6¶B7·B8¸B9ĄBAŞBB»BCĽBD˝BEĽBFŻC0ŔC1ÁC2ÂC3ĂC4ÄC5ĹC6ĆC7ÇC8ČC9ÉCAĘCBËCCĚCDÍCEÎCFĎD0ĐD1ŃD2ŇD3ÓD4ÔD5ŐD6ÖD7×D8ŘD9ŮDAÚDBŰDCÜDDÝ"
			"DEŢDFßE0ŔE1ÁE2ÂE3ĂE4ÄE5ĹE6ĆE7ÇE8ČE9ÉEAĘEBËECĚEDÍEEÎEFĎF0ĐF1ŃF2ŇF3ÓF4ÔF5ŐF6ÖF7÷F8ŘF9ŮFAÚFBŰFCÜFDÝFEŢFF˙";
/*		dst = (unsigned char*)const_cast<char*>(ansi.upperCase().c_str());
		while(*dst == (unsigned char)*ansiStr) {
			dst++;
			ansiStr++;
		}*/
		if(ansi.upperCase() != ansiStr)
			return false;

/*
	bool operator  <(cString& nd) const
	bool operator  >(cString& nd) const
	bool operator <=(cString& nd) const
	bool operator >=(cString& nd) const
	bool operator !=(cString& nd) const
	bool operator !=(const char* nd) const
	bool operator <(char* nd) const
	bool operator >(char* nd) const
	size_t find(int chr, size_t pos = 0)
	inline size_t find(const char *chr, size_t pos = 0)

	template<U16 T>
	size_t find(const cString<T>& str, size_t pos = 0)

	cString substr(U16 pos = 0, U16 len = -1) const

	template<U16 T, U16 U>
	cString<T+U> operator + (const cString<T>& _res, const cString<U>& _src)
	template<U16 T>
	cString<2*T+100> operator + (const cString<T>& _res, const char* _src)
	template<U16 T>
	cString<2*T+100> operator + (const char* _src, const cString<T>& _res)
	template<U16 T>
	cString<T+1> operator + (const cString<T>& _res, const char _src)
	template<U16 T>
	cString<T+1> operator + (const char _src, const cString<T>& _res)
	template<U16 T>
	ostream& operator << (ostream& os, const cString<T>& str)
	template<U16 T>
	istream& operator >> (istream& os, cString<T>& str)
*/
		return true;
	}
};
Tester t;
#undef TestingCstring
#endif
