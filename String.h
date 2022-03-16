#define DEBUG
#define STRDATA_SIZE_LIMIT
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
#include <stdio.h>
#include <stdlib.h>  // for malloc itself
#include <string.h>
#include <iostream>
#include <stdint.h>

#ifdef DEBUG
#define STR_LOG printf
#else
#define STR_LOG(s) s
#endif

typedef int16_t SizeType; // 15b size

class StrData
{
public:
	typedef int RefType;
	const static uint8_t minSize = 7;
private:
#pragma pack(1)
	struct RefsSize_t
	{
		RefType refs;
		SizeType size;
#if defined(DEBUG) || defined(STRDATA_SIZE_LIMIT)
		SizeType maxSize;
#endif // DEBUG || STRDATA_SIZE_LIMIT
	};
#pragma pack()
public:
	StrData(const char *src);
	StrData(SizeType size = 0, SizeType strLen = 0);

// https://stackoverflow.com/questions/1281686/determine-size-of-dynamically-allocated-memory-in-c/48612539#48612539
#ifdef __linux__
#include <malloc.h>
	size_t GetSize(char *ptr) const {
		malloc_usable_size(ptr - sizeof(RefsSize_t)) - sizeof(RefsSize_t);
	}
	inline void SetMaxSize(size_t size) {};
#elif defined(__APPLE__)
#include <malloc/malloc.h>
	size_t GetSize(char *ptr) const {
		malloc_size(ptr - sizeof(RefsSize_t)) - sizeof(RefsSize_t);
	}
	inline void SetMaxSize(size_t size) {};
#elif defined(_WIN32)
#include <malloc.h>
	size_t GetSize(char *ptr) const {
		return _msize(ptr - sizeof(RefsSize_t)) - sizeof(RefsSize_t);
	}
	inline void SetMaxSize(SizeType size)
	{
#if defined(DEBUG) || defined(STRDATA_SIZE_LIMIT)
		if (mem != nullptr)
		{
			RefsSize_t *internals = reinterpret_cast<RefsSize_t*>(mem) - 1;
			internals->maxSize = size;
		}
#endif // DEBUG) || STRDATA_SIZE_LIMIT
	};
#else
private:
	c_MaxSize;
	inline void SetMaxSize(size_t size) { c_MaxSize = size };
	inline SizeType GetSize(char *ptr) { return c_MaxSize; };
#endif
public:
	SizeType MaxSize() const;
	void Assign(const StrData& src);
	char *mem; // (c_MaxSize); *{references, size, content}
#ifdef DEBUG
	RefsSize_t *refsSize;
#endif // DEBUG
	SizeType Size() const;
	SizeType RemainingSize() const;
	void AddSize(SizeType plus);
	void SetSize(SizeType newSize);

	StrData::RefType AddRef();
	RefType ReleaseRef();
	~StrData();
	//inline operator char*() const
	//{
	//	return mem;
	//}
	char &operator[](int pos);
	unsigned char& operator&(int pos);
	void Realloc(SizeType size);
	void SetData(const char *src, SizeType limit = -1);
	void AddData(const char *src, SizeType pos = -1);
};

class String {
public:
	String();
	String(SizeType bufSize);
	String(const String& src);
	String(const char* src);
	void Reserve(SizeType size);
	String& fromUtf(const unsigned char *src);
	String to_UTF8();

	String& operator =(const char* src);
	String& operator +=(const char *src);
	const String& operator +=(char src);
	inline const String& operator +=(int number) { return operator+=(static_cast<long long>(number)); }
	inline const String& operator +=(long number) { return operator+=(static_cast<long long>(number)); }
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

	inline operator const void*() const { return m_data.mem; }
	inline operator const StrData&() const { return m_data; }
	String num2str(long long number);
	inline const char *c_str() const { return m_data.mem; }
	inline SizeType max_size() const { return m_data.MaxSize(); }
	SizeType find(int chr, SizeType pos = 0) const;
	SizeType find(const char *chr, SizeType pos = 0) const;
	SizeType find(const String& str, SizeType pos = 0) const;
	String substr(SizeType pos = 0, SizeType len = -1);
	~String();
	String& upperCase();
	bool CopyTo(char *dest, SizeType size);
	void append(const char *src);

	SizeType length() const { return m_data.Size(); }
	void length(SizeType shorter);
	const static SizeType npos = (UINT64_MAX & ((1ui64 << (8 * sizeof(SizeType) - 1)) - 1));

	static char JoinChar;
protected:
	StrData m_data;
private:
	char decodeUtf(unsigned char **src);
	void encodeUtf(unsigned char *dest, unsigned char *src, int16_t &maxSize);
};
String operator +(const String& _res, const String& _src);
String operator +(const String& _res, const char* _src);
String operator +(const char* _src, const String& _res);
String operator +(const String& _res, const char _src);
String operator +(const char _src, const String& _res);
std::ostream& operator <<(std::ostream& os, const String& str);
#endif
