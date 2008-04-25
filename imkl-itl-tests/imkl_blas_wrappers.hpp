#ifndef DESOLIN_IMKL_BLAS_WRAPPERS_HPP
#define DESOLIN_IMKL_BLAS_WRAPPERS_HPP

#include <vector>
#include <cassert>
#include <cstddef>
#include <algorithm>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
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
    std::vector< boost::tuple<std::size_t, std::size_t, T> > matrixData;
    matrixData.reserve(stream.nnz());

    while(!stream.eof())
    {
      mtl::entry2<double> entry;
      stream >> entry;
      assert(entry.row >=0 && entry.col >=0);
      assert(entry.row < static_cast<int>(rows) && entry.col < static_cast<int>(cols));
      matrixData.push_back(boost::make_tuple(entry.row, entry.col, entry.value));
    }

    std::sort(matrixData.begin(), matrixData.end());

    row_ptr.reserve(rows+1);
    col_ind.reserve(stream.nnz());
    val.reserve(stream.nnz());

    std::size_t currentRow = 0;
    row_ptr.push_back(0);

    for(std::size_t index=0; index<matrixData.size(); ++index)
    {
      const boost::tuple<std::size_t, std::size_t, T>& entry(matrixData[index]);

      while(currentRow < boost::get<0>(entry))
      {
        row_ptr.push_back(val.size());
        ++currentRow;
      }
      
      col_ind.push_back(boost::get<1>(entry));
      val.push_back(boost::get<2>(entry));
    }
    row_ptr.push_back(val.size());

    assert(row_ptr.size() == rows + 1);
    assert(static_cast<int>(col_ind.size()) == stream.nnz());
    assert(static_cast<int>(val.size()) == stream.nnz());
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
