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

##### I. 2<sup>20</sup>

![image](https://github.com/user-attachments/assets/30ca8fb3-e4e4-4e43-a232-1ac3892525c5)

##### II. 2<sup>26</sup>

![image](https://github.com/user-attachments/assets/1f404b04-cf70-4118-9387-d5eb05b4f4c8)

##### III. 2<sup>28</sup>

![image](https://github.com/user-attachments/assets/8fe60eb6-f396-4b85-acfe-1236808087b0)


##### IV. 2<sup>29</sup>

![image](https://github.com/user-attachments/assets/37dce62f-2094-436d-9ec1-b552e5d38afd)


##### V. 2<sup>30</sup>

![image](https://github.com/user-attachments/assets/3ecc4d8a-c0b9-419e-ab3d-d8bdf67c1ccb)



#### 2. Debug <br />

##### I. 2<sup>20</sup>

![image](https://github.com/user-attachments/assets/f08fa98b-35b8-4a46-a009-bce0b7162641)

##### II. 2<sup>26</sup>

![image](https://github.com/user-attachments/assets/bb4f4b7e-930c-45fb-a739-008c1df6b73f)


##### III. 2<sup>28</sup>

![image](https://github.com/user-attachments/assets/d1260fc4-0576-414c-9a12-7fba40e4b5ac)

##### IV. 2<sup>29</sup>

![image](https://github.com/user-attachments/assets/efd5f5b3-05e3-4c11-8326-fa5f7ce7fce2)


##### V. 2<sup>30</sup>

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
  First is the evaluation of the execution speed and efficiency of different dot product kernel implementations: the C implementation, x86 SIMD, AVX2 using XMM registers, and AVX2 using YMM registers. As seen on the table provided on the Average Execution Time section the debug mode of C is the slowest among all of them while AVX2 YMM is the fastest. while x86 is has an improvement of speed from C, it is still slower from both AVX implementations because x86 still processes one element at a time like C but is faster because of its optimitation in floating-point execution. having said that both AVX are faster than the scalar implementations because of having the ability to process 2 elements at a time for AVX2 XMM and 4 elements at a time for AVX2 YMM, with that being reflected on the table as having C the slowest and AVX2 YMM fastest. Furthermore their vectorization affects a lot of things for example, having AVX2 YMM to process four elements per iteration, this leads to a fewer loop iterations, which means lesser memory access and an improved cache utilization which makes the CPU have a better performance. Moreover, while running the program to gather the execution times we were able to observe that running the cache initialization multiple times did not significantly impact the results, indicating that the cache warm-up may be consistent after the first execution. Also, To minimize cache penalties, the C kernel was executed before each benchmark to warm up the cache. 
</p>

In the specs it was stated only the required vector size output for testing are: 2<sup>20</sup>, 2<sup>26</sup>, 2<sup>30</sup>, and if 2<sup>30</sup> cannot be executed consider using 2<sup>29</sup> or 2<sup>28</sup> vector sizes. The inclusion of all these vector sizes gives a better understanding as to what affects the release 2<sup>30</sup> high total execution time compared to its debug counterpart, it has something to do with the system's memory capacity being the bottleneck. Running this in a system with a 16GB RAM, task manager shows approximately 12GB use of memory as shown below.
![Release_TaskManager_2^30](https://github.com/user-attachments/assets/1da376a0-c379-4cdc-8bf3-6a5a37363333)
Although this is just a conjecture at this point since verifying this requires a system with a larger RAM, but based on the execution time values for release with vector sizes 2<sup>28</sup> and 2<sup>29</sup>, 2<sup>30</sup> should be faster compared to its debug counterpart.

### 2. Problems Encountered

#### 1. Boundary Handling
- For the cases where the number of inputs is not divisible by 2 (for AVX2_XMM) or 4 (for AVX2_YMM), the remainder is stored when the vector_size is divided by 2 or 4. That value is then stored in RCX so the **LOOP** instruction can be used to handle the remaining inputs that have not yet been computed. 
#### 2. 1st Vector Address 
- An issue arises from the Boundary Handling since the **DIV** instruction stores the remainder in *RDX*, where the address for the first vector is stored.
- The solution for this problem is straightforward, *RDX* is temporarily stored in the memory and is loaded back into a register after the **DIV** instruction is done.
#### 3. Computing for the Result in AVX2_XMM
- After computing through all of the elements in the vectors, the result must be computed by summing all the values in the XMM register. This is done using the **HADDPD** instruction to add the XMM register with itself. 
#### 4. Computing for the Result in AVX2_YMM
- To sum all the elements in the YMM register, you must first extract the lower and upper half of the YMM register into two XMM registers. Then, the **HADDPD** instruction is first used to add both XMM registers then performing the instruction again to add the destination register with itself.
