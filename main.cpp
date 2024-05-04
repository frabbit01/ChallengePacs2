/**
 * @file main.cpp
 * @author Francesca Visalli
 * @brief File that contains int main() function made to test the implementation by reading a matrix in matrix market format from a file
 * @version 0.1
 * @date 2024-05-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "Matrix.hpp"
#include<iostream>
#include<fstream>
#include<string>
#include<complex>
#include<chrono.hpp>
#include <random>
using namespace algebra;
/**
 * @brief main function to test the implementation
 * 
 * @return int 0 if no errors occur
 */
int main(){
    Timings::Chrono chrono_object;
    std::default_random_engine engine{42};
    std::uniform_real_distribution<> distr{-1.0,100.0};
    const char * filename= "lnsp_131.mtx"; ///I used this object instead of a file stream since the library I used was originally written for C, so in order to avoid conflicts I used fopen and fclose
    Matrix<double,StorageOrder::Rows> matrix;
    matrix=matrix.read_market_matrix(filename); ///here I read the matrix
    std::cout<<"rows: "<<matrix.rows()<<"\ncolumns: "<<matrix.columns()<<"\nnon-zero elems: "<<matrix.nnz()<<std::endl;
    /// I generate a random vector to test my implementation
    std::vector<double> vector(matrix.columns());
    for(std::size_t i=0;i<vector.size();++i){
        vector[i]=distr(engine);
    }
    /// The time interval for compressed matrices is shorter than the uncompressed case
    chrono_object.start();
    auto result=matrix*vector;
    chrono_object.stop();
    std::cout<<result[0]<<std::endl;
    std::cout<<"Time required for uncompressed matrix vector multiplication"<<std::endl;
    std::cout<<chrono_object<<std::endl;

    std::cout<<"result"<<std::endl;
    for(std::size_t i=0;i<matrix.columns();++i){
        std::cout<<result[i]<<" ";
    }
    std::cout<<std::endl;

    matrix.compress();
    chrono_object.start();
    result=matrix*vector;
    chrono_object.stop();
    std::cout<<result[0]<<std::endl;
    std::cout<<"\nTime required for CSR matrix vector multiplication"<<std::endl;
    std::cout<<chrono_object<<std::endl;

    std::cout<<"\nresult"<<std::endl;
    for(std::size_t i=0;i<matrix.columns();++i){
        std::cout<<result[i]<<" ";
    }
    std::cout<<std::endl;

    Matrix<double,StorageOrder::Columns> test_matrix(matrix.columns(),1);
    for(std::size_t i=0;i<matrix.columns();++i){
        test_matrix(i,0)=vector[i];
    }
    test_matrix.set_nnz(matrix.columns());
    test_matrix.compress();
    
    result=matrix*test_matrix; /// This shows that the implementation works for one-column matrices as well
    std::cout<<"result with a 1-column matrix"<<std::endl;
    for(std::size_t i=0;i<matrix.columns();++i){
        std::cout<<result[i]<<" ";
    }
    std::cout<<std::endl;

    /**here I test the same for a column ordered matrix*/

    Matrix<double,StorageOrder::Columns> matrix_columns; 
    matrix_columns=matrix_columns.read_market_matrix(filename); 
    std::cout<<"rows: "<<matrix_columns.rows()<<"\ncolumns: "<<matrix_columns.columns()<<"\nnon-zero elems: "<<matrix_columns.nnz()<<std::endl;

    chrono_object.start();
    result=matrix_columns*vector;
    chrono_object.stop();
    std::cout<<"\nTime required for uncompressed matrix vector multiplication"<<std::endl;
    std::cout<<chrono_object<<std::endl;
    std::cout<<"\nresult\n"<<std::endl;
    for(std::size_t i=0;i<matrix.columns();++i){
        std::cout<<result[i]<<" ";
    }
    std::cout<<std::endl;

    matrix_columns.compress();
    chrono_object.start();
    result=matrix_columns*vector;
    chrono_object.stop();
    std::cout<<"\nTime required for CSC matrix vector multiplication"<<std::endl;
    std::cout<<chrono_object<<std::endl;
    std::cout<<"\nresult\n"<<std::endl;
    for(std::size_t i=0;i<matrix.columns();++i){
        std::cout<<result[i]<<" ";
    }
    std::cout<<std::endl;
    
    matrix_columns.uncompress();
    test_matrix.uncompress();
    result=matrix_columns*test_matrix;
    std::cout<<"\nresult with a 1-column matrix\n"<<std::endl;
    for(std::size_t i=0;i<matrix.columns();++i){
        std::cout<<result[i]<<" ";
    }
    std::cout<<std::endl;

    /**testing for matrix vector multiplication and with complex numbers
     * I test this with a very small matrix and vector
    */

    Matrix<std::complex<double>,StorageOrder::Rows> R(3,2);
    std::vector<std::complex<double>> vector_complex,result_complex(2);
    vector_complex.push_back(std::complex<double>(1.0,1.0));
    vector_complex.push_back(std::complex<double> (2.0,2.0));
    for (std::size_t i=0;i<3;++i){
        for(std::size_t j=0;j<2;++j){
            std::complex<double> temp (3*i/2,2*j);
            R(i,j)=temp;
        }
    }
    R.set_nnz(5);
    R.compress();    
    result_complex=R*vector_complex; 
    
    std::cout<<"matrix vector multiplication result: "<<result[0]<<" , "<<result[1]<<std::endl; 
    return 0;
}