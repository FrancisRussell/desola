#ifndef DESOLIN_INTERNAL_REPS_HPP
#define DESOLIN_INTERNAL_REPS_HPP

#include <cassert>
#include <desolin/Desolin_fwd.hpp>

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

  virtual void accept(InternalMatrixVisitor<T_element>& visitor) = 0;

  virtual T_element getElementValue(const ElementIndex<matrix>& index) = 0;
};

template<typename T_element>
class ConventionalScalar : public InternalScalar<T_element>
{
private:
  T_element value;
  
public:
  ConventionalScalar() : InternalScalar<T_element>(true)
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
      value.reserve(this->rows);
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

  ConventionalMatrix(const int rowCount, const int colCount, const T_element initialValue) : InternalMatrix<T_element>(true, rowCount, colCount), value(rowCount*colCount, initialValue)
  {
  }

  virtual void allocate()
  {
    if(!this->allocated)
    {
      value.reserve(this->rows * this->cols);
      this->allocated=true;
    }
  }

  virtual void accept(InternalMatrixVisitor<T_element>& visitor)
  {
    visitor.visit(*this);
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

template<typename T_elementType>
class InternalScalarVisitor
{
public:
  virtual void visit(ConventionalScalar<T_elementType>& value) = 0;
};

template<typename T_elementType>
class InternalVectorVisitor
{
public:
  virtual void visit(ConventionalVector<T_elementType>& value) = 0;
}; 

template<typename T_elementType>
class InternalMatrixVisitor
{
public:
  virtual void visit(ConventionalMatrix<T_elementType>& value) = 0;
};

#endif
