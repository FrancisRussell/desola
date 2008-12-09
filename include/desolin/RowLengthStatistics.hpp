#ifndef DESOLIN_ROW_LENGTH_STATISTICS_HPP
#define DESOLIN_ROW_LENGTH_STATISTICS_HPP

#include <cstddef>
#include <map>
#include <cassert>

namespace desolin
{

namespace detail
{

class RowLengthStatistics
{
private:
  std::map<std::size_t, std::size_t> lengths;

public:
  template<typename T>
  RowLengthStatistics(const CRSMatrix<T>& matrix)
  {
    const std::size_t numRows = matrix.getRowCount();
    const int* rowPtr = matrix.get_row_ptr();

    assert(numRows == matrix.row_ptr_size() - 1);
    
    for(std::size_t row = 0; row<numRows; ++row)
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
};

}

}

#endif
