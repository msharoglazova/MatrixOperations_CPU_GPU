/*
 Name: Maria Sharoglazova
 
 Course: CS280 Introduction to Parallel and Cloud Programming, Section 831, Spring 2019,Online
 
 CRN: 36298
 
 Assignment: #12, Matrix_CPU_GPU
 
 Date: 5-15-2019
 */

/**
 * @author Maria Sharoglazova
 * @since 5-10-2019
 * @file Matrix_CPU_GPU_test.cpp
 * Test driver for the Matrix_CPU_GPU class on CPU
 *
 * NOTE: If the GPU is not present, comment out all the GPU related functions
 * as well as global functions in the header and implementation files.
 */

#include "Matrix_CPU_GPU.h"
#include "Matrix_CPU_GPU.cpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <fstream>

using namespace std;

int main()
{
    //ofstream fout;
    //fout.open("mulmatrix.csv", ios::out | ios::app);
    
    duration<double> runtime;
    Matrix_CPU_GPU<int> *sample_matrix1 = new Matrix_CPU_GPU<int>(2236, 2236, "threesData.bin");
    Matrix_CPU_GPU<int> *sample_matrix2 = new Matrix_CPU_GPU<int>(2236, 2236, "threesData.bin");
    Matrix_CPU_GPU<int> *sample_matrix3 = new Matrix_CPU_GPU<int>(2236, 2236);
    
    sample_matrix1->fillRowMaj(sample_matrix1->readDataFromFile());
    sample_matrix2->fillRowMaj(sample_matrix2->readDataFromFile());
    
    //sample_matrix1->display();
    //sample_matrix2->display();
    
    // CASE 1: Multiply two row major matrices
    sample_matrix3 = sample_matrix1->multiply_CPU(sample_matrix2);
    //sample_matrix3->display();

    runtime = sample_matrix1->getRunTime();
    cout << setprecision(2) << fixed << runtime.count() << endl;
    //fout << setprecision(2) << fixed << runtime.count() << " seconds" << endl;
    
    // CASE 2: Multiply between a row major and a column major matrices
    sample_matrix2->convertOrderCPU();
    
    sample_matrix3 = sample_matrix1->multiply_CPU(sample_matrix2);
    
    runtime = sample_matrix1->getRunTime();
    cout << setprecision(2) << fixed << runtime.count() << endl;
    //fout << setprecision(2) << fixed << runtime.count() << " seconds" << endl;
    
    // CASE 3: Multiply between two column major matrices
    sample_matrix1->convertOrderCPU();
    
    sample_matrix3 = sample_matrix1->multiply_CPU(sample_matrix2);
    
    runtime = sample_matrix1->getRunTime();
    cout << setprecision(2) << fixed << runtime.count() << endl;
    //fout << setprecision(2) << fixed << runtime.count() << " seconds" << endl;
    
    cout << endl;
    
    //fout.close();
    
    return 0;
}

