#ifndef MATRIX_HPP
#define MATRIX_HPP

#include<iostream>
#include<map>
#include<vector>

namespace algebra{
    //How to pass an enumerator??
    enum class StorageOrder{Rows,Columns};
    template<typename T,StorageOrder S> class Matrix{
        public:
            void compress();//Bisogna generalizzare per il caso in cui lo StorageOrder non sia per CSR
            void uncompress(); //Implementazione da finire!!!!
            bool is_compressed() const{return compressed;}
            //call operator:Missing
            //constructor
            Matrix()=default; //default constructor
            //Bisogna generalizzare per il caso in cui lo StorageOrder non sia per CSR!!
            Matrix(std::size_t _n_rows,std::size_t _n_cols):  //constructor that takes size of the matrix as input
            n_rows(_n_rows),n_cols(_n_cols),compressed(false){
                T default_t;
                for(std::size_t i=0;i<n_rows;++i){
                    for(std::size_t j=0;j<n_cols;++j){
                        std::array<std::size_t,2> key={i,j};
                        COOmap[key]=default_t;
                    }
                }
            } 
            //resize
        private:
            //CSR format
            unsigned n_rows; //Initialized in the constructor (non default)
            unsigned n_cols; //Initialized in the constructor (non default)
            unsigned n_nnz; //I want to update this every time I use the call operator
            std::vector<std::size_t> inner_indices; //I initialize this in the uncompress method; starting index for the element of each row
            std::vector<std::size_t> outer_indices; //I initialize this in the uncompress method; corresponding column idxs
            std::vector<T> values; //I initialize this in the uncompress method; values vector

            //COOmap format
            std::map<std::array<std::size_t,2>,T> COOmap;

            //check for format
            bool compressed;
    };
}; //namespace algebra

//friend operator

//extras: matric*matrix, chrono utility

//Implementation
#include "Matrix_impl.hpp"


#endif /*MATRIX_HPP*/