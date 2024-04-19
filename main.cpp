#include "Matrix.hpp"
#include<iostream>
//I write a main to test my implementation
#include<fstream>
#include<string>
using namespace algebra;
int main(){
    //std::ifstream("/mnt/c/Users/hp/Desktop/uni/PACS/ChallengePacs2/lnsp_131.mtx.gz"); //this path will need to be changed depending on the location of the file
    //I need to change the matrix reader with fstream functions
    std::string filename="Insp_131.mtx";
    //const char * filename= "Insp_131.mtx";
    std::ifstream in(filename);
    if(!in)
        std::cerr<<"file cannot be opened"<<std::endl;
    Matrix<double,StorageOrder::Rows> matrix;
    //matrix=matrix.read_market_matrix(filename);
    std::cout<<"yay"<<std::endl;
    return 0;
}