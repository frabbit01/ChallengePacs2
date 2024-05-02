#include "Matrix.hpp"
#include<iostream>
//I write a main to test my implementation
#include<fstream>
#include<string>
#include<complex>
#include<chrono.hpp>
#include <random>
using namespace algebra;
int main(){
    Timings::Chrono chrono_object;
    std::default_random_engine engine{42};
    std::uniform_real_distribution<> distr{-1.0,100.0};
    const char * filename= "lnsp_131.mtx"; //I used this object instead of a file stream since the library I used was originally written for C, so in order to avoid conflicts I used fopen and fclose
    Matrix<double,StorageOrder::Rows> matrix;
    matrix=matrix.read_market_matrix(filename); //here I read the matrix
    //std::cout<<matrix(43,34)<<std::endl; //here I check that the values and dimensions are correct
    std::cout<<"rows: "<<matrix.rows()<<"\ncolumns: "<<matrix.columns()<<"\nnon-zero elems: "<<matrix.nnz()<<std::endl;
    std::vector<double> vector(matrix.columns());
    for(std::size_t i=0;i<vector.size();++i){
        vector[i]=distr(engine);
    }
    matrix.compress();
    chrono_object.start();
    auto result=matrix*vector;
    chrono_object.stop();
    std::cout<<result[0]<<std::endl;
    std::cout<<"Time required for matrix vector multiplication"<<std::endl;
    std::cout<<chrono_object<<std::endl;

    std::vector<double> test_vector(matrix.columns());
    for(std::size_t i=0;i<matrix.columns();++i){
        test_vector[i]=1.0;
    }
    std::cout<<"test"<<std::endl;
    auto test_product=matrix*test_vector;
    for(std::size_t i=0;i<matrix.columns();++i){
        std::cout<<test_product[i]<<" ";
    }
    std::cout<<std::endl;

    Matrix<double,StorageOrder::Columns> test_matrix(matrix.columns(),1);
    for(std::size_t i=0;i<matrix.columns();++i){
        test_matrix(i,0)=1.0;
    }
    std::cout<<test_matrix.nnz()<<std::endl;
    test_matrix.set_nnz(matrix.columns());
    
    test_matrix.compress(); 
    
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
    Matrix<double,StorageOrder::Columns> matrix_columns; 
    matrix_columns=matrix_columns.read_market_matrix(filename); 
    //std::cout<<matrix_columns(43,34)<<std::endl; //here I check that the values and dimensions are correct
    std::cout<<"rows: "<<matrix_columns.rows()<<"\ncolumns: "<<matrix_columns.columns()<<"\nnon-zero elems: "<<matrix_columns.nnz()<<std::endl;
    matrix_columns.compress();
    //matrix_columns.uncompress();
    //matrix_columns.resize(50,50);
    //std::cout<<matrix_columns(43,34)<<std::endl; 
    chrono_object.start();
    result=matrix_columns*vector;
    chrono_object.stop();
    std::cout<<result[0]<<std::endl;
    std::cout<<"Time required for matrix vector multiplication"<<std::endl;
    std::cout<<chrono_object<<std::endl;
    std::cout<<"test 2"<<std::endl;
    auto test_product2=matrix*test_matrix; 
    for(std::size_t i=0;i<matrix.columns();++i){
        std::cout<<test_product2[i]<<" ";
    }
    std::cout<<std::endl;
    //std::cout<<"rows: "<<matrix_columns.rows()<<"\ncolumns: "<<matrix_columns.columns()<<"\nnon-zero elems: "<<matrix_columns.nnz()<<std::endl;

    //testing for matrix vector multiplication and with complex numbers

    /*Matrix<std::complex<double>,StorageOrder::Rows> R(3,2);
    //std::cout<<"ok"<<std::endl; //if i do not print this I get an error
    //std::cout<<R.nnz()<<std::endl;
    std::vector<std::complex<double>> v,result(3);
    v.push_back(std::complex<double>(1.0,1.0));
    v.push_back(std::complex<double> (2.0,2.0));
    for (std::size_t i=0;i<3;++i){
        for(std::size_t j=0;j<2;++j){
            std::complex<double> temp (3*i/2,2*j);
            R(i,j)=temp;
        }
    }
    R.set_nnz(5);
    //R.compress(); //need to remove every comparison with 0 and put it as default_t
    //R.uncompress();
    //auto outer_indices=R.get_outer_indices();
    
    //R.uncompress(); 
    //std::cout<<v[0]<<std::endl; //printing this here makes the whole code work: so it means the problem is in the matrix vec multiplication?
    chrono_object.start();
    result=R*v; 
    chrono_object.stop();
    //double time=chrono_object.wallTime();
    std::cout<<chrono_object<<std::endl;
    //std::cout<<"R:\n"<<" "<<R(0,1)<<"\n"<<R(1,0)<<" "<<R(1,1)<<std::endl;
    //std::cout<<"R:\n"<<R(0,0)<<" "<<R(0,1)<<"\n"<<R(1,0)<<" "<<R(1,1)<<"\n"<<R(2,0)<<" "<<R(2,1)<<std::endl; 
    std::cout<<"matrix vector multiplication result: "<<result[0]<<" , "<<result[1]<<" "<<result[2]<<std::endl; //without printing "ok " program crashes here*/
    return 0;
}