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
	return(uint64_t)(t.QuadPart / (double) freq.QuadPart * MICROSECOND);
}

extern double x86(size_t n, double* vector1,double* vector2);
extern double AVX2_XMM(size_t n, double* vector1, double* vector2);
extern double AVX2_YMM(size_t n, double* vector1, double* vector2);

double C_Kernel(size_t n, double* vector1, double* vector2)
{
	double result = 0.0;
	for (int i = 0; i < n; ++i)
	{
		result += vector1[i] * vector2[i];
	}

	return result;
}

int main() {
	// variables
	double result = 0.0;
	double CRes = 0.0; // C result, will be the reference for error checking

	// array size and bytes required
	const size_t pow = 20;
	const size_t ARRAY_SIZE = 1ULL << pow; // 1ULL for explicit 64-bit shift to suppress 32-bit implicitly converted to 64-bit compiler warning
	const size_t ARRAY_BYTES = ARRAY_SIZE * sizeof(double);

	// array declaration
	double* vec1 = (double*)malloc(ARRAY_BYTES);
	double* vec2 = (double*)malloc(ARRAY_BYTES);

	// time related vars
	uint64_t startTime;
	uint64_t endTime;
	uint64_t totalTime;

	// number of execution per kernel
	size_t numExec = 50;
	size_t cacheInit = 3;

	// err count
	size_t errCount = 0;


	// initialize array values
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		vec1[i] = 2.0;
		vec2[i] = 1.0;
	}

	//--------------------------- C Kernel ---------------------------//

	printf("Running C Program...\n\n");

	// initialize cache
	if (cacheInit) {
		for (size_t i = 0; i < cacheInit; ++i) {
			C_Kernel(ARRAY_SIZE, vec1, vec2);
		}
	}

	startTime = GetStopWatch();
	for (size_t i = 0; i < numExec; ++i) {
		result = C_Kernel(ARRAY_SIZE, vec1, vec2);
	}
	endTime = GetStopWatch();
	totalTime = (endTime - startTime) / numExec;
	
	// initialization of C kernel result
	CRes = result;

	printf("Result : %lf\n", result);
	printf("Execution time: %lldus\n", totalTime);

	//--------------------------- x86 Kernel ---------------------------//

	printf("\n\nRunning x86 Program...\n\n");
	// initialize cache
	if (cacheInit) {
		for (size_t i = 0; i < cacheInit; ++i) {
			x86(ARRAY_SIZE, vec1, vec2);
		}
	}

	startTime = GetStopWatch();
	for (size_t i = 0; i < numExec; ++i) {
		result = x86(ARRAY_SIZE, vec1, vec2);
	}
	endTime = GetStopWatch();
	totalTime = (endTime - startTime) / numExec;

	printf("Result : %lf\n", result);
	printf("Execution time: %lldus\n", totalTime);
	if (result != CRes) {
		errCount++;
		printf("x86 program is incorrect\n");
	} else {
		printf("x86 program is correct\n");
	}

	//--------------------------- AVX2_XMM Kernel ---------------------------//

	printf("\n\nRunning AVX2_XMM Program...\n\n");
	// initialize cache
	if (cacheInit) {
		for (size_t i = 0; i < cacheInit; ++i) {
			AVX2_XMM(ARRAY_SIZE, vec1, vec2);
		}
	}

	startTime = GetStopWatch();
	for (size_t i = 0; i < numExec; ++i) {
		result = AVX2_XMM(ARRAY_SIZE, vec1, vec2);
	}
	endTime = GetStopWatch();
	totalTime = (endTime - startTime) / numExec;


	printf("Result : %lf\n", result);
	printf("Execution time: %lldus\n", totalTime);
	if (result != CRes) {
		errCount++;
		printf("AVX2_XMM program is incorrect\n");
	} else {
		printf("AVX2_XMM program is correct\n");
	}

	//--------------------------- AVX2_YMM Kernel ---------------------------//

	printf("\n\nRunning AVX2_YMM Program...\n\n");
	// initialize cache
	if (cacheInit) {
		for (size_t i = 0; i < cacheInit; ++i) {
			AVX2_YMM(ARRAY_SIZE, vec1, vec2);
		}
	}

	startTime = GetStopWatch();
	for (size_t i = 0; i < numExec; ++i) {
		result = AVX2_YMM(ARRAY_SIZE, vec1, vec2);
	}
	endTime = GetStopWatch();
	totalTime = (endTime - startTime) / numExec;

	printf("Result : %lf\n\n", result);
	printf("Execution time: %lldus\n", totalTime);

	if (result != CRes) {
		errCount++;
		printf("AVX2_YMM program is incorrect\n\n");
	} else {
		printf("AVX2_YMM program is correct\n\n");
	}

	printf("Total Error Count: %lld\n\n", errCount);

	// free arrays
	free(vec1);
	free(vec2);

	return 0;
}