/**
 * @file TrackPropStats.cpp
 *
 * @brief Development helper
 * @author El Tom Jan Elzein
 * @date: 8.4.2022
 *
 * TrackProp(s) statistic class + demo overloads
 * should not be used it in production code (WIP)
 */

#include <limits.h>
#include <stdio.h>

#include "TrackProp.h"

static unsigned writeNo = 0;

#ifdef _DEBUG
#include <stdarg.h>
#include <strsafe.h>

#include <windows.h>
extern void DBGOUTPUT(const char* kwszDebugFormatString, ...) \
{
	int cbFormatString = 98;
	va_list args;
	static char wszDebugString[100];

	va_start(args, kwszDebugFormatString);

	/* Populate the buffer with the contents of the format string. */
	StringCbVPrintfA(&wszDebugString[0], cbFormatString, kwszDebugFormatString, args);

	OutputDebugStringA(wszDebugString);
	printf(wszDebugString);

	va_end(args);
}
#else
#define DBGOUTPUT( kwszDebugFormatString, ... );
#endif

#define LOG_WRITE DBGOUTPUT

void TrackPropBase::UpdateReadStats()
{
	if (sets < 0)
	{
		DBGOUTPUT("Uninitialized %s read !!!\n", Name);
		// __debugbreak();
	}

	if (prevRead != static_cast<int64_t>(_content) || sets < 1)
	{
		readChanges++;
	}
	if (readChanges < sets)
	{
		DBGOUTPUT("%s lost Set (sets - readChanges) = %lli !\n", Name, sets - readChanges);
		sets = readChanges;
	}
	reads++;
	prevRead = _content;
}

void TrackPropBase::TrackAssignment(TrackType right)
{
	calls++;
	if (_content != right || sets < 1)  // ... || first write
	{
		if (id >= 0) {
			DBGOUTPUT("|%i|%s|%i->%i|%u|\n", id, Name ? Name : "-", _content, right, writeNo);
		} else {
			DBGOUTPUT("|-|%s|%i->%i|%u|\n", Name ? Name : "-", _content, right, writeNo);
		}
		BeforeChange(right);
		_prev = _content;
		_content = right;
		_validity = static_cast<uint8_t>(_validity | (E_CONTENT_VALID + E_PREV_READ_VALID));
		sets++;
	}
}

void TrackPropBase::BeforeChange(TrackType after)
{
	static uint64_t visits = 0;
	static TrackType breakSettingValue = static_cast<TrackType>(INT_MAX);  // break at n-th time set
	static int64_t breakChangeNo = INT64_MAX;        // break at n-th change
	static int64_t breakSetNo = INT64_MAX;           // break at n-th set
	static uint64_t whenVisiting = 0;                // break at n-th run
	if (++visits == whenVisiting)
	{
		__debugbreak();
	}
	sets++;
	if ((static_cast<int64_t>(breakSettingValue) != ULONG_MAX && after == breakSettingValue) || (breakChangeNo != ULONG_MAX && breakChangeNo == sets) ||
		(breakSetNo != ULONG_MAX && breakSetNo == calls))
	{
		__debugbreak();
	}
	if (_validity & E_CONTENT_VALID) {
		if (sets - readChanges > 0 && (_content != after))
		{
			if (Name) {
				LOG_WRITE("%s lost change\n", Name);
			} else {
				LOG_WRITE("%i lost change\n", id);
			}
			readChanges = sets + 1;
		}
	} else {
		readChanges++;
	}
	writeNo++;
}

template <typename T>
T TrackProp<T>::operator++()
{
	UpdateReadStats(_content);
	T next = _content + 1;
	if (!Name)
	{
		LOG_WRITE("|arr|%i->%i|%u|\n", _prev, next, writeNo - 1);
	} else {
		LOG_WRITE("|%i|%s|++%i|%u|\n", id, Name, _content, writeNo);
	}
	BeforeChange(next);
	_prev = ++_content;
	return _content;
}

template <typename T>
T TrackProp<T>::operator++(int)
{
	UpdateReadStats(_content);
	T next = _content + 1;
	if (!Name)
	{
		LOG_WRITE("|arr|%i->%i|%u|\n", _prev, next, writeNo - 1);
	} else {
		LOG_WRITE("|%i|%s|%i++|%u|\n", id, Name, _content, writeNo);
	}
	BeforeChange(next);
	_prev = next;
	return _content++;
}

template <typename T>
T TrackProp<T>::operator--()
{
	UpdateReadStats(_content);
	T next = _content - 1;
	if (!Name)
	{
		LOG_WRITE("|arr|%i->%i|%u|\n", _prev, next, writeNo - 1);
	} else {
		LOG_WRITE("|%i|%s|--%i|%u|\n", id, Name, _content, writeNo);
	}
	BeforeChange(next);
	_prev = next;
	_content = _content - 1;
	return _content;
}

template <typename T>
T TrackProp<T>::operator--(int)
{
	UpdateReadStats(_content);
	T next = _content - 1;
	if (!Name)
	{
		LOG_WRITE("|arr|%i->%i|%u|\n", _prev, next, writeNo - 1);
	} else {
		LOG_WRITE("|%i|%s|%i--|%u|\n", id, Name, _content, writeNo);
	}
	BeforeChange(next);
	_prev = next;
	_content = _content - 1;
	return _content;
}

template <typename T>
TrackProp<T>& TrackProp<T>::operator+=(const T& right) {
	UpdateReadStats(_content);
	T next = _content + right;
	if (!Name)
	{
		LOG_WRITE("|arr|%i->%i|%u|\n", _prev, next, writeNo - 1);
	} else {
		LOG_WRITE("|%i|%s|%i->%i|%u|\n", id, Name, _content, next, writeNo);
	}
	BeforeChange(next);
	_prev = _content;
	_content += right;
	return *this;
}

template <typename T>
TrackProp<T>& TrackProp<T>::operator-=(const T& right) {
	UpdateReadStats(_content);
	T next = _content - right;
	if (!Name)
	{
		LOG_WRITE("|arr|%i->%i|%u|\n", _prev, next, writeNo - 1);
	} else {
		LOG_WRITE("|%i|%s|%i->%i|%u|\n", id, Name, _content, next, writeNo);
	}
	BeforeChange(next);
	_prev = _content;
	_content = next;
	return *this;
}

template <typename T>
TrackProp<T>& TrackProp<T>::operator*=(const T& right) {
	UpdateReadStats(_content);
	T next = _content * right;
	if (!Name)
	{
		LOG_WRITE("|arr|%i->%i|%u|\n", _prev, next, writeNo - 1);
	} else {
		LOG_WRITE("|%i|%s|%i->%i|%u|\n", id, Name, _content, next, writeNo);
	}
	BeforeChange(_content * right);
	_prev = _content;
	_content = next;
	return *this;
}

template <typename T>
TrackProp<T>& TrackProp<T>::operator/=(const T& right) {
	UpdateReadStats(_content);
	if (right != 0) {
		T next = _content / right;
		BeforeChange(next);
		if (!Name)
		{
			LOG_WRITE("|arr|%i->%i|%u|\n", _prev, next, writeNo - 1);
		} else {
			LOG_WRITE("|%i|%s|%i->%i|%u|\n", id, Name, _content, next, writeNo);
		}
		_prev = _content;
		_content = next;
	}
	return *this;
}

template <typename T>
TrackProp<T>& TrackProp<T>::operator%=(const T& right) {
	UpdateReadStats(_content);
	if (right != 0) {
		T next = _content % right;
		BeforeChange(next);
		if (!Name)
		{
			LOG_WRITE("|arr|%i->%i|%u|\n", _prev, next, writeNo - 1);
		} else {
			LOG_WRITE("|%i|%s|%i->%i|%u|\n", id, Name, _content, next, writeNo);
		}
		_prev = _content;
		_content = next;
	}
	return *this;
}

template<typename T>
template<typename E>
bool TrackProp<T>::operator==(E right) const
{
	const_cast<TrackProp<T>*>(this)->UpdateReadStats(_content);
	return _content == right;
}
