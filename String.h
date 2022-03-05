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

#ifndef CSTRING
#define CSTRING
#include <string.h>
#include <iostream>
#include <stdint.h>

typedef int16_t SizeType; // 15b size

class String {
public:
	String();
	explicit String(const String& src);
	String(const char* src);
	void Reserve(SizeType size);
	String& fromUtf(const unsigned char *src);

	String& operator =(const char* src);
	String& operator +=(const char *src);
	const String& operator +=(char src);
	const String& operator +=(int number);
	const String& operator +=(long number);
	const String& operator +=(long long number);
	String& operator +=(const String& src); // "strcat"
	String& operator =(const String& src);
	bool operator  <(String& nd) const;
	bool operator  >(String& nd) const;
	bool operator <=(String& nd) const;
	bool operator >=(String& nd) const;
	bool operator ==(String& nd) const;
	bool operator !=(String& nd) const;
	bool operator !=(const char* nd) const;
	bool operator <(char* nd) const;
	bool operator >(char* nd) const;
	bool operator ==(const char* nd) const;
	char& operator [] (const int pos);

	inline operator const char*() const 
	{
		return reinterpret_cast<const char*>(m_data);
	}
	String& num2str(long long number);
	inline const char *c_str() const { return reinterpret_cast<const char*>(m_data); }
	inline SizeType max_size() const { return c_maxSize; }
	SizeType find(int chr, SizeType pos = 0) const;
	SizeType find4test(const char *chr, SizeType pos = 0, SizeType len = -1) const;
	SizeType find(const char *chr, SizeType pos = 0) const;
	SizeType find(const String& str, SizeType pos = 0) const;
	String substr(SizeType pos = 0, SizeType len = -1) const;
	~String();
	String& upperCase();
	bool cpy2(char *dest, SizeType size);
	bool append(const char *src, SizeType size);

	SizeType getMaxSize() const { return c_maxSize; }
	SizeType length() const { return m_size; }
	void length(SizeType shorter);
	const static SizeType npos = (UINT64_MAX & ((1ui64 << (8 * sizeof(SizeType) - 1)) - 1));

protected:
	const SizeType c_maxSize;
	SizeType m_size;
	unsigned char *m_data;
private:
	char decodeUtf(unsigned char **src);
	SizeType PrepareLen(const char *src);
	const static uint8_t minSize = 7;
};
String operator +(const String& _res, const String& _src);
String operator +(const String& _res, const char* _src);
String operator +(const char* _src, const String& _res);
String operator +(const String& _res, const char _src);
String operator +(const char _src, const String& _res);
std::ostream& operator <<(std::ostream& os, const String& str);
#endif
