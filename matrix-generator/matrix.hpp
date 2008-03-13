#ifndef DESOLIN_MATRIX_GENERATORS_MATRIX_HPP
#define DESOLIN_MATRIX_GENERATORS_MATRIX_HPP

#include<desolin/file-access/iohb.h>
#include<boost/numeric/ublas/matrix.hpp>
#include<boost/numeric/ublas/matrix_proxy.hpp>
#include<cassert>
#include<vector>
#include<string>


template<typename T>
class CCSMatrix
{
public:
  typedef T value_type;

private:
  const unsigned rows;
  const unsigned cols;
  std::vector<int> col_ptr;
  std::vector<int> row_ind;
  std::vector<value_type> val;

public:
  template<typename UblasSparseMatrix>
  CCSMatrix(UblasSparseMatrix& matrix) : rows(matrix.size1()), cols(matrix.size2())
  {
    for(int col=0; col<cols; ++col)
    {
      col_ptr.push_back(val.size() + 1);
      typedef boost::numeric::ublas::matrix_column<UblasSparseMatrix> column_t;
      column_t mc(matrix, col);
      for(typename column_t::const_iterator rowIter(mc.begin()); rowIter!=mc.end(); ++rowIter)
      {
        row_ind.push_back(rowIter.index() + 1);
        val.push_back(*rowIter);
      }
    }
    // We need to push a final column pointer to signal the end of the matrix
    col_ptr.push_back(val.size() + 1);
  }

  std::size_t nnz() const
  {
    return val.size();
  }

  void writeToFile(const std::string& fileName) const
  {
   const char* title = "Test title";
   const char* key = "Test key";
   const double *rhs = NULL, *guess=NULL, *exact=NULL;
   const char* type = "RUA";
   char *Ptrfmt=NULL, *Indfmt=NULL, *Valfmt=NULL, *Rhsfmt=NULL;
   const char* Rhstype=NULL;

   writeHB_mat_double(fileName.c_str(), rows, cols, val.size(), 
                        &col_ptr[0], &row_ind[0], &val[0], 
                        0, rhs, guess, exact,
                        title, key, type, 
                        Ptrfmt, Indfmt, Valfmt, Rhsfmt,
                        Rhstype);
  }
};

#endif
