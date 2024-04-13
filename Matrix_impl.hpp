#ifndef MATRIX_IMPL_HPP
#define MATRIX_IMPL_HPP

//implemenation of Matrix class methods 
#include "Matrix.hpp"
#include<utility>

namespace algebra{
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
        if(!compressed)
            return;
        for(std::size_t i=0;i<n_rows;++i){
            for(size_t j=0;j<n_cols;++j){ //I know n_cols will have to have been initialized either in the call operator (non const) or in the default constructor, since I cannot add elements in the compressed state
                //finire dopo aver definito il call operator
            }
        }
    }
};



#endif /*MATRIX_IMPL_HPP*/