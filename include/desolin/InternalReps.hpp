/****************************************************************************/
/* Copyright 2005-2006, Francis Russell                                     */
/*                                                                          */
/* Licensed under the Apache License, Version 2.0 (the License);            */
/* you may not use this file except in compliance with the License.         */
/* You may obtain a copy of the License at                                  */
/*                                                                          */
/*     http://www.apache.org/licenses/LICENSE-2.0                           */
/*                                                                          */
/* Unless required by applicable law or agreed to in writing, software      */
/* distributed under the License is distributed on an AS IS BASIS,          */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/* See the License for the specific language governing permissions and      */
/* limitations under the License.                                           */
/*                                                                          */
/****************************************************************************/

#ifndef DESOLIN_INTERNAL_REPS_HPP
#define DESOLIN_INTERNAL_REPS_HPP

#include <cassert>
#include <vector>
#include <cstddef>
#include <algorithm>
#include <desolin/Desolin_fwd.hpp>
#include <boost/scoped_array.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <desolin/file-access/mtl_entry.hpp>

namespace desolin
{

namespace detail
{

template<typename T_element>
class InternalValue
{
protected:
  bool allocated;

public:
  InternalValue(const bool isAllocated) : allocated(isAllocated) 
  {
  }
  
  inline bool isAllocated() const
  {
    return allocated;
  }
  
  virtual void allocate() = 0;

  virtual ~InternalValue() {}
};

template<typename T_element>
class InternalScalar : public InternalValue<T_element>
{
public:
  InternalScalar(const bool allocated) : InternalValue<T_element>(allocated)
  {
  }
  
  virtual void accept(InternalScalarVisitor<T_element>& visitor) = 0;

  virtual T_element getElementValue() = 0;
};

template<typename T_element>
class InternalVector : public InternalValue<T_element>
{
protected:
  const std::size_t rows;

public:
  InternalVector(const bool allocated, const std::size_t rowCount) : InternalValue<T_element>(allocated), rows(rowCount)
  {
  }
  
  const inline std::size_t getRowCount() const
  {
    return rows;
  }

  virtual void accept(InternalVectorVisitor<T_element>& visitor) = 0;

  virtual T_element getElementValue(const ElementIndex<vector>& index) = 0;
};

template<typename T_element>
class InternalMatrix : public InternalValue<T_element>
{
protected:
  const std::size_t rows;
  const std::size_t cols;
  
public:
  InternalMatrix(const bool allocated, const std::size_t rowCount, const std::size_t colCount) : InternalValue<T_element>(allocated), rows(rowCount), cols(colCount)
  {
  }
  
  const inline std::size_t getRowCount() const
  {
    return rows;
  }

  const inline std::size_t getColCount() const
  {
    return cols;
  }

  virtual std::size_t nnz() const = 0;

  virtual void accept(InternalMatrixVisitor<T_element>& visitor) = 0;

  virtual T_element getElementValue(const ElementIndex<matrix>& index) = 0;
};

template<typename T_element>
class ConventionalScalar : public InternalScalar<T_element>
{
private:
  T_element value;
  
public:
  ConventionalScalar() : InternalScalar<T_element>(true), value(T_element(0))
  {
  }

  ConventionalScalar(const T_element initialValue) : InternalScalar<T_element>(true), value(initialValue)
  {
  }  

  virtual void allocate()
  {
    this->allocated=true;
  }

  virtual void accept(InternalScalarVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }

  virtual T_element getElementValue()
  {
    assert(this->allocated);
    return value;
  } 

  T_element* getValue()
  {
    assert(this->allocated);
    return &value;
  }
};

template<typename T_element>
class ConventionalVector : public InternalVector<T_element>
{
private:
  boost::scoped_array<T_element> value;
  
public:
  ConventionalVector(const std::size_t rowCount) : InternalVector<T_element>(false, rowCount), value(NULL)
  {
  }

  ConventionalVector(const std::size_t rowCount, const T_element initialValue) : InternalVector<T_element>(true, rowCount), value(new T_element[rowCount])
  {
    std::fill(value.get(), value.get() + this->rows, initialValue);
  }

  virtual void allocate()
  {
    if(!this->allocated)
    {
      value.reset(new T_element[this->rows]);
      this->allocated=true;
    }
  }

  virtual void accept(InternalVectorVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }

  T_element* getValue()
  {
    assert(this->allocated);
    return value.get();
  }

  virtual T_element getElementValue(const ElementIndex<vector>& index)
  {
    assert(this->allocated);
    return value[index.getRow()];
  }
};

template<typename T_element>
class ConventionalMatrix : public InternalMatrix<T_element>
{
private:
  boost::scoped_array<T_element> value;
  
public:
  ConventionalMatrix(const std::size_t rowCount, const std::size_t colCount) : InternalMatrix<T_element>(false, rowCount, colCount), value(NULL)
  {
  }

  template<typename StreamType>
  explicit ConventionalMatrix(StreamType& stream) : InternalMatrix<T_element>(true, stream.nrows(), stream.ncols()), value(new T_element[this->rows*this->cols])
  {
    while(!stream.eof())
    {
        entry2<double> entry;
        stream >> entry;
	value[this->cols*entry.row + entry.col] = entry.value;
    }
  }
  
  virtual void allocate()
  {
    if(!this->allocated)
    {
      value.reset(new T_element(this->rows * this->cols));
      this->allocated=true;
    }
  }

  virtual void accept(InternalMatrixVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }

  virtual std::size_t nnz() const
  {
    return this->rows * this->cols;
  }

  T_element* getValue()
  {
    assert(this->allocated);
    return value.get();
  }

  virtual T_element getElementValue(const ElementIndex<matrix>& index) 
  {
    assert(this->allocated);
    return value[(this->cols*index.getRow())+index.getCol()];
  }
};

template<typename T_element>
class CRSMatrix : public InternalMatrix<T_element>
{
private:
  std::vector<int> col_ind;
  std::vector<int> row_ptr;
  std::vector<T_element> val;
  
public:
  CRSMatrix(const std::size_t rowCount, const std::size_t colCount) : InternalMatrix<T_element>(false, rowCount, colCount)
  {
  }

  template<typename StreamType>
  explicit CRSMatrix(StreamType& stream) : InternalMatrix<T_element>(true, stream.nrows(), stream.ncols())
  {
    std::vector< boost::tuple<std::size_t, std::size_t, T_element> > matrixData;
    matrixData.reserve(stream.nnz());

    while(!stream.eof())
    {
      entry2<double> entry;
      stream >> entry;
      assert(entry.row >= 0 && entry.col >=0);
      assert(entry.row < static_cast<int>(this->getRowCount()) && entry.col < static_cast<int>(this->getColCount()));
      matrixData.push_back(boost::make_tuple(entry.row, entry.col, entry.value));
    }

    std::sort(matrixData.begin(), matrixData.end());

    row_ptr.reserve(this->getRowCount() + 1);
    col_ind.reserve(stream.nnz());
    val.reserve(stream.nnz());

    std::size_t currentRow = 0;
    row_ptr.push_back(0);

    for(std::size_t index=0; index<matrixData.size(); ++index)
    {
      const boost::tuple<std::size_t, std::size_t, T_element>& entry(matrixData[index]);

      while(currentRow < boost::get<0>(entry))
      {
        row_ptr.push_back(val.size());
        ++currentRow;
      }
      
      col_ind.push_back(boost::get<1>(entry));
      val.push_back(boost::get<2>(entry));
    }
    row_ptr.push_back(val.size());

    assert(row_ptr.size() == this->getRowCount() + 1);
    assert(static_cast<int>(col_ind.size()) == stream.nnz());
    assert(static_cast<int>(val.size()) == stream.nnz());
  }

  virtual void allocate()
  {
    if(!this->allocated)
    {
      row_ptr.resize(this->getRowCount()+1, 0);
      this->allocated=true;
    }
  }

  virtual void accept(InternalMatrixVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }

  std::size_t nnz() const
  {
    return col_ind.size();
  }

  std::size_t row_ptr_size() const
  {
    return row_ptr.size();
  }

  int* get_col_ind()
  {
    return &col_ind[0];
  }

  int* get_row_ptr()
  {
    return &row_ptr[0];
  }

  T_element* get_val()
  {
    return &val[0];
  }

  const int* get_col_ind() const
  {
    return &col_ind[0];
  }

  const int* get_row_ptr() const
  {
    return &row_ptr[0];
  }

  const T_element* get_val() const
  {
    return &val[0];
  }

  virtual T_element getElementValue(const ElementIndex<matrix>& index) 
  {
    assert(this->allocated);
    for(int valPtr = row_ptr[index.getRow()]; valPtr < row_ptr[index.getRow()+1]; ++valPtr)
      if (col_ind[valPtr] == static_cast<int>(index.getCol()))
        return val[valPtr];

    return T_element();
  }
};

template<typename T_elementType>
class InternalScalarVisitor
{
public:
  virtual void visit(ConventionalScalar<T_elementType>& value) = 0;
  virtual ~InternalScalarVisitor() {}
};

template<typename T_elementType>
class InternalVectorVisitor
{
public:
  virtual void visit(ConventionalVector<T_elementType>& value) = 0;
  virtual ~InternalVectorVisitor() {}
}; 

template<typename T_elementType>
class InternalMatrixVisitor
{
public:
  virtual void visit(ConventionalMatrix<T_elementType>& value) = 0;
  virtual void visit(CRSMatrix<T_elementType>& value) = 0;
  virtual ~InternalMatrixVisitor() {}
};

}

}
#endif
