#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include <time.h>

#define STOPWATCH_TICKS_PER_US 1
#define MICROSECOND 1000000
#define NANOSECOND 1000000000

uint64_t GetStopWatch()
{
	LARGE_INTEGER t, freq;

	QueryPerformanceCounter(&t);
	QueryPerformanceFrequency(&freq);
	return(uint64_t)(t.QuadPart / (double) freq.QuadPart * NANOSECOND);
}

extern void x86SIMD(size_t n, double* vector1,double* vector2);

double C_Implementation(size_t n, double* vector1, double* vector2)
{
	return 0.0;
}

int main() {
	// variables
	double result;

	// array size and bytes required
	const size_t ARRAY_SIZE = 1 << 24;
	const size_t ARRAY_BYTES = ARRAY_SIZE * sizeof(double);

	// array declaration
	double* vec1 = (double*)malloc(ARRAY_BYTES);
	double* vec2 = (double*)malloc(ARRAY_BYTES);

	//--------------------------- C Program ---------------------------//

	//--------------------------- AVX2 Program ---------------------------//
	return 0;
}