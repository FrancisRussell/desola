#ifndef DESOLIN_BLAS_WRAPPERS_HPP
#define DESOLIN_BLAS_WRAPPERS_HPP

#include<vector>
#include<cstddef>
#include<mtl/harwell_boeing_stream.h>
#include<mtl/entry.h>

namespace desolin
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
  std::vector<value_type> values;

public:
  BLASVector(const size_type rows) : values(rows)
  {
  }

  BLASVector(const size_type rows, const value_type initial) : values(rows, initial)
  {
  }

  void resize(const size_type rows)
  {
    values.resize(rows);
  }

  inline size_type nrows() const
  {
    return values.size();
  }

  inline value_type* data()
  {
    return &values[0];
  }

  inline const value_type* data() const
  {
    return &values[0];
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
  std::vector<value_type> values;

public:
  BLASGeneralMatrix(const size_type r, const size_type c) : rows(r), cols(c), values(rows*cols, 0)
  {
  }

  template<typename StreamType>
  explicit BLASGeneralMatrix(StreamType& str) : rows(str.nrows()), cols(str.ncols()), values(rows*cols, 0)
  {
    while(!str.eof())
    {
      mtl::entry2<double> entry;
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
    return &values[0];
  }

  inline const value_type* data() const
  {
    return &values[0];
  }
};

}

}
#endif
