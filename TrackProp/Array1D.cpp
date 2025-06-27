/**
 * @file Array1D.h
 *
 * @brief Development helper
 * @author Elzein
 * @date: 8.4.2022
 *
 * Array1D<Type, size> simpleArray;
 * is alternative to
 * Type simpleArray[size];
 * but there is bounary check and overloads to track all reads/writes
 * should not be used it in production code (WIP)
 *
 * Array1D<int, 5> simpleArray; // <=> int simpleArray[5];
 * simpleArray[4] = 5; // this will be tracked
 *
 * MIND all arrays items need 1 item of static TrackPropStats::props (GetNextProp)
 *
 */
#include "Array1D.h"
#include <stdio.h>
#include <new>

template <typename T, int cols>
Array1D<T, cols>::Array1D(const char **elements):
	Cols(cols)
//	matrix()
{
	for (size_t i = 0; i < Cols; i++)  // re-create/construct invalid default TrackPropRef() objects
	{
		new (&me[i]) TrackProp<T>(elements ? elements[i] : NULL);
		if (!elements) {
			me[i].id = i;
		}
	}
}

template <typename T, int cols>
TrackProp<T> &Array1D<T, cols>::operator[](size_t i)
{
	if (i < cols)
	{
		return me[i];
	}
	else
	{
		printf("Out of bounds !\n");
		TrackProp<T> *crash = NULL;
		return *crash;
	}
}

// Array types used
template class Array1D<int, 3>;
