/*
 Name: Maria Sharoglazova
 
 Course: CS280 Introduction to Parallel and Cloud Programming, Section 831, Spring 2019,Online
 
 CRN: 36298
 
 Assignment: #12, Matrix_CPU_GPU
 
 Date: 5-15-2019
 */

/**
 * @copyright MSharoglazova 5-15-19
 * @author Maria Sharoglazova
 * @since 5-10-2019
 * @file Matrix_CPU_GPU.cpp
 * Implementation file for the Matrix_CPU_GPU class.
 * Objects of this class are matrices, filled in either row major
 * or column major order. Multiplication may be performed on either CPU or GPU.
 */

#include "Matrix_CPU_GPU.h"

#define BLOCK_SIZE 26


//default constructor
template<typename T>
Matrix_CPU_GPU<T>::Matrix_CPU_GPU(int width, int height)
{
    this->width = width;
    this->height = height;
    
    max_size = width * height;
    linear_matrix = new T[max_size];
}

// second constructor
template<typename T>
Matrix_CPU_GPU<T>::Matrix_CPU_GPU(int width, int height, string file)
{
    this->width = width;
    this->height = height;
    this->file = file;

    max_size = width * height;

    linear_matrix = new T[max_size];
}

// destructor
template<typename T>
Matrix_CPU_GPU<T>::~Matrix_CPU_GPU()
{
    delete linear_matrix;
}

template<typename T>
T* Matrix_CPU_GPU<T>::getMatrix()
{
    return linear_matrix;
}

template<typename T>
T* Matrix_CPU_GPU<T>::readDataFromFile()
{
    //local variables instantiation
    ifstream fin;                                                      // for the file stream
    T filedata;                                                        // for the file datatype
    T* loadArray;                                                      // pointer to a dynamic array
    int filesize;                                                          // for the file size
    int i = 0;                                                         // iterator
    
    if(file.substr(file.find('.'), 4) == ".bin")                        // if the file is binary
    {
        fin.open(file.c_str(), ios::in | ios::binary);                          // open in binary mode
    }
    else
    {
        fin.open(file.c_str(), ios::in);                                        // else open in regular mode
    }
    
    fin.seekg(0, ios::end);
    filesize = fin.tellg();                                        // estimate the file size
    fin.seekg(0, ios::beg);
    
    loadArray = new T[filesize];                                  // dynamically allocate an array of the file size
    
    if(fin.fail())                                                      // test the file
    {
        printf("File %s failed to open. Aborting.", file.c_str());
        exit(1);
    }
    else
    {
        while(fin.good())                                               // while not end of file
        {
            fin.read(reinterpret_cast<char*>(&filedata), sizeof(T)); // read into stream
            
            if(fin.fail())                      // if logic error on the input or end of file
            {
                fin.clear();                    // clear error flags
                fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // remove bad input from stream
            }
            else
            {
                loadArray[i] = filedata;
                //cout << loadArray[i] << " ";
                i++;
            } // end if/else
        } // end while
    } // end if/else
    fin.close();                                                           // close the file stream
    
    return loadArray;
}

template<typename T>
void Matrix_CPU_GPU<T>::fillRowMaj(T* linear_array)
{
    int k = 0; // iterator for the linear array
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            linear_matrix[i * width + j] = linear_array[k];
            k++;
        }
    }
    
    col_m = false;
    row_m = true;
    
}

template<typename T>
void Matrix_CPU_GPU<T>::fillColMaj(T* linear_array)
{
    int k = 0; // iterator for the linear array
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            linear_matrix[j * height + i] = linear_array[k];
            k++;
        }
    }
    
    row_m = false;
    col_m = true;
}

template<typename T>
void Matrix_CPU_GPU<T>::display()
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            cout << linear_matrix[i * width + j] << "       ";
        }
        cout << endl << endl;
    }
    cout << endl;
}

template<typename T>
duration<double> Matrix_CPU_GPU<T>::getRunTime()
{
    return runTime;
}

template<typename T>
double Matrix_CPU_GPU<T>::validate_CPU_GPU(Matrix_CPU_GPU<T> *matrixCPU, Matrix_CPU_GPU<T> *matrixGPU)
{
    T* arrayCPU = new T[max_size];
    T* arrayGPU = new T[max_size];
    arrayCPU = matrixCPU->getMatrix();
    arrayGPU = matrixGPU->getMatrix();
    
    double error = 0;
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            error += arrayCPU[i * width + j] - arrayGPU[i * width + j];
        }
    }
    
    return error;
}

//------------------------------- CPU -------------------------------------------

template<typename T>
void Matrix_CPU_GPU<T>::convertOrderCPU()
{
    T* temp_matrix = new T[max_size];
    
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            temp_matrix[i*width + j] = linear_matrix[j * width + i];
        }
    }
    
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            linear_matrix[i * width + j] = temp_matrix[i * width + j];
        }
    }
    
    if(row_m == true)
    {
        row_m = false;
        col_m = true;
    }
    else
    {
        col_m = false;
        row_m = true;
    }
}

template<typename T>
Matrix_CPU_GPU<T>* Matrix_CPU_GPU<T>::multiply_CPU(Matrix_CPU_GPU<T> *other_matrix)
{
    high_resolution_clock::time_point start;
    high_resolution_clock::time_point stop;

    // create the new matrix to store the CPU result
    Matrix_CPU_GPU<T> *result_matrix = new Matrix_CPU_GPU<T>(width, height);

    T sum;
    T* result_array = new T[max_size];
    T* other_array = new T[max_size];

    other_array = other_matrix->getMatrix();

    start = high_resolution_clock::now();

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            sum = 0;
            for(int k = 0; k < width; k++)
            {
                sum += linear_matrix[i * width + k] * other_array[k * width + j];
            }
            result_array[i * width + j] = sum;
        }
    }

    stop = high_resolution_clock::now();

    runTime = duration_cast< duration<double> >(stop - start);

    result_matrix->fillRowMaj(result_array);

    return result_matrix;
}


//--------------------------------- GPU -------------------------------------------

template<typename T>
__global__ void convertMatrixOrderKl(T* in_matrix, T* out_matrix, int *matsize)
{
    int matwidth = *matsize;
    int row = blockIdx.y*blockDim.y+threadIdx.y;
    int col = blockIdx.x*blockDim.x+threadIdx.x;

    if(row > matwidth || col > matwidth)
        return;

    out_matrix[row * matwidth + col] = in_matrix[col * matwidth + row];

}

template<typename T>
__global__ void matrixMultiplyKl(T* matrix1, T* matrix2, T* matrix3, int *matsize)
{
    int matwidth = *matsize;
    int row = blockIdx.y*blockDim.y+threadIdx.y;
    int col = blockIdx.x*blockDim.x+threadIdx.x;

    T sum = 0;

    if(row > matwidth || col > matwidth)
        return;

    for(int i = 0; i < matwidth; i++)
    {
        sum += matrix1[row * matwidth + i] * matrix2[i * matwidth + col];
    }
    matrix3[row * matwidth + col] = sum;
}

template<typename T>
void Matrix_CPU_GPU<T>::convertOrderGPU()
{
    // GPU variables
    T* in, *out;
    int *W;

    cudaMalloc((void**)&in, sizeof(T)*max_size);
    cudaMalloc((void**)&out, sizeof(T)*max_size);
    cudaMalloc((void**)&W, sizeof(int));

    cudaMemcpy(in, linear_matrix, sizeof(T)*max_size, cudaMemcpyHostToDevice);
    cudaMemcpy(W, &width, sizeof(int), cudaMemcpyHostToDevice);

    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 dimGrid(width/dimBlock.x, height/dimBlock.y);

    convertMatrixOrderKl<T><<<dimGrid,dimBlock>>>(in, out, W);
    cudaDeviceSynchronize();

    cudaMemcpy(linear_matrix, out, sizeof(T)*max_size, cudaMemcpyDeviceToHost);
    cudaDeviceSynchronize();

    cudaFree(in);
    cudaFree(out);

    if(row_m == true)
    {
        row_m = false;
        col_m = true;
    }
    else
    {
        col_m = false;
        row_m = true;
    }
}

template<typename T>
Matrix_CPU_GPU<T>* Matrix_CPU_GPU<T>::multiply_GPU(Matrix_CPU_GPU<T> *other_matrix)
{
    high_resolution_clock::time_point start;
    high_resolution_clock::time_point stop;

    // create the new matrix to store the CPU result
    Matrix_CPU_GPU<T> *result_matrix = new Matrix_CPU_GPU<T>(width, height);

    T* result_array = new T[max_size];
    T* array2 = new T[max_size];
    array2 = other_matrix->getMatrix();

    // GPU variables
    T *A, *B, *C;
    int *W;

    cudaMalloc((void**)&A, sizeof(T)*max_size);
    cudaMalloc((void**)&B, sizeof(T)*max_size);
    cudaMalloc((void**)&C, sizeof(T)*max_size);
    cudaMalloc((void**)&W, sizeof(int));

    cudaMemcpy(A, linear_matrix, sizeof(T)*max_size, cudaMemcpyHostToDevice);
    cudaMemcpy(B, array2, sizeof(T)*max_size, cudaMemcpyHostToDevice);
    cudaMemcpy(W, &width, sizeof(int), cudaMemcpyHostToDevice);

    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 dimGrid(width/dimBlock.x, height/dimBlock.y);

    start = high_resolution_clock::now();

    matrixMultiplyKl<T><<<dimGrid,dimBlock>>>(A, B, C, W);
    cudaDeviceSynchronize();

    stop = high_resolution_clock::now();
    runTime = duration_cast< duration<double> >(stop - start);

    cudaMemcpy(result_array, C, sizeof(T)*max_size, cudaMemcpyDeviceToHost);
    cudaDeviceSynchronize();

    result_matrix->fillRowMaj(result_array);

    cudaFree(A);
    cudaFree(B);
    cudaFree(C);

    return result_matrix;
}

