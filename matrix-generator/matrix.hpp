#ifndef DESOLA_MATRIX_GENERATOR_MATRIX_HPP
#define DESOLA_MATRIX_GENERATOR_MATRIX_HPP

#include<desola/iohb/iohb.h>
#include<boost/numeric/ublas/matrix.hpp>
#include<boost/numeric/ublas/matrix_proxy.hpp>
#include<cassert>
#include<cstddef>
#include<vector>
#include<string>


template<typename T>
class CCSMatrix
{
public:
  typedef T value_type;

private:
  const std::size_t rows;
  const std::size_t cols;
  const bool symmetric;
  std::vector<int> col_ptr;
  std::vector<int> row_ind;
  std::vector<value_type> val;

public:
  template<typename UblasSparseMatrix>
  CCSMatrix(UblasSparseMatrix& matrix, const bool s) : rows(matrix.size1()), cols(matrix.size2()), symmetric(s)
  {
    for(std::size_t col=0; col<cols; ++col)
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
   assert(rows == cols);
   const char* title = "Test title";
   const char* key = "Test key";
   const double *rhs = NULL, *guess=NULL, *exact=NULL;
   const char* type = (symmetric ? "RSA" : "RUA");
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
