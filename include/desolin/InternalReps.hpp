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
#include <map>
#include <vector>
#include <desolin/Desolin_fwd.hpp>
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
  const int rows;

public:
  InternalVector(const bool allocated, const int rowCount) : InternalValue<T_element>(allocated), rows(rowCount)
  {
  }
  
  const inline int getRowCount() const
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
  const int rows;
  const int cols;
  
public:
  InternalMatrix(const bool allocated, const int rowCount, const int colCount) : InternalValue<T_element>(allocated), rows(rowCount), cols(colCount)
  {
  }
  
  const inline int getRowCount() const
  {
    return rows;
  }

  const inline int getColCount() const
  {
    return cols;
  }

  virtual int nnz() const = 0;

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
  std::vector<T_element> value;
  
public:
  ConventionalVector(const int rowCount) : InternalVector<T_element>(false, rowCount)
  {
  }

  ConventionalVector(const int rowCount, const T_element initialValue) : InternalVector<T_element>(true, rowCount), value(rowCount, initialValue)
  {
  }

  virtual void allocate()
  {
    if(!this->allocated)
    {
      value.resize(this->rows, T_element(0));
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
    return &value[0];
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
  std::vector<T_element> value;
  
public:
  ConventionalMatrix(const int rowCount, const int colCount) : InternalMatrix<T_element>(false, rowCount, colCount)
  {
  }

  template<typename StreamType>
  explicit ConventionalMatrix(StreamType& stream) : InternalMatrix<T_element>(true, stream.nrows(), stream.ncols()), value(stream.nrows()*stream.ncols())
  {
    while(!stream.eof())
    {
        entry2<double> entry;
        stream >> entry;
	value[this->getColCount()*entry.row + entry.col] = entry.value;
    }
  }
  
  virtual void allocate()
  {
    if(!this->allocated)
    {
      value.resize(this->rows * this->cols, T_element());
      this->allocated=true;
    }
  }

  virtual void accept(InternalMatrixVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
  }

  virtual int nnz() const
  {
    return this->rows * this->cols;
  }

  T_element* getValue()
  {
    assert(this->allocated);
    return &value[0];
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
  CRSMatrix(const int rowCount, const int colCount) : InternalMatrix<T_element>(false, rowCount, colCount)
  {
  }

  template<typename StreamType>
  explicit CRSMatrix(StreamType& stream) : InternalMatrix<T_element>(true, stream.nrows(), stream.ncols())
  {
    std::vector< std::map<std::size_t, T_element> > matrixData(this->getRowCount());

    while(!stream.eof())
    {
      entry2<double> entry;
      stream >> entry;
      assert(entry.row >= 0 && entry.col >=0);
      matrixData[entry.row][entry.col] = entry.value;
    }

    for(int row=0; row<this->getRowCount(); ++row)
    {
      row_ptr.push_back(val.size());
      const std::map<std::size_t, T_element>& mr(matrixData[row]);
      for(typename std::map<std::size_t, T_element>::const_iterator colIter(mr.begin()); colIter!=mr.end(); ++colIter)
      {
        col_ind.push_back(colIter->first);
        val.push_back(colIter->second);
      }
    }
    row_ptr.push_back(val.size());
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

  int nnz() const
  {
    return col_ind.size();
  }

  unsigned row_ptr_size() const
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

  virtual T_element getElementValue(const ElementIndex<matrix>& index) 
  {
    assert(this->allocated);
    for(int valPtr = row_ptr[index.getRow()]; valPtr < row_ptr[index.getRow()+1]; ++valPtr)
      if (col_ind[valPtr] == index.getCol())
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
