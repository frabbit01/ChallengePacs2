#include "Matrix.hpp"
#include<iostream>
//I write a main to test my implementation
#include<fstream>
#include<string>
using namespace algebra;
int main(){
    const char * filename= "lnsp_131.mtx"; //I used this object instead of a file stream since the library I used was originally written for C, so in order to avoid conflicts I used fopen and fclose
    Matrix<double,StorageOrder::Rows> matrix;
    matrix=matrix.read_market_matrix(filename);
    std::cout<<"yay"<<std::endl;
    std::cout<<matrix.nnz()<<std::endl;
    std::cout<<matrix(43,34)<<std::endl;
    return 0;
}