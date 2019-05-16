# MatrixOperations_CPU_GPU
A class that creates and inititalizes 2D matrices and performs operations on the matrices using a CPU or a GPU

Program Goal:
Matrix_CPU_GPU represent a class, objects of which contain matrices. It is designed to exemplify how computations, 
specifically multiplication, are performed on matrices using either CPU or GPU. 

Organization Design:
An object of the class has a linear array that represents a 2 dimensional matrix, via a combination of indices. 
Elements can be stored in row major or column major order, as well as conversions to either are possible using CPU or GPU. 
All operations can be performed using a CPU or a GPU, for which special __global__ template functions are created. 
Objects of the class also contain a duration object of a <chrono> C++11 library, which is meant to time 
the process of multiplication. A special method exists to validate the result of computation on CPU vs GPU.  

Implementation Discussion:
As can be seen from the timing of multiplication, using a GPU to process large size matrices yields times 
that are significantly faster than if the same operation is performed on a CPU. This is possible through 
dividing matrices into blocks and processing large amount of elements simultaneously.
To ensure that the entire matrix is processed and no threads are left out, the BLOCK_SIZE dimensions 
had to be calculated so that the entire matrix is a multiple of the block size. The limitation of this is that, 
should the matrices change the size, the block size will need to be manually recalculated. This provides some 
consideration for future work, to incorporate a functional definition of the block size. Also, further work 
can include other operations that can be performed with matrices.
