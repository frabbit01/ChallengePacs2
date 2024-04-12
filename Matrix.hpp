#ifndef MATRIX_HPP
#define MATRIX_HPP

#include<iostream>
#include<map>
#include<vector>

namespace algebra{

    template<typename T,typename StorageOrder> class Matrix{
        public:
            void compress();
            void uncompress();
            bool is_compressed const();
            //call operator
            //constructor

        private:
            //CSR format
            unsigned n_rows; //controlla se togliere :number of rows
            unsigned n_nnz; //controlla se togliere: number of non-zero elems
            std::vector<std:size_t> inner_indices; //nel costruttore, costruirlo di size n_rows+1: starting index for the element of each row
            std::vector<std::size_t> outer_indices; //nel costruttore, costruirlo di size n_nnz : corresponding column idxs
            std::vector<T> values; //nel costruttore, costruirlo di length n_nnz: values vector

            //COOmap format
            std::map<std::array<std::size t,2>,T> COOmap;

            //check for format
            bool compressed;
    };
}; //namespace algebra

//friend operator

//extras: matric*matrix, chrono utility

#endif /*MATRIX_HPP*/