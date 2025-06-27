#include "TrackProp.h"
#include "Array1D.h"
#include "Array2D.h"

int main()
{
	const char* names[] = { "one", "two", "three" };
//*
	Array1D<int, 3> simpleArray(names); /*/
	Array1D<int, 3> simpleArray(nullptr); // */
	simpleArray[0] = 1;
	simpleArray[0] > 0;
	simpleArray[0] = 0;
	simpleArray[1] = 2;
	simpleArray[1] = 1; // two/1 lost change
	simpleArray[2] = 3;
	Array2D<int, 2, 3> twoDarray("2D");
	for (size_t i = 0; i < twoDarray.Rows; i++) {
		for (size_t j = 0; j < twoDarray.Cols; j++) {
			twoDarray[i][j] = 5 - i * twoDarray.Cols - j;
		}
	}
	twoDarray[0][0] = 1;
	TRACK_PROP(int, pod);
	pod = 0; // 2D lost change
	pod = 1; // pod lost change
	pod > 0; // (reset)
	pod = 2;
	pod = 3; // "pod" lost change
}
