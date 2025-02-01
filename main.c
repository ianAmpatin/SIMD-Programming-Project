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

extern double x86(size_t n, double* vector1,double* vector2);
extern double AVX1(size_t n, double* vector1, double* vector2);
extern double AVX2(size_t n, double* vector1, double* vector2);


double C_Implementation(size_t n, double* vector1, double* vector2)
{
	return 0.0;
}

int main() {
	// variables
	double result = 0.0;

	// array size and bytes required
	const size_t ARRAY_SIZE = 1 << 2;
	const size_t ARRAY_BYTES = ARRAY_SIZE * sizeof(double);

	// array declaration
	double* vec1 = (double*)malloc(ARRAY_BYTES);
	double* vec2 = (double*)malloc(ARRAY_BYTES);

	// initialize array values
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		vec1[i] = 2.0;
		vec2[i] = 2.0;
	}

	// print array values
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		printf("Array 1 : %lf  ||  Array 2 : %lf\n", vec1[i], vec2[i]);
	}

	//--------------------------- C Program ---------------------------//

	//--------------------------- x86 Program ---------------------------//

	printf("Running x86 Program...\n\n");
	result = x86(ARRAY_SIZE, vec1, vec2);
	printf("Result : %lf", result);

	//--------------------------- AVX1 Program ---------------------------//

	//--------------------------- AVX2 Program ---------------------------//


	// free arrays
	free(vec1);
	free(vec2);

	return 0;
}