#ifndef DESOLIN_ROW_LENGTH_STATISTICS_HPP
#define DESOLIN_ROW_LENGTH_STATISTICS_HPP

#include "Desolin_fwd.hpp"
#include <cstddef>
#include <map>
#include <cassert>
#include <ostream>

namespace desolin
{

namespace detail
{

class RowLengthStatistics
{
private:
  const std::size_t rows;
  std::map<std::size_t, std::size_t> lengths;

public:
  typedef std::map<std::size_t, std::size_t>::iterator iterator;
  typedef std::map<std::size_t, std::size_t>::const_iterator const_iterator;
  typedef std::pair<std::size_t, std::size_t> value_type;

  template<typename T>
  RowLengthStatistics(const CRSMatrix<T>& matrix) : rows(matrix.getRowCount())
  {
    const int* rowPtr = matrix.get_row_ptr();
    assert(rows == matrix.row_ptr_size() - 1);
    
    for(std::size_t row = 0; row<rows; ++row)
      ++lengths[rowPtr[row+1] - rowPtr[row]];
  }

  std::size_t& operator[](const std::size_t index)
  {
    return lengths[index];
  }

  std::size_t operator[](const std::size_t index) const
  {
    const std::map<std::size_t, std::size_t>::const_iterator lengthIter = lengths.find(index);

    if (lengthIter == lengths.end())
      return 0;
    else
      return lengthIter->second;
  }

  iterator begin()
  {
    return lengths.begin();
  }

  const_iterator begin() const
  {
    return lengths.begin();
  }

  iterator end()
  {
    return lengths.end();
  }

  const_iterator end() const
  {
    return lengths.end();
  }

  std::size_t numRows() const
  {
    return rows;
  }
};

}

}

#endif
