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
	int CKernelTime = 0;

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
	size_t numExec = 30;
	size_t cacheInit = 5;


	// initialize array values
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		vec1[i] = 5.0;
		vec2[i] = 5.0;
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
	CKernelTime = totalTime;

	printf("Result : %lf\n\n", result);
	printf("Execution time: %lldns\n", totalTime);

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

	printf("Result : %lf\n\n", result);
	printf("Execution time: %lldns\n", totalTime);
	printf("Speed Difference with C Kernel: %lld\n", (totalTime - CKernelTime));

	//--------------------------- AVX1 Kernel ---------------------------//

	printf("\n\nRunning AVX1 Program...\n\n");
	// initialize cache
	if (cacheInit) {
		for (size_t i = 0; i < cacheInit; ++i) {
			AVX1(ARRAY_SIZE, vec1, vec2);
		}
	}

	startTime = GetStopWatch();
	for (size_t i = 0; i < numExec; ++i) {
		result = AVX1(ARRAY_SIZE, vec1, vec2);
	}
	endTime = GetStopWatch();
	totalTime = (endTime - startTime) / numExec;

	printf("Result : %lf\n\n", result);
	printf("Execution time: %lldns\n", totalTime);
	printf("Speed Difference with C Kernel: %lld\n", (totalTime - CKernelTime));

	//--------------------------- AVX2 Kernel ---------------------------//

	printf("\n\nRunning AVX2 Program...\n\n");
	// initialize cache
	if (cacheInit) {
		for (size_t i = 0; i < cacheInit; ++i) {
			AVX2(ARRAY_SIZE, vec1, vec2);
		}
	}

	startTime = GetStopWatch();
	for (size_t i = 0; i < numExec; ++i) {
		result = AVX2(ARRAY_SIZE, vec1, vec2);
	}
	endTime = GetStopWatch();
	totalTime = (endTime - startTime) / numExec;

	printf("Result : %lf\n\n", result);
	printf("Execution time: %lldns\n", totalTime);
	printf("Speed Difference with C Kernel: %lld\n", (totalTime - CKernelTime));

	// free arrays
	free(vec1);
	free(vec2);

	return 0;
}