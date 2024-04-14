#ifndef MATRIX_HPP
#define MATRIX_HPP

#include<iostream>
#include<map>
#include<vector>

namespace algebra{
    enum class StorageOrder{Rows,Columns};
    template<typename T,StorageOrder S> class Matrix{
        public:
            //getters
            unsigned rows() const {return n_rows;}
            unsigned columns() const {return n_cols;}
            unsigned nnz() const {return n_nnz;}
            //other methods
            void compress();//Bisogna generalizzare per il caso in cui lo StorageOrder non sia per CSR
            void uncompress(); //da fare per il caso csc 
            bool is_compressed() const{return compressed;}
            //call operator non const:Missing
            const T & operator() (const std::size_t & i, const std::size_t &j) const; //va generalizzato per il caso CSC nell'ultima parte
            T & operator() (const std::size_t & _i, const std::size_t &_j); //va generalizzato per il caso CSC se compresso
            //overloading less than: missing for csc
            //constructor
            Matrix()=default; //default constructor
            //It is not necessary to specialize this constructor, for the CSC it is necessary to implement a new comparison operator
            Matrix(unsigned _n_rows,unsigned _n_cols):  //constructor that takes size of the matrix as input
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
            void resize(const unsigned & newrows, const unsigned &newcols); //check if it means to also get it bigger, implement if so is the case
        private:
            //Compressed format
            unsigned n_rows; //Initialized in the constructor (non default)
            unsigned n_cols; //Initialized in the constructor (non default)
            unsigned n_nnz; //I want to update this every time I use the call operator or resize the matrix in any way
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