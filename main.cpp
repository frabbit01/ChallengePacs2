#include "Matrix.hpp"
#include<iostream>
//I write a main to test my implementation
#include<fstream>
using namespace algebra;
int main(){
    //std::ifstream("/mnt/c/Users/hp/Desktop/uni/PACS/ChallengePacs2/lnsp_131.mtx.gz"); //this path will need to be changed depending on the location of the file
    //I need to change the matrix reader with fstream functions
    char * filename= "Insp_131.mtx.gz";
    Matrix<double,StorageOrder::Rows> matrix;
    matrix=matrix.read_market_matrix(filename);
    std::cerr<<"yay"<<std::endl;
    return 0;
}