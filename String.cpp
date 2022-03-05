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

String::String() : c_maxSize(-1) // uninitalized => unknow limit
{
	m_size = 0;
	m_data = NULL;
}

String::String(const String& src) : c_maxSize(src.c_maxSize)
{
	m_size = src.m_size;
	memcpy(m_data, src.m_data, m_size + 1);
}

String::String(const char *src) : c_maxSize(PrepareLen(src))
{
	m_data = reinterpret_cast<unsigned char*>(new char[c_maxSize]);
	memcpy(m_data, src, m_size + 1);
}

void String::Reserve(SizeType size)
{
	if (m_data == nullptr)
	{
		size++;
		if (size == 0)
		{
			throw "Reserve size overflow !";
		}
		const_cast<SizeType&>(c_maxSize) = size;
		m_data = new unsigned char[size];
		*m_data = 0;
	}
}

String& String::fromUtf(const unsigned char *src)
{
	unsigned char *ptr = m_data;
	unsigned char *srcPtr = const_cast<unsigned char *>(src);
	SizeType maxSize = c_maxSize;
	if (*src) while ((*ptr++ = decodeUtf(&srcPtr)) && (maxSize-- > 0)); else *ptr = 0;
	if (maxSize <= 0) {
		*(m_data + c_maxSize + maxSize) = 0;
		if (maxSize < -1) throw "String Internal error";
		maxSize = 0;
	}
	else *ptr = 0;
	m_size = c_maxSize - maxSize;
	return *this;
}

String& String::operator =(const char* src)
{
	size_t realLen = strlen(src);
	SizeType size = static_cast<SizeType>(realLen);
	if (realLen != size)
	{
		throw "String too long !";
	}
	m_size = size;
	if (c_maxSize < 0)
	{
		const_cast<SizeType&>(c_maxSize) = m_size + minSize;
		m_data = reinterpret_cast<unsigned char*>(new char[c_maxSize]);
	}
	else if (realLen > static_cast<size_t>(c_maxSize))
	{
		realLen = c_maxSize - 1;
		memcpy(m_data, &src, realLen);
		m_data[realLen] = 0;
		return *this;
	}
	memcpy(m_data, src, realLen);
	m_data[realLen] = 0;
	return *this;
}

String& String::operator +=(const char *src)
{
	if (!src || !*src) return *this;
	int remainingSize = c_maxSize - m_size;
	if (remainingSize < 1) return *this;
	unsigned char *ptr = m_data + m_size;
	for (; remainingSize; remainingSize--) {
		if ((*ptr++ = *src++)) {
			m_size++;
		}
		else return *this;
	}
	*ptr = 0;
	return *this;
}

const String& String::operator +=(char src)
{
	if (c_maxSize - m_size < 1) return *this;
	unsigned char *ptr = m_data + m_size;
	*ptr++ = src;
	m_size++;
	*ptr = 0;
	return *this;
}

const String& String::operator +=(int number)
{
	return operator+=(static_cast<long long>(number));
}

const String& String::operator +=(long number)
{
	return operator+=(static_cast<long long>(number));
	return *this;
}

const String& String::operator +=(long long number)
{
	String retVal;
	retVal.Reserve(20);
	retVal.num2str(number);
	if (c_maxSize - m_size >= retVal.length() && retVal.length())
		operator += (retVal);
	return *this;
}

String& String::operator +=(const String& src)
{
	SizeType remainingSize = c_maxSize - m_size;
	SizeType srcSize = src.length();
	if (srcSize > remainingSize) {
		if (!remainingSize) return *this;
		memcpy(m_data + m_size, src.m_data, remainingSize);
		m_size += remainingSize;
		*(m_data + m_size) = 0;
	}
	else
	{
		const unsigned char *srcData = src.m_data;
		if (srcSize > 0) {
			unsigned char *ptr = m_data + m_size;
			for (; remainingSize; remainingSize--) {
				if ((*ptr++ = *srcData++)) {
					m_size++;
				}
				else return *this;
			}
			*ptr = 0;
			return *this;
		}
	}
	return *this;
}

String& String::operator =(const String& src)
{
	if (c_maxSize < 0)
	{
		m_data = reinterpret_cast<unsigned char*>(new char[src.c_maxSize]);
		const_cast<SizeType&>(c_maxSize) = src.c_maxSize;
	}
	if (c_maxSize < src.m_size) {
		m_size = c_maxSize;
		memcpy(m_data, src.m_data, --m_size);
		*(m_data + m_size) = 0;
	}
	else {
		memcpy(m_data, src.m_data, src.m_size + 1);
		m_size = src.m_size;
	}
	return *this;
}

bool String::operator  <(String& nd) const
{
	return strcmp(reinterpret_cast<char*>(m_data), reinterpret_cast<char*>(nd.m_data)) < 0;
}

bool String::operator  >(String& nd) const
{
	return strcmp(reinterpret_cast<char*>(m_data), reinterpret_cast<char*>(nd.m_data)) > 0;
}

bool String::operator <=(String& nd) const
{
	return strcmp(reinterpret_cast<char*>(m_data), reinterpret_cast<char*>(nd.m_data)) <= 0;
}
bool String::operator >=(String& nd) const
{
	return strcmp(reinterpret_cast<char*>(m_data), reinterpret_cast<char*>(nd.m_data)) >= 0;
}

bool String::operator ==(String& nd) const
{
	if (m_size != nd.m_size) return false;
	return !strcmp(reinterpret_cast<char*>(m_data), reinterpret_cast<char*>(nd.m_data));
}

bool String::operator !=(String& nd) const
{
	if (m_size != nd.m_size) return true;
	return (0 != strcmp(reinterpret_cast<char*>(m_data), reinterpret_cast<char*>(nd.m_data)));
}

bool String::operator !=(const char* nd) const
{
	if (!nd) return true;
	return (0 != strcmp(reinterpret_cast<char*>(m_data), nd));
}

bool String::operator <(char* nd) const
{
	return strcmp(reinterpret_cast<char*>(m_data), nd) < 0;
}

bool String::operator >(char* nd) const
{
	return strcmp(reinterpret_cast<char*>(m_data), nd) > 0;
}

bool String::operator ==(const char* nd) const
{
	return !strcmp(reinterpret_cast<char*>(m_data), nd);
}

char& String::operator [] (const int pos) {
	if (pos > m_size || (m_size && (m_size == pos)))
		throw "Index out of string.";

	return *reinterpret_cast<char*>(m_data + pos);
}

String& String::num2str(long long number)
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
		
		if (maxSize - i) operator =(&asciiNum[maxSize - i]);
		else operator =(asciiNum);
		return *this;
	}
	m_size = 0;
	*m_data = 0;
	return *this;
}

SizeType String::find(int chr, SizeType pos) const
{
	char* ptr;

	if (m_size <= pos)
		return npos;
	ptr = strchr(reinterpret_cast<char*>(m_data) + pos, chr);
	if (!ptr)
		return npos;

	return ptr - reinterpret_cast<char*>(m_data);
}

SizeType String::find4test(const char *chr, SizeType pos, SizeType len) const
{
	unsigned char* ptr;

	if (!chr || m_size <= pos)
		return npos;
	if (len < 0) {
		ptr = reinterpret_cast<unsigned char*>(strstr(reinterpret_cast<char*>(m_data) + pos, chr));
	}
	else {
		if (memcmp(m_data + pos, chr, len) == 0) {
			ptr = m_data;
		}
		else ptr = NULL;
	}
	if (!ptr)
		return npos;

	return ptr - m_data;
}

SizeType String::find(const char *chr, SizeType pos) const
{
	char* ptr;

	if (!chr || m_size <= pos)
		return npos;
	ptr = strstr(reinterpret_cast<char*>(m_data) + pos, chr);
	if (!ptr)
		return npos;

	return ptr - reinterpret_cast<char*>(m_data);
}

SizeType String::find(const String& str, SizeType pos) const
{
	char* ptr;

	if (m_size < pos || (m_size - pos) < str.length())
		return npos;
	ptr = strstr(reinterpret_cast<char*>(m_data) + pos, str.c_str());
	if (!ptr)
		return npos;

	return ptr - reinterpret_cast<char*>(m_data);
}

String String::substr(SizeType pos, SizeType len) const
{
	String res;
	if (pos > m_size) {
		return res;
	}
	const unsigned char *src = m_data + pos;
	SizeType remainingSize = m_size - pos;
	const_cast<SizeType&>(res.c_maxSize) = m_size + minSize;
	unsigned char *ptr = res.m_data = new unsigned char[res.c_maxSize];
	if (remainingSize > len) remainingSize = len;
	for (; remainingSize; remainingSize--) {
		if ((*ptr++ = *src++)) {
			res.m_size++;
		}
		else
			return res;
	}
	*ptr = 0;
	return res;
}

String::~String()
{
	if (m_data != nullptr)
	{
		delete[] m_data;
	}
}

String& String::upperCase()
{
	static const unsigned char c_upperCase[] = {
		// áÁčČďĎéÉěĚíÍňŇóÓřŘšŠťŤúÚůŮýÝžŽ śŚźŹ 0xe1,0xc1,0xe8,0xc8,0xef,0xcf,0xe9,0xc9,0xec,0xcc,0xed,0xcd,0xf2,0xd2,0xf3,0xd3,0xf8,0xd8,0x9a,0x8a,0x9d,0x8d,0xfa,0xda,0xf9,0xd9,0xfd,0xdd,0x9e,0x8e
		0xe1u,0xe8u,0xefu,0xe9u,0xecu,0xedu,0xf2u,0xf3u,0xf8u,0x9au,0x9du,0xfau,0xf9u,0xfdu,0x9eu, 0x9cu,0x9fu,0xe0u,0xe2u,0xe3u,0xe4u,0xe5u,0xe6u,0xe7u,0xeau,0xebu,0xeeu,0xf0u,0xf1u,0xf4u,0xf5u,0xf6u,0xfbu,0xfcu,0xfeu,0xb3u,0xb9u,0xbau,0xbeu,0xbfu, 0,
		0xc1u,0xc8u,0xcfu,0xc9u,0xccu,0xcdu,0xd2u,0xd3u,0xd8u,0x8au,0x8du,0xdau,0xd9u,0xddu,0x8eu, 0x8cu,0x8fu,0xc0u,0xc2u,0xc3u,0xc4u,0xc5u,0xc6u,0xc7u,0xcau,0xcbu,0xceu,0xd0u,0xd1u,0xd4u,0xd5u,0xd6u,0xdbu,0xdcu,0xdeu,0xa3u,0xa5u,0xaau,0xbcu,0xafu, 0 };
	char *pos;
	for (int i = 0; i<m_size; i++) {
		if (m_data[i] >= 'a' && m_data[i] <= 'z')
			m_data[i] -= 32;
		else if ((unsigned char)m_data[i] > 127) {
			pos = strchr((char*)&c_upperCase[0], m_data[i]);
			if (pos)
				m_data[i] = *(pos + sizeof(c_upperCase) / 2);
		}
	}
	return *this;
}

bool String::cpy2(char *dest, SizeType size)
{
	if (m_size < size) {
		memcpy(dest, m_data, m_size + 1);
		return true;
	}
	else {
		memcpy(dest, m_data, size - 1);
		*(dest + size - 1) = 0;
		return false;
	}
}

bool String::append(const char *src, SizeType size)
{
	if (c_maxSize - m_size >= size) {
		memcpy(m_data + m_size, src, size);
		m_size += size;
		*(m_data + m_size) = 0;
		return true;
	}
	else {
		memcpy(m_data + m_size, src, c_maxSize - m_size);
		m_size = c_maxSize;
		*(m_data + m_size) = 0;
		return false;
	}
}

SizeType String::PrepareLen(const char *src)
{
	size_t realLen = strlen(src);
	m_size = static_cast<SizeType>(realLen);
	if (realLen != m_size)
	{
		throw "String too long !";
	}
	return m_size + minSize;
}

char String::decodeUtf(unsigned char **src) {
	unsigned char znak = **src;
	if ((znak < 0xC2) || ((znak > 0xC5) && (znak != 0xC8) && (znak != 0xCB) && (znak != 0xE2))) {
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

void String::length(SizeType shorter)
{
	if (shorter < c_maxSize && m_data != nullptr)
	{
		m_size = shorter;
		m_data[m_size] = 0;
	}
}

String operator + (const String& _res, const String& _src) {
	String res;
	res.Reserve(_res.getMaxSize() + _src.getMaxSize());
	SizeType len = _res.length();
	char *dest = &res[0];
	memcpy(dest, &const_cast<char&>(_res[0]), len);
	dest += len;
	memcpy(dest, &const_cast<char&>(_src[0]), _src.length());
	len += _src.length();
	res.length(len);
	return res;
}

String operator + (const String& _res, const char* _src) {
	String res;
	size_t len2 = strlen(_src);
	SizeType len = _res.length();
	if (len2 + len > String::npos)
	{
		len2 = String::npos - len - 1;
	}
	res.Reserve(_res.getMaxSize() + len);
	char *dest = &res[0];
	memcpy(dest, &const_cast<char&>(_res[0]), len);
	dest += len;
	memcpy(dest, _src, len2);
	res.length(len + static_cast<SizeType>(len2));
	return res;
}

String operator + (const char* _src, const String& _res) {
	String res;
	size_t len2 = strlen(_src);
	SizeType len = _res.length();
	if (len2 + len > String::npos)
	{
		len2 = String::npos - len - 1;
	}
	res.Reserve(_res.getMaxSize() + len);
	char *dest = &res[0];
	memcpy(dest, _src, len2);
	dest += len2;
	memcpy(dest, &const_cast<char&>(_res[0]), len);
	res.length(len + static_cast<SizeType>(len2));
	return res;
}

String operator + (const String& _res, const char _src) {
	String res;
	memcpy(&res, &_res, &_res[0] - (char*)&_res + _res.length()); // copy w/o useless ending 0
	res += _src;
	return res;
}

String operator + (const char _src, const String& _res) {
	String res;
	SizeType len = _res.length();
	if (1 + len > String::npos)
	{
		len--;
	}
	res.Reserve(_res.getMaxSize() + len);
	char *dest = &res[0];
	*dest++ = _src;
	memcpy(dest, &const_cast<char&>(_res[0]), len);
	res.length(len + 1);
	return res;
}

std::ostream& operator << (std::ostream& os, const String& str) {
	os << str.c_str();
	return os;
}
