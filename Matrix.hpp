/**
 * @file Matrix.hpp
 * @author Francesca Visalli (frabbit01)
 * @brief Template Class Matrix and its specialization for column ordered
 * matrices In this file you will find the declaration of all the methods and
 * members of the Matrix class. For their definitions take a look at
 * Matrix_impl.hpp and Matrix_csc.hpp
 *
 * Among the inclusions I included also a library published on the mnist site
 *
 * @version 0.1
 * @date 2024-05-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MATRIX_HPP
#define MATRIX_HPP
// clang-format off
#include<iostream>
#include<map>
#include<vector>
#include<cmath>
#include "mmio.hpp" //I included the library that is published on the mnist site, of which the link was attached in the given pdf file
#include<fstream>
#include<string>
//@note For misterous reasons your version of the compiler does not require to include <array>< when using arrays!
#include<array>
namespace algebra{
    //@note good documentation with doxygen!
    /**
     * @brief Enumerated class that defines the ordainement of a Matrix object
     * enumerator list is {Rows, Columns}
     * 
     */
    enum class StorageOrder{Rows,Columns};
    /**
     * @brief Matrix template class that considers row ordered matrices as its default case. It can deal with both uncompressed matrices and with CSR matrices.
     * 
     * @tparam T type of the Matrix object elements
     * @tparam S StorageOrder of the matrix (either StorageOrder::Rows or StorageOder::Columns)
     */
    template<typename T,StorageOrder S=StorageOrder::Rows> class Matrix{
        public:
        /* GETTERS*/
            /// @brief Getter for the number of rows of the matrix
            /// @returns The private member n_rows
            unsigned rows() const {return n_rows;}
            /// @brief Getter for the number of columns of the matrix
            /// @return The private member n_cols
            unsigned columns() const {return n_cols;}
            /// @brief Getter for the number of non zero elements of the matrix
            /// @return The private member n_nnz
            unsigned nnz() const {return n_nnz;}
            /// @brief Getter for the vector of values (not empty only if the matrix is in the compressed format)
            /// @return A vector of size equal to the value of n_nnz containing the corresponding values
            std::vector<T> get_values() const {return values;}
            /// @brief Getter for the vector of inner indices, in the default case of a matrix ordered by rows it has the size of n_rows+1 and contains the starting index for each row
            /// @return The private member inner_indices (not empty only if the matrix is compressed)
            std::vector<std::size_t> get_inner_indices() const {return inner_indices;}
            /// @brief Getter for the vector of outer indices of the matrix. In the default case of a matrix ordered by rows it contains the column index corresponding to each non zero value
            /// @return The private member outer_indices (not empty only if the matrix is compressed) of size n_nnz.
            std::vector<std::size_t> get_outer_indices() const {return outer_indices;}
            /// @brief Method that copies the COOmap member into a vector and returns it
            /// @return The aforementioned vector 
            std::vector<T> map_to_vec() const {
                std::vector<T> res(COOmap.size());
                unsigned i=0;
                for(auto iter=COOmap.begin();iter!=COOmap.end();++iter){
                    res[i]=iter->second;
                    ++i;
                }
                return res;
            }
            /*SETTER*/
            /// @brief Setter which allows the user to set a value for the private member n_nnz. Its call is recommended after an insertion, especially of a null element, to avoid undefined behaviour after compression
            //@note I would not expose this setter to the fgeneral public. Too dangerous. It is used only internally, so it should be private.
            // Maybe is also useless since members can modifie nnz without the need of a setter
            void set_nnz(unsigned n){n_nnz=n;}
            
            /*OTHER PUBLIC METHODS*/
            /// @brief Method that converts an uncompressed matrix into a compressed format (The default is CSR) by updating its private members
            void compress();
            /// @brief Method that converts an compressed matrix (CSR by default) into an  uncompressed format (ordered ny rows as default) by updating its private members
            void uncompress(); 
            /// @brief Method that checks the format of a matrix 
            /// @return The private member compressed
            bool is_compressed() const{return compressed;}
            /*CALL OPERATOR*/
            /// @brief Call operator (const version)
            /// @param i Row index
            /// @param j Column index
            /// @return A const reference to the corresponding element of the matrix
            const T & operator() (const std::size_t & i, const std::size_t &j) const; 
            /// @brief Call operator (non const version). The user is not allowed to insert a value that exceeds the matrix dimensions when it is in the compressed state
            /// @param _i Row index
            /// @param _j Column index
            /// @return A reference to the corresponding element of the matrix
            T & operator() (const std::size_t & _i, const std::size_t &_j); 
            /*CONSTRUCTORS*/
            /// @brief Default constructor
            Matrix()=default;
            /// @brief Constructor that initializes an uncompressed matrix of zeros with the specified dimensions
            /// @param _n_rows Desired number of rows
            /// @param _n_cols Desired number of columns
            Matrix(unsigned _n_rows,unsigned _n_cols): 
            n_rows(_n_rows),n_cols(_n_cols),n_nnz(0),compressed(false),default_t(0){
                //T default_t; //@note You are redefining the default_t variable here, but it is already defined in the private section of the class
                // and I suppose you wanted to use that one. I have commented it out.
                // Otherwise, you are using the local variable and not the member variable!
                for(std::size_t i=0;i<n_rows;++i){
                    for(std::size_t j=0;j<n_cols;++j){
                        std::array<std::size_t,2> key={i,j};
                        COOmap[key]=default_t;
                    }
                }
            } 
            /*OTHER PUBLIC METHODS*/
            /// @brief Method that resizes the matrix to have the desired dimensions
            /// @param newrows Desired number of rows
            /// @param newcols Desired number of columns
            void resize(const unsigned & newrows, const unsigned &newcols); 

            /// @brief Method that reads a matrix in a Matrix Market format
            /// @param filename The name of the file from which the matrix will be read from 
            /// @return A matrix in the uncompressed format 
            Matrix<T,StorageOrder::Rows> read_market_matrix(const char * filename); 
            
            /*MATRIX VECTOR PRODUCT*/
            /// @brief Matrix-vector product (friend operator)
            /// @param M Matrix to be multiplied (row ordered)
            /// @param v Vector to be multiplied
            /// @return A vector (result of the operatrion)
            template<typename U>
            friend std::vector<U> operator * (Matrix<U,StorageOrder::Rows> & M, std::vector<U> &v);
            /// @brief Matrix-vector product (friend operator)
            /// @param M Matrix to be multiplied (row ordered)
            /// @param m A matrix having only one column to be multiplied
            /// @return A vector (result of the operatrion)
            // @note I dont understand why you are limiting to the case of RowMajor matrices.
            template<typename U,StorageOrder K>
            friend std::vector<U> operator * (Matrix<U,StorageOrder::Rows> & M, Matrix<U,K> &m);
        private:
            /// Compressed format
            unsigned n_rows; /// Initialized in the constructor (non default)
            unsigned n_cols; /// Initialized in the constructor (non default)
            unsigned n_nnz=0; /// I want to update this every time I use the call operator or resize the matrix in any way
            std::vector<std::size_t> inner_indices; /// I initialize this in the compress method; starting index for the element of each row
            std::vector<std::size_t> outer_indices; /// I initialize this in the compress method; corresponding column idxs
            std::vector<T> values; /// I initialize this in the compress method; values vector

            ///COOmap format
            //@note the simplest way to accomodate row and column ordering when using a map is to change the comparator function
            // The column ordered version compare the second element first and then the first element.
            std::map<std::array<std::size_t,2>,T> COOmap;

            ///check for format
            bool compressed;

            ///member I use to return 0 element for any type T
            T default_t= 0; //@note it could be static, since it is the same for all the instances of the class, I suppose.
    };

    /**
     * Class specialization for CSC format
     * 
     */

    /**
     * @brief Struct that contains a new comparator for the COOmap member
     * 
     */
    struct CompareKeysCSC {
        /**
         * @brief Comparator between arrays of two sixe_t elements for column ordered matrices
         * 
         * @param lhs First array to be compared
         * @param rhs Second array to be compared
         * @return true when either the second element of the first array is less than the second element of rhs or when they're equal but the first element of lhs is smaller then the second element of rhs 
         * @return false otherwise
         */
        //@note You are redefining the comparison operator for the array. I suppose to use it in the map. But it is better to create a functor and 
        // pass it to the map as third template parameter. Overloading < propagates the behaviour to all comparisons of arrays of 2 doubles in the methods of this class, which maybe confusiong and prone
        // to error. When redefining a comparison operator is meant to have a localised effect, it is better to use a functor.
        bool operator()(const std::array<std::size_t, 2>& lhs, const std::array<std::size_t, 2>& rhs) const {
            if (lhs[1] < rhs[1])
                return true;
            if (lhs[1] == rhs[1] && lhs[0] < rhs[0])
                return true;
            return false;
        }
    };
    
    /**
     * @brief class specialization of the Matrix template class for column ordered matrices
     * 
     * @tparam T type of the matrix elements
     */
    //@note Now I undertand. You have made a specialization of the whole class. Not necessary, you are duplicating a lot of stuff!
    // you can just specialize some methods, or use if constexpr, and set the comparison operator aaccording to the ordering, thus keeping
    // the same class for both cases. 
    template<typename T> class Matrix<T,StorageOrder::Columns>{
        public:
            /*GETTERS*/
            
            /// @brief Getter for the number of rows of the matrix
            /// @returns The private member n_rows
            unsigned rows() const {return n_rows;}
            /// @brief Getter for the number of columns of the matrix
            /// @returns The private member n_cols
            unsigned columns() const {return n_cols;}
            /// @brief Getter for the number of non zero elements of the matrix
            /// @returns The private member n_nnz
            unsigned nnz() const {return n_nnz;}
            /// @brief Getter for the vector of values (not empty only if the matrix is in the compressed format)
            /// @return A vector of size equal to the value of n_nnz containing the corresponding values
            std::vector<T> get_values() const {return values;}
            /// @brief Getter for the vector of inner indices for a column ordered matrix; it has the size of n_cols+1 and contains the starting index for each column
            /// @return The private member inner_indices (not empty only if the matrix is compressed)
            std::vector<std::size_t> get_inner_indices() const {return inner_indices;}
            /// @brief Getter for the vector of outer indices for a column ordered matrix; it has the size of n_nz and contains the column index for each non zero element
            /// @return The private member outer_indices (not empty only if the matrix is compressed)
            std::vector<std::size_t> get_outer_indices() const {return outer_indices;}
            /// @brief Method that copies the COOmap member into a vector and returns it
            /// @return The aforementioned vector 
            std::vector<T> map_to_vec() const {
                std::vector<T> res(COOmap.size());
                unsigned i=0;
                for(auto iter=COOmap.begin();iter!=COOmap.end();++iter){
                    res[i]=iter->second;
                    ++i;
                }
                return res;
            }
            /*SETTER*/

            /// @brief Setter which allows the user to set a value for the private member n_nnz. Its call is recommended after an insertion, especially of a null element, to avoid undefined behaviour after compression
            void set_nnz(unsigned n){n_nnz=n;} //The only problem with this is that I have to call this manually, since I do not know whether the value after insertion is 0 or not
            
            /*OTHER PUBLIC METHODS*/
            /// @brief Method that resizes the(column ordered) matrix to have the desired dimensions
            /// @param newrows Desired number of rows
            /// @param newcols Desired number of columns
            void compress();
            /// @brief Method that converts an compressed matrix (CSC by default) into an  uncompressed format (ordered ny rows as default) by updating its private members
            void uncompress(); 
            /// @brief Method that checks the format of a matrix 
            /// @return The private member compressed
            bool is_compressed() const{return compressed;}

            /*CALL OPERATOR*/
            /// @brief Call operator (const version)
            /// @param i Row index
            /// @param j Column index
            /// @return A const reference to the corresponding element of the matrix
            const T & operator() (const std::size_t & i, const std::size_t &j) const;
            /// @brief Call operator (non const version). The user is not allowed to insert a value that exceeds the matrix dimensions when it is in the compressed state
            /// @param _i Row index
            /// @param _j Column index
            /// @return A reference to the corresponding element of the matrix
            T & operator() (const std::size_t & _i, const std::size_t &_j); 
            
            /*CONSTRUCTORS*/
            /// @brief Default constructor
            Matrix()=default;
            /// @brief Constructor that initializes an uncompressed column ordered matrix of zeros with the specified dimensions
            /// @param _n_rows Desired number of rows
            /// @param _n_cols Desired number of columns
            Matrix(unsigned _n_rows,unsigned _n_cols):  //constructor that takes size of the matrix as input
            n_rows(_n_rows),n_cols(_n_cols),n_nnz(0),compressed(false),default_t(0){
                //T default_t;//@note You are redefining the default_t variable here, but it is already defined in the private section of the class
                // Moreover it is uninitialised. This can cause serious problems. I suppose you wanted to use the default_t member of the class.
                // I have commented it out
                for(std::size_t i=0;i<n_rows;++i){
                    for(std::size_t j=0;j<n_cols;++j){
                        std::array<std::size_t,2> key={i,j};
                        COOmap[key]=default_t;
                    }
                }
            } 

            /*OTHER PUBLIC METHODS*/
            /// @brief Method that resizes the matrix to have the desired dimensions
            /// @param newrows Desired number of rows
            /// @param newcols Desired number of columns
            void resize(const unsigned & newrows, const unsigned &newcols); 

            /// @brief Method that reads a matrix in a Matrix Market format
            /// @param filename The name of the file from which the matrix will be read from 
            /// @return A matrix in the uncompressed format 
            Matrix<T,StorageOrder::Columns> read_market_matrix(const char * filename);

            /*MATRIX-VECTOR PRODUCT*/
            /// @brief Matrix-vector product (friend operator)
            /// @param M Matrix to be multiplied (column ordered)
            /// @param v Vector to be multiplied
            /// @return A vector (result of the operatrion)
            template<typename U>
            friend std::vector<U> operator * (Matrix<U,StorageOrder::Columns> & M, std::vector<U> &v);
            /// @brief Matrix-vector product (friend operator)
            /// @param M Matrix to be multiplied (column ordered)
            /// @param m A matrix having only one column to be multiplied
            /// @return A vector (result of the operatrion)
            template<typename U,StorageOrder K>
            friend std::vector<U> operator * (Matrix<U,StorageOrder::Columns> & M, Matrix<U,K> &m);
        private:
            ///Compressed format
            unsigned n_rows; ///Initialized in the constructor (non default)
            unsigned n_cols; ///Initialized in the constructor (non default)
            unsigned n_nnz; ///I want to update this every time I use the call operator or resize the matrix in any way
            std::vector<std::size_t> inner_indices; ///I initialize this in the uncompress method; starting index for the element of each column
            std::vector<std::size_t> outer_indices; ///I initialize this in the uncompress method; corresponding row idxs
            std::vector<T> values; ///I initialize this in the uncompress method; values vector

            ///COOmap format
            std::map<std::array<std::size_t,2>,T,CompareKeysCSC> COOmap;
            ///check for format
            bool compressed;

            ///default value
            T default_t=0;
    };
}; ///namespace algebra


/**
 * Implementation of the methods declared in this file
 */
#include "Matrix_impl.hpp" ///for StorageOrder::Rows
#include "Matrix_csc_impl.hpp" ///for StorageOrder::Columns

#endif /*MATRIX_HPP*/