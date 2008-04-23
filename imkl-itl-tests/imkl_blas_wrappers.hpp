#ifndef DESOLIN_IMKL_BLAS_WRAPPERS_HPP
#define DESOLIN_IMKL_BLAS_WRAPPERS_HPP

#include <vector>
#include <map>
#include <cassert>
#include "blas_wrappers.hpp"

namespace desolin
{

namespace blas_wrappers
{

template<typename T>
class BLASCRSMatrix
{
public:
  typedef T value_type;
  typedef std::size_t size_type;

private:
  const size_type rows;
  const size_type cols;
  std::vector<int> row_ptr;
  std::vector<int> col_ind;
  std::vector<value_type> val;

public:
  template<typename StreamType>
  explicit BLASCRSMatrix(StreamType& stream) : rows(stream.nrows()), cols(stream.ncols())
  {
    std::vector< std::map<std::size_t, T> > matrixData(rows);

    while(!stream.eof())
    {
      mtl::entry2<double> entry;
      stream >> entry;
      assert(entry.row >=0 && entry.col >=0);
      matrixData[entry.row][ entry.col] = entry.value;
    }

    for(size_type row=0; row<rows; ++row)
    {
      row_ptr.push_back(val.size());
      const std::map<std::size_t, T>& mr(matrixData[row]);
      for(typename std::map<std::size_t, T>::const_iterator colIter(mr.begin()); colIter!=mr.end(); ++colIter)
      {
        col_ind.push_back(colIter->first);
        val.push_back(colIter->second);
      }
    }
    row_ptr.push_back(val.size());
  }

  inline size_type nnz() const
  {
    return val.size();
  }

  inline size_type nrows() const
  {
    return rows;
  }

  inline size_type ncols() const
  {
    return cols;
  }

  inline value_type* get_val()
  {
    return &val[0];
  }

  inline const value_type* get_val() const
  {
    return &val[0];
  }
  
  inline int* get_row_ptr()
  {
    return &row_ptr[0];
  }

  inline const int* get_row_ptr() const
  {
    return &row_ptr[0];
  }

  inline int* get_col_ind()
  {
    return &col_ind[0];
  }

  inline const int* get_col_ind() const
  {
    return &col_ind[0];
  }
};

}

}

#endif
