/**
 * @file Array2D.h
 *
 * @brief Development helper
 * @author Jan Elzein
 * @date: 8.4.2022
 *
 * Array2D<Type, rows, cols> twoDimensionalTrackedArray;
 * is alternative to
 * Type twoDimensionalTrackedArray[cols][rows];
 * but there is bounary check and overloads to track all reads/writes
 * should not be used it in production code (WIP)
 *
 * Array2D<int, 3, 4> twoDimensionalTrackedArray; // <=> int twoDimensionalTrackedArray[3][4];
 * TrackProp<int> *x = &twoDimensionalTrackedArray[i][j]; // pointer to or NULL if out of limits
 * int &testRef = twoDimensionalTrackedArray[i][j]._content; // reference of particular array element
 * printf("== %i\n", twoDimensionalTrackedArray[i][j]._content); // print element in case of default typedef int TrackedType (TrackPropStats.h)
 *
 * Tracked examples:
 * twoDimensionalTrackedArray[i][j] = j + i*twoDimensionalTrackedArray.Cols;
 *
 * twoDimensionalTrackedArray[0][0] += 100;
 *
 * MIND all arrays items need 1 item of static TrackPropStats::props (GetNextProp)
 *
 */
#ifndef ARRAY_2D_INCL
#define ARRAY_2D_INCL
#ifdef _DEBUG
#ifndef QT_SIMULATION
#define TRACK_PROP_ACTIVATED
#endif  // !QT_SIMULATION
#endif  // _DEBUG
#include "TrackPropMacros.h"
#include "TrackProp.h"
#include <stdio.h>
#include <new>

#ifdef __cplusplus

template <typename T, int cols>
class ArrayRows
{
public:
	const size_t Cols;

	ArrayRows():
		Cols(cols)
	{
	}

	TrackProp<T> &operator[](size_t i)
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

private:
	TrackProp<T> me[cols];
};

template <typename T, int rows, int cols>
class Array2D
{
public:
	const char *Name;
	const size_t Rows;
	const size_t Cols;

	Array2D(const char *Name):
		Rows(rows),
		Cols(cols),
		Name(Name)
	{
		for (size_t i = 0; i < Rows; i++)
		{
			for (size_t j = 0; j < Cols; j++)
			{
				TrackProp<T> &ref = props[i][j];
				new (&ref) TrackProp<T>(Name);
				ref.id = i * Cols + j;
			}
		}
	}

	~Array2D() {}

	ArrayRows<T, cols>(&operator[](size_t i))
	{
		if (i < rows)
		{
			return props[i];
		}
		else
		{
			ArrayRows<T, cols> *ret = NULL;
			printf("Out of bounds !\n");
			return *ret;
		}
	}

private:
	ArrayRows<T, cols> props[rows];
};
#endif  // __cplusplus
#endif  // !ARRAY_2D_INCL
