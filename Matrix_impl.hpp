#ifndef MATRIX_IMPL_HPP
#define MATRIX_IMPL_HPP

//implemenation of Matrix class methods 
#include "Matrix.hpp"
#include<utility>
#include<limits>
#include<algorithm>
#include "mmio.hpp"
namespace algebra{
    template<typename T,StorageOrder S>
    void
    Matrix<T,S>::resize(const unsigned & newrows, const unsigned &newcols){
        if(newrows>n_rows||newcols>n_cols){
            std::cerr<<"The wished dimensions are bigger than the current ones"<<std::endl;
            return;
        }
        if(!compressed){
            for(std::size_t i=0;i<n_rows;++i){
                for(std::size_t j=0;j<n_cols;++j){
                    if(i>=newrows||j>=newcols){
                        std::array<std::size_t,2> key={i,j};
                        T default_t;
                        if(COOmap[key]!=default_t)
                            --n_nnz; //If the element is not of the default value I decrease the numner of non zero elements
                        COOmap.erase(key); //I erase the elements out of range for the new dimensions
                    }
                }
            }
        }
        else{
            std::size_t diff=n_nnz-inner_indices[newrows+1]; //number of elements to erase
            auto last=outer_indices.end(); //I delete the outer indices corresponding to the non zero elements to erase
            auto first=last-diff;
            outer_indices.erase(first,last);
            auto last_i=inner_indices.end(); //I delete the inner indices corresponding to the deleted rows
            auto first_i=last-(n_rows-newrows);
            inner_indices.erase(first_i,last_i);
            n_nnz-=diff; //new number of nonzero elements
        }
        //I update the dimensions
        n_rows=newrows;
        n_cols=newcols;
    }

    template<typename T,StorageOrder S>
    void
    Matrix<T,S>::compress(){
        if(compressed) //If the matrix is already compressed I exit the function
            return;
        inner_indices=std::vector<std::size_t>(n_rows+1);
        outer_indices=std::vector<std::size_t>(n_nnz);
        values=std::vector<T>(n_nnz);
        std::size_t count=0,i=0,row=0;
        bool start_row=false;//flag to check when the first non null element of a row is
        for(auto iter=COOmap.cbegin();iter!=COOmap.cend();++iter){
            if(iter->first[0]!=row) //I reset the flag when I go to the next row in the matrix
                start_row=false; 
            if(iter->second!=0){
                values[i]=iter->second;//store nnz value
                outer_indices=iter->first[1];//store column index
                ++i;
                if(!start_row){//when the row has the first non zero element
                    ++row;
                    inner_indices[row]=count; //store the number of non zero elements before reaching this element
                    start_row=true;
                }
                ++count; //I count the non-zero elements
            }
        }
        COOmap.clear(); //I clear the map for the COOmap format
        compressed=true;
    }

    template<typename T,StorageOrder S>
    void
    Matrix<T,S>::uncompress(){
        T default_t;
        if(!compressed) //If already uncompressed I exit
            return;
        for(std::size_t i=0;i<n_rows;++i){
            unsigned n1=outer_indices[i+1],n2=n_nnz, k=n1;
            if(i<n_rows-1)
                n2=outer_indices[i+2];
            for(size_t j=0;j<n_cols;++j){ //I know n_cols will have to have been initialized either in the call operator (non const) or in the default constructor, since I cannot add elements in the compressed state
                std::array<std::size_t,2> key={i,j};
                if(j!=outer_indices[k]){ //If null element insert 0 value
                    auto v=std::make_pair<std::array<std::size_t,2>,T> (key,default_t);
                    COOmap.insert(v);
                }
                else{ //If non null insert value and update the counter
                    auto v=std::make_pair<std::array<std::size_t,2>,T> (key,values[k]);
                    COOmap.insert(v);
                    if(k+1!=n2) //if we haven't reached the end of the row, update the counter of non zero elements
                        ++k;
                }
            }
        }
        //clear the vectors
        values.clear();
        outer_indices.clear();
        inner_indices.clear();
    }
    template<typename T,StorageOrder S>
    const T & 
    Matrix<T,S>::operator() (const std::size_t & i, const std::size_t &j) const{
        T default_t;
        if(i>=n_rows||j>=n_cols){ //I check if I am within the matrix dimensions
            std::cerr<<"out of bounds"<<std::endl; 
            return std::numeric_limits<T>::quiet_Nan();
        }
        if(!compressed){
            std::array<std::size_t,2> key={i,j};
            return COOmap[key];
        }

        unsigned n1=inner_indices[i+1],n2=n_nnz;
        if(i<n_rows-1)
            n2=inner_indices[i+2];
        for(std::size_t k=n1;k<=n2;++k){//I am cycling through the non null elements of the row
            if(outer_indices[k]==j)
                return values[k]; //If the element is non null I return it
        }
        return default_t; //If the element is null I return the default value
    }

    template<typename T,StorageOrder S>
    T & 
    Matrix<T,S>::operator() (const std::size_t & _i, const std::size_t &_j) {
        T default_t;
        if(compressed &&(_i>=n_rows||_j>=n_cols)){ //If the user tries to add a new element when the matrix is in a compressed state, return an error
            std::cerr<<"You cannot add new elements when the matrix is in a compressed state"<<std::endl;
            //return std::numeric_limits<T>::quiet_Nan();
            return default_t;
        }
        if(compressed){
            unsigned n1=inner_indices[_i+1],n2=n_nnz;
            if(_i<n_rows-1)
                n2=inner_indices[_i+2];
            for(std::size_t k=n1;k<=n2;++k){//I am cycling through the non null elements of the row
                if(outer_indices[k]==_j)
                    return values[k]; //If the element is non null I return it
            }
            return default_t; //If I get a zero element return a default value for T
        }
        if(_i<n_rows&&_j<n_cols){
            std::array<std::size_t,2> key;
            return COOmap[key];
        }
        //if the element is not within bounds I have to adjust the matrix dimensions
        std::size_t g=0;
        auto n_rows_to_add=std::max((_i+1)-n_rows,g), n_cols_to_add=std::max(_j+1-n_cols,g);
        unsigned counter=0; //I keep track of how many null elements I am inserting to fix the matrix dimensions
        //I insert a default value for T to make the matrix have enough rows and columns to be rectangular after inserting the new element
        if(n_cols_to_add>0){
            for(std::size_t i=0;i<n_rows;++i){
                for(std::size_t j=n_cols-1;j<n_cols+n_cols_to_add;++j){
                    COOmap[{i,j}]=default_t; 
                    ++counter;
                }
            }
        }
        if(n_rows_to_add>0){
            for(std::size_t i=n_rows-1;i<n_rows+n_rows_to_add;++i){
                for(std::size_t j=0;j<n_cols+n_cols_to_add;++j){
                    COOmap[{i,j}]=default_t;
                    ++counter;
                }
            }
        }
        //i fix the matrix dimensions
        n_rows+=n_rows_to_add;
        n_cols+=n_cols_to_add;
        n_nnz+=counter;

        return COOmap[{_i,_j}]; //I return a reference to the new element
    }

    template<typename T>
    std::vector<T> 
    operator*(Matrix<T,StorageOrder::Rows> & M, std::vector<T> &v){
        if(M.n_cols!=v.size()){ //dimensions check
                std::cerr<<"incompatible dimensions for matrix vector multiplication"<<std::endl;
                return {};
            }
        std::vector<T> res(v.size());
        if(M.compressed){
            for(std::size_t i=0;i<M.n_rows;++i){
                std::size_t n_elems=M.inner_indices[i+1], old_n_elems=M.inner_indices[i];
                for(std::size_t k=old_n_elems-1;k<n_elems;++k){
                    res[i]+=M.values[k]*v[M.outer_indices[k-old_n_elems+1]];
                }
            }
        }
        else{
            for(std::size_t i=0;i<M.n_rows;++i){
                for(std::size_t j=0;j<M.n_cols;++j){
                    res[i]+=M(i,j)*v[j];
                }
            }
        }
        return res;
    }
    template<typename T,StorageOrder S>
    Matrix<T,StorageOrder::Rows> 
    Matrix<T,S>::read_market_matrix(const char * filename){
        int return_code;
        MM_typecode matcode;
        FILE *f;
        int rows,cols,nnz;
        f=std::fopen(filename,"r");
        if(!f){ //check if the file has opened correctly
            std::cerr<<"file opening has failed"<<std::endl;
            return Matrix<T,StorageOrder::Rows>();
        }
        if(mm_read_banner(f,&matcode)!=0){ //check if the banner has been successfully read
            std::cerr<<"Matrix banner was not read successfully"<<std::endl;
            return Matrix<T,StorageOrder::Rows>();
        }
        if(!mm_is_matrix(matcode)||!mm_is_real(matcode)){//check if the matrix is a general real matrix
            std::cerr<<"This program only works with general real matrices"<<std::endl;
            return Matrix<T,StorageOrder::Rows>();
        }
        if(mm_read_mtx_crd_size(f,&rows,&cols,&nnz)!=0){ //check if the dimensions have been all read correctly
            std::cerr<<"The sizes were not read correctly"<<std::endl;
            return Matrix<T,StorageOrder::Rows>();
        }
        //I call the constructor
        Matrix<T,StorageOrder::Rows> result(rows,cols);
        //I insert the non zero values in the matrix
        for(std::size_t k=0;k<nnz;++k){
            std::size_t i,j;
            T value;
            fscanf(f,"%ld %ld %lg\n",&i,&j,&value);
            result(i-1,j-1)=value; //It has to be considered that now the indices have to start from 0
        }
        //I close the file
        if(f!=stdin)
            fclose(f);
        return result;
    }
};



#endif /*MATRIX_IMPL_HPP*/