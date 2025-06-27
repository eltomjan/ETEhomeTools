/**
 * @file TrackProp.h
 *
 * @brief Development helper
 * @author El Tom Jan Elzein
 * @date: 8.4.2022
 *
 * could be used thru TRACK_PROP from TrackPropMacros.h
 * to track all reads/writes to particular type (enum, int)
 * must fit to TrackedType defined in TrackPropStats.h
 * should not be used it in production code (WIP)
 *
 * Example enum declaration and overloads (some_sequence.cpp)
 *
 * #ifndef ENUM_ITEMS
 * ...
 * typedef enum SOME_STEP
 * {
 * #ifndef ENUM_ITEM
 * #define ENUM_ITEM(i) i
 * #endif // !ENUM_ITEM
 * #endif // !ENUM_ITEMS trick: ENUM_ITEM(i) = ENUM_ITEMS ? #i : i
 *	ENUM_ITEM(SR_NOT_ACTIVE),
 *	...
 *	ENUM_ITEM(SR_CONTINUE_CYCLE)
 * #ifndef ENUM_ITEMS
 * } some_step_t;
 * ...
 * #endif // !ENUM_ITEMS
 *
 * // Concrete type assignment overloads example
 * #define ENUM_ITEMS
 * #define ENUM_ITEM(i) i
 * enum SOME_STEP
 * {
 * #include "../Operations/some_sequence.cpp"
 * };
 * #undef ENUM_ITEM
 * template<>
 * SOME_STEP TrackProp<SOME_STEP>::operator =(SOME_STEP right)
 * {
 * 	me.calls++;
 * 	if (_content != right || me.sets < 1) // ... || first write
 * 	{
 * 		_prev = _content;
 * 		TrackPropStats::LogEnum(me.Name, _content, right);
 * 		TrackPropStats::BeforeChange(me, static_cast<TrackedType>(_content), right);
 * 		_content = right;
 * 	}
 * 	return _content;
 * }
 *
 * template<>
 * TrackProp<SOME_STEP>::operator SOME_STEP()
 * {
 * 	TrackPropStats::UpdateReadStats(me, _content);
 * 	return _content;
 * }
 *
 * // Concrete enum log method example
 * void TrackPropStats::LogEnum(const char *name, TrackedType o, TrackedType n)
 * {
 * 	static const char* someStepStrs[] =
 * 	{
 * #define ENUM_ITEM(i) #i
 * #include "../Operations/some_sequence.cpp"
 * // Include full file with defined ENUM_ITEMS => get enum items without code around
 * 	};
 * 	int max = sizeof(someStepStrs) / sizeof(char*);
 * 	if (o > max || n > max)
 * 	{
 * 		__debugbreak();
 * 	}
 * 	DebugToolsNs::DebugTools::ShowTime(false);
 * 	LOG_WRITE_INFO(TRACK, "|%s|%s->%s|%u|\n", name, someStepStrs[o], someStepStrs[n], TrackPropStats::writeNo);
 * 	DebugToolsNs::DebugTools::ShowTime(true);
 * }
 *
 * Next examples:
 * - src\AppLayer\Common\TrackPropChild.cpp (not included in project)
 * - https://stackoverflow.com/questions/207976/how-to-easily-map-c-enums-to-strings/69505404#69505404
 *
 * inputs_outputs_maps example:
 * ----------------------------
 * namespace
 * {
 * 	// const char *outFuncs[256] = { "OF_INLET1",...
 * }
 * ...
 * #include "Array1D.h"
 * ...
 * extern Array1D<bool, OF_MAXFUNC> FuncOut;
 * ...
 * *const_cast<char**>(&outFuncs[255]) = "OF_NONE"; in load_all_IO_maps(void)
 *
 * Another Process example changes:
 * ------------------------------
 * TRACK_PROP_EXTERN(another_step_t, DnotherStep); // all extern refs needs to be changed
 * ...
 * #include "TrackProp.h"
 * ...
 * TRACK_PROP_SET(another_step_t, AnotherStep, SD_NOTACTIVE);
 * ...
 * template<>
 * another_step_t TrackProp<another_step_t>::operator =(another_step_t right)
 * {
 * 	const char *steps[] = { "SD_NOTACTIVE", "SD_START",... , "SD_REDIST_7" }; another_step_t
 *prev = _content; TrackPropStats::TrackAssignment(static_cast<TrackedType>(_content), me, static_cast<TrackedType>(right), steps); _prev = prev; return
 *_content;
 * }
 * template<>
 * TrackProp<another_step_t>::operator another_step_t()
 * {
 * 	TrackPropStats::UpdateReadStats(me, _content);
 * 	return _content;
 * }
 */
#ifndef TRACK_PROP_INCL
#define TRACK_PROP_INCL
#ifdef _DEBUG
#ifndef QT_SIMULATION
#define TRACK_PROP_ACTIVATED
#endif  // !QT_SIMULATION
#endif  // _DEBUG
#include "TrackPropMacros.h"
#include <stdint.h>
#include <memory.h>

#ifdef __cplusplus

typedef unsigned long TrackType;
class TrackPropBase {
public:
	TrackType _content, _prev;
	int64_t reads, sets, calls, prevRead, readChanges;
	const char* Name;
	int id;
	void UpdateReadStats();
	void TrackAssignment(TrackType right);
	void BeforeChange(TrackType after);

protected:
	uint8_t _validity;
	enum uint8_t {
		E_UNSET = 0,
		E_CONTENT_VALID = 1,
		E_PREV_VALID = 2,
		E_PREV_READ_VALID = 4
	};
};

template <typename T>
class TrackProp : public TrackPropBase
{
public:
	// Construct uninitialized (array item)
	TrackProp()
	{
		Name = nullptr;
		sets = -1;
		memset(&_content, 0, sizeof(T));
		memset(&_prev, 0, sizeof(T));
		id = -1;
		reads = 0;
		calls = 0;
		prevRead = 0;
		readChanges = 0;
		_validity = E_UNSET;
	}

	// Construct with name
	TrackProp(const char* name) : TrackProp()
	{
		Name = name;
	}

	// Construct silent with parent
	TrackProp(int id, const char* name, T val) : TrackProp()
	{
		Name = name;
		_content = reinterpret_cast<T&>(val);
		_prev = val;
		_validity = E_CONTENT_VALID;
		readChanges = sets = 1;
		this->id = id;
	}

	// Construct with value
	TrackProp(const char* name, T val) : TrackProp()
	{
		Name = name;
		_content = reinterpret_cast<T&>(val);
		_prev = val;
		_validity = E_CONTENT_VALID;
		sets = 0;
		id = -1;
	}

	// Specific TrackProp writes
	TrackProp<T>& operator=(T right) {
		TrackAssignment(static_cast<T>(right));
		return *this;
	}

	// All TrackProp reads
	operator T() const {
		const_cast<TrackProp<T>*>(this)->UpdateReadStats();
		return _content;
	}

	T operator++();
	T operator++(int);
	T operator--();
	T operator--(int);

	// Compound assignment operators
	TrackProp<T>& operator+=(const T& value);
	TrackProp<T>& operator-=(const T& value);
	TrackProp<T>& operator*=(const T& value);
	TrackProp<T>& operator/=(const T& value);
	TrackProp<T>& operator%=(const T& value);

	/* Enums */
	template <typename E>
	bool operator==(E right) const;
};
template <typename Stream, typename T>
Stream & operator>>(Stream & stream, TrackProp<T> & obj);
#endif  // __cplusplus
#endif  // !TRACK_PROP_INCL
