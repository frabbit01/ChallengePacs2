#include "Matrix.hpp"
#include<iostream>
//I write a main to test my implementation
#include<fstream>
#include<string>
using namespace algebra;
int main(){
    //const char * filename= "lnsp_131.mtx"; //I used this object instead of a file stream since the library I used was originally written for C, so in order to avoid conflicts I used fopen and fclose
    //Matrix<double,StorageOrder::Rows> matrix;
    //matrix=matrix.read_market_matrix(filename); //here I read the matrix
    //std::cout<<matrix(43,34)<<std::endl; //here I check that the values and dimensions are correct
    //std::cout<<"rows: "<<matrix.rows()<<"\ncolumns: "<<matrix.columns()<<"\nnon-zero elems: "<<matrix.nnz()<<std::endl;
    //std::vector<double> vec;
    //matrix.compress();
    //matrix.uncompress();
    //matrix(131,131)=3; //sistemare questo errore
    //std::cout<<matrix(130,130)<<std::endl;
    //matrix.uncompress();
    //matrix(131,131)=3;
    //matrix.set_nnz(matrix.nnz()+1); //this does not work
    //std::cout<<matrix(131,131)<<std::endl; //I verify that the insertion worked correctly: da sistemare, non voglio che n_nnz sia aggiornato automaticamente
    //std::cout<<matrix(43,34)<<std::endl; 
    //std::cout<<"rows: "<<matrix.rows()<<"\n columns: "<<matrix.columns()<<"\nnon-zero elems: "<<matrix.nnz()<<std::endl;
    //matrix.resize(50,50); 
    //std::cout<<matrix(43,34)<<std::endl; 
    //std::cout<<"rows: "<<matrix.rows()<<"\ncolumns: "<<matrix.columns()<<"\nnon-zero elems: "<<matrix.nnz()<<std::endl;


    //here I test the same for a column order matrix
    //Matrix<double,StorageOrder::Columns> matrix_columns; 
    //matrix_columns=matrix_columns.read_market_matrix(filename); 
    //std::cout<<matrix_columns(43,34)<<std::endl; //here I check that the values and dimensions are correct
    //std::cout<<"rows: "<<matrix_columns.rows()<<"\ncolumns: "<<matrix_columns.columns()<<"\nnon-zero elems: "<<matrix_columns.nnz()<<std::endl;
    //matrix_columns.compress();
    //matrix_columns.uncompress();
    //matrix_columns.resize(50,50);
    //std::cout<<matrix_columns(43,34)<<std::endl; 
    //std::cout<<"rows: "<<matrix_columns.rows()<<"\ncolumns: "<<matrix_columns.columns()<<"\nnon-zero elems: "<<matrix_columns.nnz()<<std::endl;

    //testing for matrix vector multiplication, next to implement with random vector generator

    Matrix<double,StorageOrder::Rows> R(3,2);
    //std::cout<<R.nnz()<<std::endl;
    std::vector<double> v={1.0,2.0},result(3);
    for (std::size_t i=0;i<3;++i){
        for(std::size_t j=0;j<2;++j){
            R(i,j)=double(i+2*j/2);
        }
    }
    R.set_nnz(5);
    //std::cout<<outer_indices[0]<<outer_indices[1]<<outer_indices[2]<<std::endl;
    R.compress(); //goes into segmentation fault when you do not set the number of nonzero elements manually
    //R.uncompress();
    auto outer_indices=R.get_outer_indices();
    
    //the values are initialized correctly in the compress method: why are they not accessed correctly??
    //R.uncompress(); //when I uncompress I get the right matrix! so this means that the elements are stored correctly in the compressed matrix!
    result=R*v; //does not work when R is compressed: wrong result!
    //std::cout<<"R:\n"<<" "<<R(0,1)<<"\n"<<R(1,0)<<" "<<R(1,1)<<std::endl;
    std::cout<<"R:\n"<<R(0,0)<<" "<<R(0,1)<<"\n"<<R(1,0)<<" "<<R(1,1)<<"\n"<<R(2,0)<<" "<<R(2,1)<<std::endl; 
    //std::cout<<"R:\n"<<R(0,1)<<" "<<R(1,1)<<"\n"<<R(2,0)<<" "<<R(2,1)<<std::endl;
    //I think the issue is when I try returning a zero element from a compressed matrix
    std::cout<<"matrix vector multiplication result: "<<result[0]<<" , "<<result[1]<<" "<<result[2]<<std::endl;
    return 0;
}