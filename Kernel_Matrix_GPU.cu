
/**
 * @author Maria Sharoglazova
 * @since 5-10-2019
 * @file Kernel_Matrix_GPU.cu
 * Test driver kernel for the Matrix_CPU_GPU class on CPU and GPU
 * Tests three cases of matrices multiplication on CPU and GPU
 * Compile on NVIDIA:
 * 	nvcc -std=c++11 -o object_file_name Kernel_Matrix_GPU.cu
 *
 * NOTE: If the GPU is not present, comment out all the GPU related functions
 * as well as global functions in the header and implementation files. Compile as c++.
 */

#include <stdio.h>
#include "Matrix_CPU_GPU.h"
#include "Matrix_CPU_GPU.cpp"
#include <chrono>
#include <iomanip>
#include <fstream>

int main()
{
    ofstream fout;
    fout.open("mulmatrix.csv", ios::in | ios::app);

    duration<double> runtime;
    Matrix_CPU_GPU<int> *matrix1 = new Matrix_CPU_GPU<int>(2236, 2236, "threesData.bin");
    Matrix_CPU_GPU<int> *matrix2 = new Matrix_CPU_GPU<int>(2236, 2236, "threesData.bin");
    Matrix_CPU_GPU<int> *matrix3 = new Matrix_CPU_GPU<int>(2236, 2236);

    matrix1->fillRowMaj(matrix1->readDataFromFile());
    matrix2->fillRowMaj(matrix2->readDataFromFile());

    // CASE 1: Multiply two row major matrices
    matrix3 = matrix1->multiply_CPU(matrix2);
    runtime = matrix1->getRunTime();
    fout << "CASE 1 CPU, " << setprecision(2) << fixed << runtime.count() << endl;
    cout << setprecision(2) << fixed << runtime.count() << endl;

    matrix3 = matrix1->multiply_GPU(matrix2);
    runtime = matrix1->getRunTime();
    fout << "CASE 1 GPU, " <<  setprecision(2) << fixed << runtime.count() << endl;
    cout << setprecision(2) << fixed << runtime.count() << endl;

    // CASE 2: Multiply between a row major and a column major matrices
    matrix2->convertOrderGPU();		// matrix 2 is now column major order

    matrix3 = matrix1->multiply_CPU(matrix2);
    runtime = matrix1->getRunTime();
    fout << "CASE 2 CPU, " << setprecision(2) << fixed << runtime.count() << endl;
    cout << setprecision(2) << fixed << runtime.count() << endl;

    matrix3 = matrix1->multiply_GPU(matrix2);
    runtime = matrix1->getRunTime();
    fout << "CASE 2 GPU, " << setprecision(2) << fixed << runtime.count() << endl;
    cout << setprecision(2) << fixed << runtime.count() << endl;

    // CASE 3: Multiply between two column major matrices
    matrix1->convertOrderGPU();		// matrix 1 is now also column major order

    matrix3 = matrix1->multiply_CPU(matrix2);
    runtime = matrix1->getRunTime();
    fout << "CASE 3 CPU, " << setprecision(2) << fixed << runtime.count() << endl;
    cout << setprecision(2) << fixed << runtime.count() << endl;

    matrix3 = matrix1->multiply_GPU(matrix2);
    runtime = matrix1->getRunTime();
    fout << "CASE 3 GPU, " << setprecision(2) << fixed << runtime.count() << endl;
    cout << setprecision(2) << fixed << runtime.count() << endl;

    fout.close();
    cout << "done";

    cout << endl;

    return 0;
}
