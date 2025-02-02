# SIMD Programming Project
This is an implementation of a dot product kernel using four different approaches:
1. C Program – A standard implementation in C.
2. x86-64 Assembly Language 
3. x86 SIMD AVX2 (XMM Registers) 
4. x86 SIMD AVX2 (YMM Registers) 

The kernel computes the dot product of two vectors, A and B, both represented as double-precision floating-point arrays. The operation follows the formula:

![image](https://github.com/user-attachments/assets/b78fa56c-2bfe-4d15-aed9-3d40b6efd9ee)

### Input
- n: An integer representing the length of vectors A and B.
-  A, B: Vectors containing double-precision floating-point values.

### Output
- Store the computed sdot value in memory.
- Display the result on the console for all implementations.




## Program Output
### A. Program Output with Execution Times

#### 1. Release <br />
##### I. 2^20

![image](https://github.com/user-attachments/assets/30ca8fb3-e4e4-4e43-a232-1ac3892525c5)

##### II. 2^26

![image](https://github.com/user-attachments/assets/1f404b04-cf70-4118-9387-d5eb05b4f4c8)

##### III. 2^28

![image](https://github.com/user-attachments/assets/8fe60eb6-f396-4b85-acfe-1236808087b0)


##### IV. 2^29

![image](https://github.com/user-attachments/assets/37dce62f-2094-436d-9ec1-b552e5d38afd)


##### V. 2^30

![image](https://github.com/user-attachments/assets/3ecc4d8a-c0b9-419e-ab3d-d8bdf67c1ccb)



#### 2. Debug <br />
##### I. 2^20

![image](https://github.com/user-attachments/assets/f08fa98b-35b8-4a46-a009-bce0b7162641)

##### II. 2^26


![image](https://github.com/user-attachments/assets/bb4f4b7e-930c-45fb-a739-008c1df6b73f)


##### III. 2^28

![image](https://github.com/user-attachments/assets/d1260fc4-0576-414c-9a12-7fba40e4b5ac)



##### IV. 2^29

![image](https://github.com/user-attachments/assets/efd5f5b3-05e3-4c11-8326-fa5f7ce7fce2)


##### V. 2^30

![image](https://github.com/user-attachments/assets/ea621325-9902-4741-8d4c-a8eda5c93fd6)


## Execution Time Table
### 1. Average Execution Time
The following tables contains the average execution time (in microseconds) for different implementations  measured in both Debug and Release modes across various vector sizes.

![image](https://github.com/user-attachments/assets/3b5f2003-f502-49a4-9ce6-818db5970602)

Based on the table 
- C is the least efficient with having longer execution times as the vector sizes increase
- x86 is faster than C but still slower than AVX2
- AVX2 using XMM registers performed a notable decrease in execution time compared to the first two implementations.
- AVX2 using YMM registers has the best results out of all the registers<br />
*Will be further explained at the performance analysis part*
## Correctness Check
![image](https://github.com/user-attachments/assets/ee2d3e06-f407-4580-bf34-74400c3b0238)

## Performance Analysis
### 1. Analysis 
<p align="justify"> 
First is the evaluation of the execution speed and efficiency of different dot product kernel implementations: the C implementation, x86 SIMD, AVX2 using XMM registers, and AVX2 using YMM registers. As seen on the table provided on the *Average Execution Time section* the debug mode of C is the slowest among all of them while AVX2 YMM is the fastest. while x86 is has an improvement of speed from C, it is still slower from both AVX implementations because x86 still processes one element at a time like C but is faster because of its optimitation in floating-point execution. having said that both AVX are faster than the scalar implementations because of having the ability to process 2 elements at a time for AVX2 XMM and 4 elements at a time for AVX2 YMM, with that being reflected on the table as having C the slowest and AVX2 YMM fastest. 
</p>
### 2. Problems Encountered
