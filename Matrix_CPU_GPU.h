
/**
 * @copyright MSharoglazova 5-15-2019
 * @author Maria Sharoglazova
 * @since 5-10-2019
 * @file Matrix_CPU_GPU.h
 * Header file for the Matrix_CPU_GPU class.
 * Objects of this class are matrices, filled in either row major
 * or column major order. Multiplication may be performed on either CPU or GPU.
 */

#ifndef MATRIX_CPU_GPU
#define MATRIX_CPU_GPU

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <limits>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <chrono>

using namespace std;
using namespace::chrono;


// global template function declaration
template<typename T>
__global__ void convertMatrixOrderKl(T* in, T* out, int *matsize);

template<typename T>
__global__ void matrixMultiplyKl(T* matrix1, T* matrix2, T* matrix3, int *matsize);
// end global template function declaration


template<typename T>
class Matrix_CPU_GPU
{
private:
    T* linear_matrix;       // matrix is placed into a linear array

    int width;
    int height;
    int max_size;           // total dimensions of the matrix
    bool row_m;
    bool col_m;
    string file;
    duration<double> runTime; // times matrices operations, i.e. multiplication
    
public:
    /** default constructor: allocates the memory for the matrix.
     * @param   int     matrix width
     * @param   int     matrix height
     */
    Matrix_CPU_GPU(int we, int hei);
    
    /** second constructor: used when the matrix is to be filled with data from a file
     * @param   int     matrix width
     * @param   int     matrix height
     * @param   string  name of the data file
     */
    Matrix_CPU_GPU(int we, int hei, string filename);
    
    //destructor
    ~Matrix_CPU_GPU();
  
    /** getter method to retrieve the matrix array
     */
    T* getMatrix();
    
    /** opens a file and reads its data into a linear array
     */
    T* readDataFromFile();

    /** populates the matrix in a row major order
     * @param   T*       pointer to the linear array of values
     */
    void fillRowMaj(T* linear_array);
    
    /** populates the matrix in a column major order
     * @param   T*       pointer to the linear array of values
     */
    void fillColMaj(T* linear_array);
    
    /** prints out the matrix
     */
    void display();
    
    /** getter method for the multiplication run time
     * @return   duration<double>   a chrono library duration object
     */
    duration<double>getRunTime();
    
    /** validator method for the CPU and GPU multiplication results
     * @return  double      error margin between calculated values
     * @param   Matrix_CPU_GPU<T>*      matrix with the CPU result of multiplication
     * @param   Matrix_CPU_GPU<T>*      matrix with the GPU result of multiplication
     */
    double validate_CPU_GPU(Matrix_CPU_GPU<T> *matrixCPU, Matrix_CPU_GPU<T> *matrixGPU);
    
    //------ CPU Specific Functions -----------------
    /** converts between a row major and column major order on CPU
     */
    void convertOrderCPU();
    
    /** performs multiplication between two matrices on CPU
     * This function will take two Matrix_CPU_GPU objects and multiply
     * the arrays (matrices) of those objects. It will return a new object.
     * @return  Matrix_CPU_GPU<T>*      pointer to a new object containing the result of operation
     * @param   Matrix_CPU_GPU<T>*      pointer to an object to be multiplied by
     */
    Matrix_CPU_GPU<T>* multiply_CPU(Matrix_CPU_GPU<T> *other_matrix);

    //------ GPU Specific Functions -----------------
    /** converts between a row major and a column major order on GPU
     */
    void convertOrderGPU();
    
    /** performs multiplication between two matrices on GPU
     * This function will take two Matrix_CPU_GPU objects and multiply
     * the arrays (matrices) of those objects. It will return a new object.
     * @return  Matrix_CPU_GPU<T>*      pointer to a new object containing the result of operation
     * @param   Matrix_CPU_GPU<T>*      pointer to an object to be multiplied by
     */
    Matrix_CPU_GPU<T>* multiply_GPU(Matrix_CPU_GPU<T> *other_matrix);
    
}; // end Matrix_CPU_GPU

#endif /* Matrix_CPU_GPU_hpp */
