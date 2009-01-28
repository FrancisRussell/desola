#ifndef DESOLA_BLAS_WRAPPERS_HPP
#define DESOLA_BLAS_WRAPPERS_HPP

#include <vector>
#include <cstddef>
#include <boost/scoped_array.hpp>
#include <algorithm>
#include <desola/file-access/mtl_harwell_boeing_stream.hpp>
#include <desola/file-access/mtl_entry.hpp>

namespace desola
{

namespace blas_wrappers
{

template<typename T>
class BLASVector
{
public:
  typedef T value_type;
  typedef std::size_t size_type;

private:
  size_type numRows;
  boost::scoped_array<value_type> values;

public:
  BLASVector(const size_type rows) : numRows(rows), values(new value_type[rows])
  {
  }

  BLASVector(const size_type rows, const value_type initial) : numRows(rows), values(new value_type[rows])
  {
    std::fill(values.get(), values.get() + numRows, initial);
  }

  BLASVector(const BLASVector& v) : numRows(v.numRows), values(new value_type[numRows])
  {
    std::copy(v.values.get(), v.values.get() + numRows, values.get());
  }

  void resize(const size_type rows)
  {
    value_type* const newValues = new value_type[rows];
    std::copy(values.get(), values.get() + std::min(rows, numRows), newValues);

    numRows = rows;
    values.reset(newValues);
  }

  inline size_type nrows() const
  {
    return numRows;
  }

  inline value_type* data()
  {
    return values.get();
  }

  inline const value_type* data() const
  {
    return values.get();
  }
};


template<typename T>
class BLASGeneralMatrix
{
public:
  typedef T value_type;
  typedef std::size_t size_type;

private:
  const size_type rows;
  const size_type cols;
  boost::scoped_array<value_type> values;

public:
  BLASGeneralMatrix(const size_type r, const size_type c) : rows(r), cols(c), values(new value_type[rows*cols])
  {
  }

  template<typename StreamType>
  explicit BLASGeneralMatrix(StreamType& str) : rows(str.nrows()), cols(str.ncols()), values(new value_type[rows*cols])
  {
    std::fill(values.get(), values.get() + rows*cols, T());

    while(!str.eof())
    {
      desola::entry2<double> entry;
      str >> entry;
      values[ncols()*entry.row + entry.col] = entry.value;
    }
  }

  inline size_type nrows() const
  {
    return rows;
  }

  inline size_type ncols() const
  {
    return cols;
  }

  inline value_type* data()
  {
    return values.get();
  }

  inline const value_type* data() const
  {
    return values.get();
  }
};

}

}
#endif
