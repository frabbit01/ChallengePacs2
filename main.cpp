#include "Matrix.hpp"
#include<iostream>
//I write a main to test my implementation
#include<fstream>
#include<string>
using namespace algebra;
int main(){
    const char * filename= "lnsp_131.mtx"; //I used this object instead of a file stream since the library I used was originally written for C, so in order to avoid conflicts I used fopen and fclose
    Matrix<double,StorageOrder::Rows> matrix;
    matrix=matrix.read_market_matrix(filename); //here I read the matrix
    std::cout<<matrix(43,34)<<std::endl; //here I check that the values and dimensions are correct
    std::cout<<"rows: "<<matrix.rows()<<"\n columns: "<<matrix.columns()<<"\nnon-zero elems: "<<matrix.nnz()<<std::endl;

    //here I test the same for a column order matrix
    Matrix<double,StorageOrder::Columns> matrix_columns;
    matrix_columns=matrix_columns.read_market_matrix(filename);
    std::cout<<matrix(43,34)<<std::endl; //here I check that the values and dimensions are correct
    std::cout<<"rows: "<<matrix.rows()<<"\n columns: "<<matrix.columns()<<"\nnon-zero elems: "<<matrix.nnz()<<std::endl;
    return 0;
}