#ifndef HEAP_ARRAY
#define HEAP_ARRAY

#include <string.h>

#include <cstddef>

template <typename T, const size_t size>
class HeapArray
{
public:
	HeapArray():
		heap(*reinterpret_cast<T (*)[size]>(new T[size])),
		count(size)
	{
		memset(&heap, 0, size);
	}

	T& operator[](size_t index) const
	{
		if (index >= size)
		{
			__debugbreak();
		}
		return heap[index];
	}

	~HeapArray() { delete[] (&heap); }

private:
	T (&heap)[size];
	const size_t count;
};

#endif  // HEAP_ARRAY
