/*
  C++ String implementation + UTF decoder and c_maxSize limit
  Copyright (c) 2017-22 El Tom Elzein.  All rights reserved.
  https://github.com/eltomjan/ETEhomeTools

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This String is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/ 

#include "String.h"

char String::JoinChar = 0;
String::String()
{}

String::String(SizeType bufSize) : m_data(bufSize)
{
	*m_data.mem = 0;
}

String::String(const String& src) : m_data(src.m_data)
{
	m_data.AddRef();
}

String::String(const char *src) : m_data(src)
{}

void String::Reserve(SizeType size)
{
	if (++size == 0)
	{
		throw STR_LOG("Reserve size overflow !");
		size--;
	}
	m_data.Realloc(size);
}

String& String::fromUtf(const unsigned char *src)
{
	char *ptr = m_data.mem;
	unsigned char *srcPtr = const_cast<unsigned char *>(src);
	SizeType bufSize = m_data.MaxSize();
	const SizeType maxSpace = bufSize;
	if (*src && bufSize)
	{
		while ((*ptr++ = decodeUtf(&srcPtr)) && (bufSize-- > 0));
	}
	if (bufSize <= 0) {
		if (bufSize < -1)
		{
			throw STR_LOG("String Internal error");
		}
		*(m_data.mem + maxSpace - 1) = 0;
	}
	else *ptr = 0;
	m_data.SetSize(maxSpace - bufSize);
	return *this;
}

String& String::operator =(const char* src)
{
	m_data.SetData(src);
	return *this;
}

String& String::operator +=(const char *src)
{
	if (!src || !*src) return *this;
	m_data.AddData(src);
	return *this;
}

const String& String::operator +=(char src)
{
	if (m_data.RemainingSize() < 1)
	{
		return *this;
	}
	char *ptr = m_data.mem + m_data.Size();
	*ptr++ = src;
	m_data.AddSize(1);
	*ptr = 0;
	return *this;
}

const String& String::operator +=(long long number)
{
	String retVal;
	retVal.Reserve(20);
	retVal.num2str(number);
	if (m_data.mem != nullptr && m_data.RemainingSize() >= retVal.length())
	{
		m_data.AddData(retVal.m_data.mem);
	}
	return *this;
}

String& String::operator +=(const String& src)
{
	SizeType remainingSize = m_data.RemainingSize();
	if (!remainingSize) return *this;

	m_data.AddData(src.m_data.mem);

	return *this;
}

String& String::operator =(const String& src)
{
	if (m_data.mem == src.m_data.mem)
	{
		return *this;
	}

	m_data.Assign(src.m_data);
	return *this;
}

bool String::operator  <(String& nd) const
{
	return strcmp(m_data.mem, nd.m_data.mem) < 0;
}

bool String::operator  >(String& nd) const
{
	return strcmp(m_data.mem, nd.m_data.mem) > 0;
}

bool String::operator <=(String& nd) const
{
	return strcmp(m_data.mem, nd.m_data.mem) <= 0;
}
bool String::operator >=(String& nd) const
{
	return strcmp(m_data.mem, nd.m_data.mem) >= 0;
}

bool String::operator ==(String& nd) const
{
	if (m_data.Size() != nd.length()) return false;
	return !strcmp(m_data.mem, nd.m_data.mem);
}

bool String::operator !=(String& nd) const
{
	if (m_data.Size() != nd.length()) return true;
	return (0 != strcmp(m_data.mem, nd.m_data.mem));
}

bool String::operator !=(const char* nd) const
{
	if (!nd) return true;
	return (0 != strcmp(m_data.mem, nd));
}

bool String::operator <(char* nd) const
{
	return strcmp(m_data.mem, nd) < 0;
}

bool String::operator >(char* nd) const
{
	return strcmp(m_data.mem, nd) > 0;
}

bool String::operator ==(const char* nd) const
{
	return !strcmp(m_data.mem, nd);
}

char& String::operator [] (const int pos)
{
	if (pos < 0 || pos > m_data.Size() || (m_data.Size() && (m_data.Size() == pos)))
		throw "Index out of string.";

	return *(m_data.mem + pos);
}

String String::num2str(long long number)
{
	// number size ns >= log 2^bits
	// ns >= bits * log 2
	unsigned short maxSize = 20;
	bool negative = number < 0;
	unsigned short i = 0;
	char asciiNum[22] = "                    ";
	asciiNum[maxSize] = 0;
	if (!number) asciiNum[maxSize - ++i] = '0';
	else while (number && i < maxSize) {
		i++;
		if (negative) asciiNum[maxSize - i] = '0' - (number % 10);
		else asciiNum[maxSize - i] = (number % 10) + '0';
		number /= 10;
	}
	if (maxSize - negative >= i)
	{
		if (negative) asciiNum[maxSize - ++i] = '-';
		
		if (maxSize - i) m_data.SetData(&asciiNum[maxSize - i]);
		else m_data.SetData(asciiNum);
		return *this;
	}
	return *this;
}

SizeType String::find(int chr, SizeType pos) const
{
	char* ptr;

	if (m_data.Size() <= pos)
		return npos;
	ptr = strchr(m_data.mem + pos, chr);
	if (!ptr)
		return npos;

	return ptr - m_data.mem;
}

SizeType String::find(const char *chr, SizeType pos) const
{
	char* ptr;

	if (!chr || m_data.Size() <= pos)
		return npos;
	ptr = strstr(m_data.mem + pos, chr);
	if (!ptr)
		return npos;

	return ptr - m_data.mem;
}

SizeType String::find(const String& str, SizeType pos) const
{
	char* ptr;

	if (m_data.Size() < pos || (m_data.Size() - pos) < str.length())
		return npos;
	ptr = strstr(m_data.mem + pos, str.c_str());
	if (!ptr)
		return npos;

	return ptr - m_data.mem;
}

String String::substr(SizeType pos, SizeType len)
{
	if (pos < 0 || pos > m_data.Size())
	{
		throw STR_LOG("Invalid string position");
	}
	m_data.SetData(m_data.mem + pos);
	if (m_data.Size() > len)
		m_data.SetSize(len);
	return *this;
}

String::~String()
{}

String& String::upperCase()
{
	static const unsigned char c_upperCase[] = {
		// áÁčČďĎéÉěĚíÍňŇóÓřŘšŠťŤúÚůŮýÝžŽ śŚźŹ 0xe1,0xc1,0xe8,0xc8,0xef,0xcf,0xe9,0xc9,0xec,0xcc,0xed,0xcd,0xf2,0xd2,0xf3,0xd3,0xf8,0xd8,0x9a,0x8a,0x9d,0x8d,0xfa,0xda,0xf9,0xd9,0xfd,0xdd,0x9e,0x8e
		0xe1u,0xe8u,0xefu,0xe9u,0xecu,0xedu,0xf2u,0xf3u,0xf8u,0x9au,0x9du,0xfau,0xf9u,0xfdu,0x9eu, 0x9cu,0x9fu,0xe0u,0xe2u,0xe3u,0xe4u,0xe5u,0xe6u,0xe7u,0xeau,0xebu,0xeeu,0xf0u,0xf1u,0xf4u,0xf5u,0xf6u,0xfbu,0xfcu,0xfeu,0xb3u,0xb9u,0xbau,0xbeu,0xbfu, 0,
		0xc1u,0xc8u,0xcfu,0xc9u,0xccu,0xcdu,0xd2u,0xd3u,0xd8u,0x8au,0x8du,0xdau,0xd9u,0xddu,0x8eu, 0x8cu,0x8fu,0xc0u,0xc2u,0xc3u,0xc4u,0xc5u,0xc6u,0xc7u,0xcau,0xcbu,0xceu,0xd0u,0xd1u,0xd4u,0xd5u,0xd6u,0xdbu,0xdcu,0xdeu,0xa3u,0xa5u,0xaau,0xbcu,0xafu, 0 };
	char *pos;
	for (int i = 0; i < m_data.Size(); i++)
	{
		if (m_data.mem[i] >= 'a' && m_data.mem[i] <= 'z')
			m_data.mem[i] -= 32;
		else if ((unsigned char)m_data.mem[i] > 127) {
			pos = strchr((char*)&c_upperCase[0], m_data.mem[i]);
			if (pos)
				m_data.mem[i] = *(pos + sizeof(c_upperCase) / 2);
		}
	}
	return *this;
}

bool String::CopyTo(char *dest, SizeType size)
{
	if (m_data.Size() < size) {
		memcpy(dest, m_data.mem, m_data.Size() + 1);
		return true;
	}
	else {
		memcpy(dest, m_data.mem, size - 1);
		*(dest + size - 1) = 0;
		return false;
	}
}

void String::append(const char *src)
{
	SizeType maxSize = m_data.MaxSize(), size = m_data.Size();
	m_data.AddData(src, size);
}

char String::decodeUtf(unsigned char **src) {
	unsigned char znak = **src;
	if ((znak < 0xC2) || ((znak > 0xC5) && (znak != 0xC8) && (znak != 0xCB) && (znak != 0xE2))) {
		(*src)++; // no prefix => nothing to encode - no need to search in table
		return znak;
	}
	static unsigned const char w2utf[] = { // prefix length,suffix byte #, utf prefix, utf suffixes, win1250 ANSI bytes, ..., 0
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
	int pos = 0;
	do {
		int prefLen, dataLen;
		prefLen = w2utf[pos++];
		dataLen = w2utf[pos++];
		if (!memcmp(*src, &w2utf[pos], prefLen)) { // compare prefix
			pos += prefLen; // skip prefix
			char *search = (char*)memchr(&w2utf[pos], *(*src + prefLen), dataLen); // find suffix
			if (!search)
				pos += (2 * dataLen); // not found - move to next block
			else {
				*src = *src + prefLen + 1; // 2 or 3 bytes
				return *(search + dataLen); // second half of data - Win-1250
			}
		}
		else {
			pos += prefLen + (2 * dataLen); // different prefix - move to next block
		}

	} while (w2utf[pos]);
	(*src)++; // move pointer
	return 0;
}

String String::to_UTF8()
{
	unsigned char *srcPtr = reinterpret_cast<unsigned char *>(m_data.mem);
	SizeType maxSize = m_data.MaxSize();
	SizeType utfMaxSize = m_data.Size() * 3;
	unsigned char *dest = static_cast<unsigned char *>(malloc(utfMaxSize));
	if (*srcPtr) {
		encodeUtf(dest, srcPtr, utfMaxSize);
	}
	else *dest = 0;
	if (maxSize <= 0) {
		*(dest + maxSize - 1) = 0;
		if (maxSize < -1)
		{
			throw STR_LOG("String Internal error");
		}
		maxSize = 0;
	}
	const char *sDest = reinterpret_cast<char *>(dest);
	Reserve(SizeType(strlen(sDest)) + StrData::minSize - 1);
	m_data.SetData(sDest);
	free((void*)dest);

	return *this;
}

void String::encodeUtf(unsigned char *dest, unsigned char *src, int16_t &maxSize)
{
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
	if(maxSize > 0) *dest++ = 0;
}


void String::length(SizeType shorter)
{
	if (shorter < m_data.MaxSize() && m_data.MaxSize())
	{
		m_data.SetSize(shorter);
		m_data.mem[shorter] = 0;
	}
}

String operator + (const String& _res, const String& _src) {
	String res;
	SizeType len = _res.length();
	res.Reserve(len + _src.length() + StrData::minSize + !!String::JoinChar);
	char *dest = &res[0];
	memcpy(dest, _res, len);
	dest += len;
	if (String::JoinChar)
	{
		*dest++ = String::JoinChar;
	}
	memcpy(dest, _src, _src.length());
	len += _src.length();
	res.length(len);
	return res;
}

String operator + (const String& _res, const char* _src) {
	String res;
	size_t len2 = strlen(_src);
	SizeType len = _res.length();
	size_t resLen = len2 + len + !!String::JoinChar;
	if (resLen > String::npos)
	{
		len2 = String::npos - len - 1;
	}
	res.Reserve(static_cast<SizeType>(resLen) + StrData::minSize);
	char *dest = &res[0];
	memcpy(dest, _res, len);
	dest += len;
	if (String::JoinChar)
	{
		*dest++ = String::JoinChar;
	}
	memcpy(dest, _src, len2);
	res.length(static_cast<SizeType>(resLen));
	return res;
}

String operator + (const char* _src, const String& _res) {
	String res;
	size_t len2 = strlen(_src);
	SizeType len = _res.length();
	size_t resLen = len2 + len + !!String::JoinChar;
	if (resLen > String::npos)
	{
		len2 = String::npos - len - 1;
	}
	res.Reserve(static_cast<SizeType>(_res.max_size() + len2) + StrData::minSize);
	char *dest = &res[0];
	memcpy(dest, _src, len2);
	dest += len2;
	if (String::JoinChar)
	{
		*dest++ = String::JoinChar;
	}
	memcpy(dest, _res, len);
	res.length(static_cast<SizeType>(resLen));
	return res;
}

String operator + (const String& _res, const char _src) {
	String res(_res);
	res += _src;
	return res;
}

String operator + (char _src, const String& _res) {
	String res(_res.max_size());
	SizeType len = _res.length();
	if (1 + len > String::npos)
	{
		len--;
	}
	res.Reserve(res.max_size() + 1 + !!String::JoinChar);
	char *dest = &res[0];
	*dest++ = _src;
	memcpy(dest, _res, len);
	res.length(len + 1);
	return res;
}

std::ostream& operator << (std::ostream& os, const String& str) {
	os << str.c_str();
	return os;
}

StrData::StrData(const char *src)
{
	size_t realLen = strlen(src) + minSize;
	SizeType shorterSize = static_cast<SizeType>(realLen);
	if (realLen != shorterSize)
	{
		throw STR_LOG("String too long !");
	}
	RefsSize_t *init = reinterpret_cast<RefsSize_t*>(malloc(sizeof(RefsSize_t) + shorterSize));
	STR_LOG("New %p (%i)\n", init, shorterSize);
	init->refs = 1;
	init->size = shorterSize - minSize;
	mem = reinterpret_cast<char*>(init + 1);
	memcpy(mem, src, init->size + 1);
#ifdef DEBUG
	refsSize = init;
#endif // DEBUG
#if defined(DEBUG) || defined(STRDATA_SIZE_LIMIT)
	init->maxSize = shorterSize;
#endif // DEBUG) || STRDATA_SIZE_LIMIT
}

StrData::RefType StrData::ReleaseRef()
{
	RefsSize_t *internals = reinterpret_cast<RefsSize_t*>(mem) - 1;
	if (!--(internals->refs))
	{
		STR_LOG("Deleting %p %s\n", internals, mem);
		free((void*)internals);
		mem = nullptr;
		return 0;
	}
	else STR_LOG("Holding(%i) %p %s\n", internals->refs + 1, this, mem);

	return internals->refs;
}

void StrData::Assign(const StrData& src)
{
	RefsSize_t *internals = reinterpret_cast<RefsSize_t*>(mem) - 1, *srcInt;
	STR_LOG("Assigning(%i) %p:%s %i(%i)\n", mem ? internals->refs : -999, &src, src.mem, src.MaxSize(), src.Size());
	srcInt = reinterpret_cast<RefsSize_t*>(src.mem) - 1;
	if (mem)
	{
#ifndef STRDATA_SIZE_LIMIT
again:
#endif
		SizeType maxSize = MaxSize();
		if (maxSize > src.Size())
		{
#ifdef STRDATA_SIZE_LIMIT
			SizeType sizeLimit = src.MaxSize();
			if (maxSize < sizeLimit)
			{
				const_cast<StrData&>(src).SetMaxSize(maxSize); // short size limit back
			}
#endif
			mem = nullptr;
		}
		else
		{
#ifndef STRDATA_SIZE_LIMIT
			Realloc(src.MaxSize());
			goto again;
#else
			this->SetData(src.mem, maxSize - 1);
#endif // STRDATA_SIZE_LIMIT
		}
	}
	if (!mem)
	{
		mem = src.mem;
		srcInt->refs++;
#ifdef DEBUG
		refsSize = srcInt;
#endif // DEBUG
	}
}

StrData::StrData(SizeType size, SizeType strSize)
{
	if (size == 0)
	{
		mem = nullptr;
	}
	else
	{
		RefsSize_t *init = reinterpret_cast<RefsSize_t*>(malloc(sizeof(RefsSize_t) + size));
		STR_LOG("New %p (%i)\n", (void*)init, strSize);
		init->refs = 1;
		init->size = strSize;
		mem = reinterpret_cast<char*>(init + 1);
#ifdef DEBUG
		refsSize = init;
#endif // DEBUG
#if defined(DEBUG) || defined(STRDATA_SIZE_LIMIT)
		init->maxSize = size;
#endif // DEBUG) || STRDATA_SIZE_LIMIT
	}
	SetMaxSize(size);
}

SizeType StrData::MaxSize() const
{
	if (mem == nullptr) return 0;
#ifdef STRDATA_SIZE_LIMIT
	RefsSize_t *init = reinterpret_cast<RefsSize_t*>(mem) - 1;
	return init->maxSize;
#else
	return static_cast<SizeType>(GetSize(mem));
#endif // STRDATA_SIZE_LIMIT
}

SizeType StrData::Size() const
{
	if (mem == nullptr) return 0;
	RefsSize_t *rs = reinterpret_cast<RefsSize_t*>(mem) - 1;
	return rs->size;
}

SizeType StrData::RemainingSize() const
{
	return MaxSize() - Size();
}

void StrData::AddSize(SizeType plus)
{
	RefsSize_t *rs = reinterpret_cast<RefsSize_t*>(mem) - 1;
	rs->size += plus;
}

void StrData::SetSize(SizeType newSize)
{
	RefsSize_t *rs = reinterpret_cast<RefsSize_t*>(mem) - 1;
	rs->size = newSize;
}

StrData::RefType StrData::AddRef()
{
	if (mem)
	{
		RefsSize_t *rs = reinterpret_cast<RefsSize_t*>(mem) - 1;
		return ++rs->refs;
	}
	return 0;
}

StrData::~StrData()
{
	if (mem != nullptr)
	{
		RefsSize_t *rs = reinterpret_cast<RefsSize_t*>(mem) - 1;
		ReleaseRef();
	}
}

char &StrData::operator[](int pos)
{
	if (pos < 0 || static_cast<unsigned>(pos) >= GetSize(mem))
	{
		throw STR_LOG("invalid string position");
	}
	return mem[pos];
}

unsigned char& StrData::operator&(int pos)
{
	return *(reinterpret_cast<unsigned char*>(mem) + pos);
}

void StrData::Realloc(SizeType size)
{
	RefsSize_t *init = reinterpret_cast<RefsSize_t*>(mem) - 1;
	if (mem != nullptr
#if defined(DEBUG) || defined(STRDATA_SIZE_LIMIT)
		&& size > init->maxSize
#endif // DEBUG || STRDATA_SIZE_LIMIT
	)
	{
		ReleaseRef(); 
	}
	init = reinterpret_cast<RefsSize_t*>(malloc(sizeof(RefsSize_t) + size));
	STR_LOG("New %p (%i)\n", init, size);
	init->refs = 1;
	init->size = 0;
	mem = reinterpret_cast<char*>(init + 1);
	SetMaxSize(size);
	*mem = 0;
#ifdef DEBUG
	refsSize = init;
#endif // DEBUG
}

void StrData::SetData(const char *src, SizeType limit)
{
	size_t realLen = limit < 0 ? strlen(src) : limit;
	SizeType shorterSize = static_cast<SizeType>(realLen);
	if (mem == nullptr)
	{
#ifndef STRDATA_SIZE_LIMIT
realloc:
#endif // !STRDATA_SIZE_LIMIT
	   size_t reservedLen = realLen + minSize;
		if (reservedLen != static_cast<SizeType>(reservedLen))
		{
			reservedLen -= minSize - 1;
			if (reservedLen != static_cast<SizeType>(reservedLen))
			{
				throw STR_LOG("String too long !");
			}
		}
		Realloc(static_cast<SizeType>(reservedLen));
	}
	if (static_cast<SizeType>(realLen) == shorterSize)
	{
		SizeType maxSize = static_cast<SizeType>(GetSize(mem));
		if (shorterSize >= maxSize)
		{
#ifdef STRDATA_SIZE_LIMIT
			shorterSize = maxSize - 1;
#else
			goto realloc;
#endif // !STRDATA_SIZE_LIMIT
		}
		RefsSize_t *init = reinterpret_cast<RefsSize_t*>(mem) - 1;
		memcpy(mem, src, shorterSize);
		mem[shorterSize] = 0;
		init->size = shorterSize;
	}
	else
	{
		throw STR_LOG("String too long !");
	}
}

void StrData::AddData(const char *src, SizeType pos)
{
	size_t realLen = strlen(src);
	if (mem != nullptr)
	{
		RefsSize_t *init = reinterpret_cast<RefsSize_t*>(mem) - 1;
		SizeType shorterSize = static_cast<SizeType>(realLen + init->size);
		if (mem != nullptr && realLen < static_cast<size_t>(shorterSize))
		{
			SizeType maxSize = static_cast<SizeType>(GetSize(mem));
			if (shorterSize >= maxSize)
			{
#ifdef STRDATA_SIZE_LIMIT
				shorterSize = maxSize - 1;
#else
				init->refs++;
				char *myMem[2] = { mem };
				Realloc(shorterSize + minSize);
				memcpy(mem, myMem[0], init->size);
				myMem[1] = mem;
				mem = myMem[0];
				SizeType size = init->size;
				ReleaseRef();
				mem = myMem[1];
				init = reinterpret_cast<RefsSize_t*>(mem) - 1;
				init->size = size;
#endif
			}
			if (pos < 0)
			{
				pos = init->size;
			}
			if (pos <= init->size) // block if not following
			{
				if (String::JoinChar)
				{
					mem[pos++] = String::JoinChar;
				}
				memcpy(mem + pos, src, realLen);
				mem[shorterSize] = 0;
				init->size = shorterSize;
			}
		}
	}
}

//* CRT Debug Heap / _CrtMemBlockHeader gap[nNoMansLandSize] / anotherGap[nNoMansLandSize] tests
void Recreate(String **_tested)
{
	String *tested = *_tested;
	size_t size = tested->max_size(), shift = sizeof(int) + 4 + sizeof(SizeType) * 2;
	char d1[140], d2[140];
	if (size)
		memcpy(d1, &(*tested)[0] - shift, size + shift);
	delete tested;
	if (size)
	{
		tested = *_tested = new String(d1 + shift);
	}
	else
	{
		tested = *_tested = new String();
	}
	if (size)
	{
		memcpy(d2, &(*tested)[0] - shift, size + shift + 8);
		if (memcmp(d1, d2, size))
		{
#ifndef STRDATA_SIZE_LIMIT
			StrData::RefType &r1 = *reinterpret_cast<StrData::RefType*>(d1 + 4), &r2 = *reinterpret_cast<StrData::RefType*>(d2 + 4);
			r2 = r1;
			if (!memcmp(d1, d2, size))
				return;
#endif
			STR_LOG("Recreate %p difference ??\n", &(*tested)[0] - shift + 4);
		}
	}
}
void SmallTest()
{
	String *a = new String("abcd");
	String *b = new String("efgh");
	String *c = new String();
	String *d = new String(7);
	Recreate(&a); Recreate(&b); Recreate(&c); Recreate(&d);
	// áÁčČďĎéÉěĚíÍňŇóÓřŘšŠťŤúÚůŮýÝžŽ
	char *czech = "aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ\xe1\xc1\xe8\xc8\xef\xcf\xe9\xc9\xec\xcc\xed\xcd\xf2\xd2\xf3\xd3\xf8\xd8\x9a\x8a\x9d\x8d\xfa\xda\xf9\xd9\xfd\xdd\x9e\x8e";
	delete a;
	a = new String(czech);
	a->to_UTF8();
	Recreate(&a);
	a->fromUtf((unsigned char*)&((*a)[0]));
	if (memcmp(czech, &(*a)[0], strlen(czech) + 1))
		throw "Differs";
	a->upperCase();
	int i = a->length();
	char prev = 123;
	i--;
	do {
		if (i & 1)
			prev = (*a)[i];
		else if ((*a)[i] != prev)
			throw "Upper case";
	} while (i--);
	for (int i = 0; i < 4; i++)
	{
		delete a;
		a = new String("123");
		delete b;
		b = new String();
		b->Reserve(i);
		*b = *a;
		Recreate(&a);
		Recreate(&b);
	}
	*a = "abc"; *b = "def";
	*c = "1234";
	if (!(*a < *b) || !(*b > *c) ||
		!(*a <= *b) || !(*b >= *c) ||
		(*a == *b) || !(*b != *c) ||
		!(*a != "cba") || !(*a < "def") ||
		!(*b > "123") || !(*c == "1234"))
	{
		throw "Relations problem";
	}
	for (int i = 0; i < 4; i++)
	{
		if ((*c)[i] != i + '1')
			throw "Iterator problem";
	}

	*d = "Index out of string.";
	String f = "Index out of string.";
	int good = 0;
	try {
		(*c)[-1];
		good--;
	} catch (char * e) {
		good++;
#ifndef STRDATA_SIZE_LIMIT
		if (*d == e)
#else
		if (f == e)
#endif
			try {
				(*c)[4];
				good--;
			} catch (const char * e) {
#ifndef STRDATA_SIZE_LIMIT
				if (*d == e)
#else
				if (f == e)
#endif
					good++;
			}
		else good--;
	}
	if (good != 2)
		throw "Index limits.";
	a->num2str(INT_MIN);
	b->num2str(INT_MAX);
	c->num2str(0);
#ifdef STRDATA_SIZE_LIMIT
	if (*a != "-21474836" || *b != "214748364" || *c != "0")
#else
	if (*a != "-2147483648" || *b != "2147483647" || *c != "0")
#endif
		throw "num2str";
	*c += '1' + *c + '2'; // keep max + minSize == 11
	if (*c != "0102")
		throw "1 + 0 + 2 ?!";
	*c += "2" + *c + "3";
	if (*c != "0102201023")
		throw "xxx ?!";
	Recreate(&a); Recreate(&b); Recreate(&c); Recreate(&d);
};
// */