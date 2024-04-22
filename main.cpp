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
    std::cout<<"rows: "<<matrix.rows()<<"\ncolumns: "<<matrix.columns()<<"\nnon-zero elems: "<<matrix.nnz()<<std::endl;
    matrix.compress();
    //matrix.uncompress();
    //matrix(131,131)=3; //sistemare questo errore
    //std::cout<<matrix(130,130)<<std::endl;
    //matrix.uncompress();
    //matrix(131,131)=3;
    //matrix.set_nnz(matrix.nnz()+1); //this does not work
    //std::cout<<matrix(131,131)<<std::endl; //I verify that the insertion worked correctly: da sistemare, non voglio che n_nnz sia aggiornato automaticamente
    std::cout<<matrix(43,34)<<std::endl; 
    //std::cout<<"rows: "<<matrix.rows()<<"\n columns: "<<matrix.columns()<<"\nnon-zero elems: "<<matrix.nnz()<<std::endl;
    matrix.resize(50,50); //after having compressed n_nz is wrong!
    std::cout<<matrix(43,34)<<std::endl; //THIS GOES TO SEGMENTATION FAULT AFTER RESIZING A COMPRESSED MATRIX??? only for csr??
    std::cout<<"rows: "<<matrix.rows()<<"\ncolumns: "<<matrix.columns()<<"\nnon-zero elems: "<<matrix.nnz()<<std::endl;

    //here I test the same for a column order matrix
    Matrix<double,StorageOrder::Columns> matrix_columns; 
    matrix_columns=matrix_columns.read_market_matrix(filename); 
    std::cout<<matrix_columns(43,34)<<std::endl; //here I check that the values and dimensions are correct
    std::cout<<"rows: "<<matrix_columns.rows()<<"\ncolumns: "<<matrix_columns.columns()<<"\nnon-zero elems: "<<matrix_columns.nnz()<<std::endl;
    matrix_columns.compress();
    //matrix_columns.uncompress();
    matrix_columns.resize(50,50);
    std::cout<<matrix_columns(43,34)<<std::endl; 
    std::cout<<"rows: "<<matrix_columns.rows()<<"\ncolumns: "<<matrix_columns.columns()<<"\nnon-zero elems: "<<matrix_columns.nnz()<<std::endl;
    return 0;
}