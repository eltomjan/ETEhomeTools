/**
 * @file Array1D.h
 *
 * @brief Development helper
 * @author Jan Elzein
 * @date: 8.4.2022
 *
 * Array1D<Type, size> simpleArray;
 * is alternative to
 * Type simpleArray[size];
 * but there is bounary check and overloads to track all reads/writes
 *
 * Array1D<int, 5> simpleArray; // <=> int simpleArray[5];
 * simpleArray[4] = 5; // this will be tracked
 *
 * MIND all arrays items need 1 item of static TrackPropStats::props (GetNextProp)
 *
 */
#ifndef ARRAY_1D_INCL
#define ARRAY_1D_INCL
#ifdef _DEBUG
#ifndef QT_SIMULATION
#define TRACK_PROP_ACTIVATED
#endif  // !QT_SIMULATION
#endif  // _DEBUG
#include "TrackPropMacros.h"
#include "TrackProp.h"

#ifdef __cplusplus

template <typename T, int cols>
class Array1D
{
public:
	const size_t Cols;
	Array1D(const char **elements);
	TrackProp<T> &operator[](size_t i);

private:
	TrackProp<T> me[cols];
};
#endif  // __cplusplus
#endif  // !ARRAY_1D_INCL
