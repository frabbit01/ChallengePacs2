/**
 * @file Matrix_impl.hpp
 * @author Francesca Visalli (frabbit01)
 * @brief implemenation of Matrix class methods (thought for the default case of
 * row ordered matrices)
 * @version 0.1
 * @date 2024-05-03
 *
 * @copyright Copyright (c) 2024
 *
 */
// clang-format off
#ifndef MATRIX_IMPL_HPP
#define MATRIX_IMPL_HPP


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
            auto old_nnz=n_nnz;
            //@note I do not understand what are you doing here.
            (*this)(newrows-1,newcols-1)=default_t;
            n_nnz=old_nnz; ///to be safe
            return;
        }
        if(!compressed){
            for(std::size_t i=0;i<n_rows;++i){
                for(std::size_t j=0;j<n_cols;++j){
                    if(i>=newrows||j>=newcols){
                        std::array<std::size_t,2> key={i,j};
                        //@note I do not understand what you are doing here. YOu should check if the element is  present and if it is so delete it.
                        // rather simple.
                        if(COOmap[key]!=default_t)
                            --n_nnz; ///If the element is not of the default value I decrease the number of non zero elements
                        COOmap.erase(key); ///I erase the elements out of range for the new dimensions
                    }
                }
            }
        }
        else{
            //@note Operating a resize directly on the compressed matrix is very complicated and error prone. I would have
            // uncompressed the matrix, resized it and then compressed it again. SImpler and safer.
            // Anyway, I appreciate the attempt.
            std::size_t diff=n_nnz-inner_indices[newrows]; ///number of elements to erase
            auto last=outer_indices.end(); ///I delete the outer indices corresponding to the non zero elements to erase
            auto first=last-diff;
            outer_indices.erase(first,last);
            auto lastv=values.end(); ///I delete the values corresponding to the non zero elements to erase
            auto firstv=lastv-diff;
            values.erase(firstv,lastv);
            n_nnz-=diff;
            auto last_i=inner_indices.end(); ///I delete the inner indices corresponding to the deleted rows
            auto first_i=last_i-(n_rows-newrows);
            inner_indices.erase(first_i,last_i);
            std::size_t actual_size=outer_indices.size();
            unsigned pos=0;
            while(pos<actual_size){
                auto iterv=values.begin();
                auto itero=outer_indices.begin();
                if(outer_indices[pos]>=newcols){
                    --n_nnz;
                    for(std::size_t k=0;k<inner_indices.size();++k){ ///I adjust the indices
                        if(pos<=inner_indices[k]&&k>0){
                            --inner_indices[k];
                        }
                    }
                    values.erase(iterv+pos);
                    outer_indices.erase(itero+pos);
                     --pos;
                    --actual_size;
                }
                ++pos;
            }
        }
        ///I update the dimensions
        n_rows=newrows;
        n_cols=newcols;
    }

    template<typename T,StorageOrder S>
    void
    Matrix<T,S>::compress(){
        if(compressed) ///If the matrix is already compressed I exit the function
            return;
        inner_indices=std::vector<std::size_t>(n_rows+1);
        outer_indices=std::vector<std::size_t>(n_nnz);
        values=std::vector<T>(n_nnz);
        std::size_t count=0,i=0,row=0;
        bool start_row=true;
        //@note Very complicated. You could have exploited the fact thet the map is ordered. For instance, with
        // auto const start = COOmap.lower_bound({i,0});
        // auto const end = COOmap.upper_bound({i+1,0});
        // you have the iterators from the beginning to past-the-end of row i.
        // So, you can fill your compressed arrays row by row, which is simpler than what you have done. 
        // you can do similarly for column ordering.

              for(auto iter=COOmap.begin();iter!=COOmap.end();++iter){
            if(iter->first[0]!=row){ ///I reset the flag when I go to the next row in the matrix
                start_row=false;
                ++row;
            }
            if(iter->second!=default_t){
                values[i]=iter->second;///store nnz value
                outer_indices[i]=iter->first[1];//store column index
                ++i;
                if(!start_row){///when the row has the first non zero element
                    inner_indices[row]=count; ///store the number of non zero elements before reaching this element
                    start_row=true;
                }
                ++count; ///I count the non-zero elements
            }
            inner_indices[row+1]=count;
        }
        COOmap.clear(); ///I clear the map for the COOmap format
        compressed=true;
    }

    template<typename T,StorageOrder S>
    void
    Matrix<T,S>::uncompress(){
        if(!compressed) ///If already uncompressed I exit
            return;
        ///I start by creating a map of the right length
        ///@note NO, you just have to fill the non zero element by looping the compressed vectors!
        // Again, you are making your life complicated!
        for(std::size_t i=0;i<n_rows;++i){
            for(std::size_t j=0;j<n_cols;++j){
                std::array<std::size_t,2> key={i,j};
                COOmap[key]=default_t;
            }
        }
        unsigned row=0; ///here I keep track of what row I am at
        unsigned counter=inner_indices[row+1]; //this helps me keep track of when I need to update the row index
        for(std::size_t k=0;k<n_nnz;++k){
            if(k==counter&&row<n_rows){
                ++row;
                counter=inner_indices[row+1];
            }
            std::array<std::size_t,2> key={row,outer_indices[k]};
            COOmap[key]=values[k];
        }
        ///clear the vectors
        values.clear();
        outer_indices.clear();
        inner_indices.clear();
        compressed=false;
    }
    template<typename T,StorageOrder S>
    const T & 
    Matrix<T,S>::operator() (const std::size_t & i, const std::size_t &j) const{
        if(i>=n_rows||j>=n_cols){ ///I check if I am within the matrix dimensions
            std::cerr<<"out of bounds"<<std::endl; 
            return default_t; ///I chose not to return a quietNaN to be more general
        }
        if(!compressed){
            std::array<std::size_t,2> key={i,j};
            return COOmap[key];
        }

        unsigned n1=inner_indices[i],n2=n_nnz;
        if(i<n_rows)
            n2=inner_indices[i+1];
        for(std::size_t k=n1;k<n2;++k){///I am cycling through the non null elements of the row
            if(outer_indices[k]==j)
                return values[k]; ///If the element is non null I return it
        }
        //@note No, returning a local constant expression by reference is a very bad idea.
        // What if the user wants to change the value? In the non const version, if the matrix is uncompressed it should be an error to try to
        // access an element that is not present.
        return 0; ///If the element is null I return the default value
    }

    template<typename T,StorageOrder S>
    T & 
    Matrix<T,S>::operator() (const std::size_t & _i, const std::size_t &_j) {
        if(compressed &&(_i>=n_rows||_j>=n_cols)){ ///If the user tries to add a new element when the matrix is in a compressed state, return an error
            std::cerr<<"You cannot add new elements when the matrix is in a compressed state"<<std::endl;
            return default_t;
        }
        if(compressed){
            unsigned n1=inner_indices[_i],n2=n_nnz;
            if(_i<n_rows&&inner_indices[_i+1]<n_nnz)///just to be safe
                n2=inner_indices[_i+1];
            for(std::size_t k=n1;k<n2;++k){///I am cycling through the non null elements of the row
                if(outer_indices[k]==_j){
                    return values[k]; ///If the element is non null I return it
                }
            }
            return default_t; ///If I get a zero element return a default value for T
        }
        if(_i<n_rows&&_j<n_cols){
            std::array<std::size_t,2> key={_i,_j};
            return COOmap[key];
        }
        ///if the element is not within bounds I have to adjust the matrix dimensions
        std::size_t g=0;
        auto n_rows_to_add=std::max((_i+1)-n_rows,g), n_cols_to_add=std::max(_j+1-n_cols,g);
        ///I insert a default value for T to make the matrix have enough rows and columns to be rectangular after inserting the new element
        if(n_cols_to_add>0){
            for(std::size_t i=0;i<n_rows;++i){
                for(std::size_t j=n_cols-1;j<n_cols+n_cols_to_add;++j){
                    COOmap[{i,j}]=default_t; 
                }
            }
        }
        if(n_rows_to_add>0){
            for(std::size_t i=n_rows-1;i<n_rows+n_rows_to_add;++i){
                for(std::size_t j=0;j<n_cols+n_cols_to_add;++j){
                    COOmap[{i,j}]=default_t;
                }
            }
        }
        ///i fix the matrix dimensions
        n_rows+=n_rows_to_add;
        n_cols+=n_cols_to_add;

        return COOmap[{_i,_j}]; ///I return a reference to the new element
    }
    /**
     * @brief friend operator that performs matrix-vector multiplication
     * 
     * @tparam T type of the elements of the matrix (and vector)
     * @param M row ordered matrix
     * @param v vector of type T
     * @return std::vector<T> result of the multiplication
     */
    template<typename T>
    std::vector<T> 
    operator*(Matrix<T,StorageOrder::Rows> & M, std::vector<T> &v){
        if(M.n_cols!=v.size()){ ///dimensions check
                std::cerr<<"incompatible dimensions for matrix vector multiplication"<<std::endl;
                return {};
            }
        std::vector<T> res(M.n_rows,T(0));
        if(M.compressed){
            for(std::size_t i=0;i<M.n_rows;++i){
                std::size_t n_elems=M.inner_indices[i+1], old_n_elems=M.inner_indices[i];
                for(std::size_t k=old_n_elems;k<n_elems;++k){
                    res[i]+=M.values[k]*v[M.outer_indices[k]];
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
    /**
     * @brief friend operator that performs matrix-vector multiplication
     * 
     * @tparam T type of the elements of the matrices 
     * @tparam S Can be either StorageOrder::Rows or StorageOrder::Columns
     * @param M row ordered matrix
     * @param m 1-column matrix
     * @return std::vector<T> resukt of the multiplication
     */
    template<typename T,StorageOrder S>
    std::vector<T> 
    operator*(Matrix<T,StorageOrder::Rows> & M, Matrix<T,S> &m){
        if(M.n_cols!=m.rows()||m.columns()>1){ ///dimensions check
                std::cerr<<"incompatible dimensions for matrix vector multiplication"<<std::endl;
                return {};
            }
        ///I transform the matrix into a vector, then call the operator * that was previously defined
        std::vector<T> v(m.rows());
        if(!m.is_compressed())
            v=m.map_to_vec();
        if(m.is_compressed())
        {       auto m_values=m.get_values();
                auto m_inner_indices=m.get_inner_indices();
                auto m_outer_indices=m.get_outer_indices();
            if(m_inner_indices.size()==m.rows()+1){
                
                for(std::size_t i=0;i<v.size();++i){
                    if(m_inner_indices[i+1]==m_inner_indices[i]){
                        
                        v[i]=M.default_t;
                    }
                    else{
                        v[i]=m_values[m_inner_indices[i+1]];
                    }
                }
            }
            else if(m_inner_indices.size()==m.columns()+1){
                for(std::size_t i=0;i<m.nnz();++i){
                    v[m_outer_indices[i]]=m_values[i];
                }
            }
        }
        auto res=M*v;
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
        if(!f){ ///check if the file has opened correctly
            std::cerr<<"file opening has failed"<<std::endl;
            return Matrix<T,StorageOrder::Rows>();
        }
        if(mm_read_banner(f,&matcode)!=0){ ///check if the banner has been successfully read
            std::cerr<<"Matrix banner was not read successfully"<<std::endl;
            return Matrix<T,StorageOrder::Rows>();
        }
        if(!mm_is_matrix(matcode)||!mm_is_real(matcode)){///check if the matrix is a general real matrix
            std::cerr<<"This program only works with general real matrices"<<std::endl;
            return Matrix<T,StorageOrder::Rows>();
        }
        if(mm_read_mtx_crd_size(f,&rows,&cols,&nnz)!=0){ ///check if the dimensions have been all read correctly
            std::cerr<<"The sizes were not read correctly"<<std::endl;
            return Matrix<T,StorageOrder::Rows>();
        }
        ///I call the constructor
        Matrix<T,StorageOrder::Rows> result(rows,cols);
        result.set_nnz(nnz);
        ///I insert the non zero values in the matrix
        //@note nnz could have been declared as unsigned int of std::size_t. Being just an int you have
        //an annoying warning since in k<nnz you are comparing an unsigned with a signed integer and the compiler
        //warns you about it. It is not a problem here, but quite annoying to have warnings. Solution: declare nnz as unsigned.
        for(std::size_t k=0;k<nnz;++k){
            std::size_t i,j;
            T value;
            //@note  Another warning here. i, and j are declared as size_t, so long unsigned int, so you need to use %lu not %ld 
            fscanf(f,"%ld %ld %lg\n",&i,&j,&value);
            //@note fscanf returns a staatues, so you should check it. 
            result(i-1,j-1)=value; ///It has to be considered that now the indices have to start from 0
        }
        ///I close the file
        if(f!=stdin)
            fclose(f);
        return result;
    }
};



#endif /*MATRIX_IMPL_HPP*/