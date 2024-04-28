#ifndef MATRIX_HPP
#define MATRIX_HPP

#include<iostream>
#include<map>
#include<vector>
#include<cmath>
#include "mmio.hpp" //I included the library that is published on the mnist site, of which the link was attached in the given pdf file
#include<fstream>
#include<string>
namespace algebra{
    enum class StorageOrder{Rows,Columns};
    template<typename T,StorageOrder S=StorageOrder::Rows> class Matrix{
        public:
            //getters
            unsigned rows() const {return n_rows;}
            unsigned columns() const {return n_cols;}
            unsigned nnz() const {return n_nnz;}
            std::vector<T> get_values() const {return values;}
            std::vector<std::size_t> get_inner_indices() const {return inner_indices;}
            std::vector<std::size_t> get_outer_indices() const {return outer_indices;}
            //setter
            void set_nnz(unsigned n){n_nnz=n;}
            //other methods
            void compress();
            void uncompress(); 
            bool is_compressed() const{return compressed;}
            //call operator 
            const T & operator() (const std::size_t & i, const std::size_t &j) const; 
            T & operator() (const std::size_t & _i, const std::size_t &_j); 
            //constructor
            Matrix()=default; //default constructor
            //It is not necessary to specialize this constructor, for the CSC it is necessary to implement a new comparison operator
            Matrix(unsigned _n_rows,unsigned _n_cols):  //constructor that takes size of the matrix as input
            n_rows(_n_rows),n_cols(_n_cols),n_nnz(0),compressed(false),default_t(0){
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

            //matrix reader: the returned matrix will be in an uncompressed state
            Matrix<T,StorageOrder::Rows> read_market_matrix(const char * filename); 
            //Matrix* vector
            template<typename U>
            friend std::vector<U> operator * (Matrix<U,StorageOrder::Rows> & M, std::vector<U> &v);
            //template<typename U,StorageOrder S>
            //friend std::vector<U> operator * (Matrix<U,StorageOrder::Rows> & M, std::Matrix<U,S> &v);
        private:
            //Compressed format
            unsigned n_rows; //Initialized in the constructor (non default)
            unsigned n_cols; //Initialized in the constructor (non default)
            unsigned n_nnz=0; //I want to update this every time I use the call operator or resize the matrix in any way
            std::vector<std::size_t> inner_indices; //I initialize this in the compress method; starting index for the element of each row
            std::vector<std::size_t> outer_indices; //I initialize this in the compress method; corresponding column idxs
            std::vector<T> values; //I initialize this in the compress method; values vector

            //COOmap format
            std::map<std::array<std::size_t,2>,T> COOmap;

            //check for format
            bool compressed;

            //member i use to return 0 element 
            T default_t= 0;
    };

    ///////////////////////////////////////////////////////////////////////////////
    //class specialization for CSC format

    //I define a comparator to give as parameter for the private member COOmap for the matrix to be column ordered
    struct CompareKeysCSC {
        bool operator()(const std::array<std::size_t, 2>& lhs, const std::array<std::size_t, 2>& rhs) const {
            if (lhs[1] < rhs[1])
                return true;
            if (lhs[1] == rhs[1] && lhs[0] < rhs[0])
                return true;
            return false;
        }
    };
    

    template<typename T> class Matrix<T,StorageOrder::Columns>{
        public:
            //getters
            unsigned rows() const {return n_rows;}
            unsigned columns() const {return n_cols;}
            unsigned nnz() const {return n_nnz;}
            std::vector<T> get_values() const {return values;}
            std::vector<std::size_t> get_inner_indices() const {return inner_indices;}
            std::vector<std::size_t> get_outer_indices() const {return outer_indices;}
            //setter
            void set_nnz(unsigned n){n_nnz=n;} //The only problem with this is that I have to call this manually, since I do not know whether the value after insertion is 0 or not
            //other methods
            void compress();
            void uncompress(); 
            bool is_compressed() const{return compressed;}
            //call operator 
            const T & operator() (const std::size_t & i, const std::size_t &j) const;
            T & operator() (const std::size_t & _i, const std::size_t &_j); 
            
            //constructor
            Matrix()=default; //default constructor
            //It is not necessary to specialize this constructor, for the CSC it is necessary to implement a new comparison operator
            Matrix(unsigned _n_rows,unsigned _n_cols):  //constructor that takes size of the matrix as input
            n_rows(_n_rows),n_cols(_n_cols),n_nnz(0),compressed(false),default_t(0){
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

            //matrix reader!
            Matrix<T,StorageOrder::Columns> read_market_matrix(const char * filename);
            //matrix*vector
            template<typename U>
            friend std::vector<U> operator * (Matrix<U,StorageOrder::Columns> & M, std::vector<U> &v);
            //template<typename U,StorageOrder S>
            //friend std::vector<U> operator * (Matrix<U,StorageOrder::Columns> & M, std::Matrix<U,S> &v);
        private:
            //Compressed format
            unsigned n_rows; //Initialized in the constructor (non default)
            unsigned n_cols; //Initialized in the constructor (non default)
            unsigned n_nnz; //I want to update this every time I use the call operator or resize the matrix in any way
            std::vector<std::size_t> inner_indices; //I initialize this in the uncompress method; starting index for the element of each column
            std::vector<std::size_t> outer_indices; //I initialize this in the uncompress method; corresponding row idxs
            std::vector<T> values; //I initialize this in the uncompress method; values vector

            //COOmap format
            //std::map<std::array<std::size_t,2>,T,decltype(CompareKeysCSC)*> COOmap;
            std::map<std::array<std::size_t,2>,T,CompareKeysCSC> COOmap;
            //check for format
            bool compressed;

            //default value
            T default_t=0;
    };
}; //namespace algebra

//extras: matric*matrix, chrono utility, norms

//Implementation
#include "Matrix_impl.hpp" //for StorageOrder::Rows
#include "Matrix_csc_impl.hpp" //for StorageOrder::Columns

#endif /*MATRIX_HPP*/