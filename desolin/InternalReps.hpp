#ifndef DESOLIN_INTERNAL_REPS_HPP
#define DESOLIN_INTERNAL_REPS_HPP

#include <cassert>
#include <desolin/Desolin_fwd.hpp>

template<typename T_elementType>
class InternalValue
{
protected:
  bool allocated;
public:
  InternalValue() : allocated(false)
  {
  }
  
  inline bool isAllocated() const
  {
    return allocated;
  }
  
  virtual void allocate() = 0;
  virtual ~InternalValue() {}
};

template<typename T_elementType>
class InternalScalar : public InternalValue<T_elementType>
{
public:
  virtual void accept(InternalScalarVisitor<T_elementType>& visitor) = 0;

  virtual T_elementType getElementValue() = 0;
};

template<typename T_elementType>
class InternalVector : public InternalValue<T_elementType>
{
protected:
  const int rows;

public:
  InternalVector(const int rowCount) : rows(rowCount)
  {
  }
  
  const inline int getRowCount() const
  {
    return rows;
  }

  virtual void accept(InternalVectorVisitor<T_elementType>& visitor) = 0;

  virtual T_elementType getElementValue(const ElementIndex<vector>& index) = 0;
};

template<typename T_elementType>
class InternalMatrix : public InternalValue<T_elementType>
{
protected:
  const int rows;
  const int cols;
  
public:
  InternalMatrix(const int rowCount, const int colCount) : rows(rowCount), cols(colCount)
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

  virtual void accept(InternalMatrixVisitor<T_elementType>& visitor) = 0;

  virtual T_elementType getElementValue(const ElementIndex<matrix>& index) = 0;
};

template<typename T_elementType>
class ConventionalScalar : public InternalScalar<T_elementType>
{
private:
  T_elementType value;
  
public:
  ConventionalScalar()
  {
  }

  ConventionalScalar(const T_elementType initialValue)
  {
    value = initialValue;
  }  

  virtual void allocate()
  {
    this->allocated=true;
  }

  virtual void accept(InternalScalarVisitor<T_elementType>& visitor)
  {
    visitor.visit(*this);
  }

  virtual T_elementType getElementValue()
  {
    assert(this->allocated);
    return value;
  } 

  T_elementType* getValue()
  {
    assert(this->allocated);
    return &value;
  }
};

template<typename T_elementType>
class ConventionalVector : public InternalVector<T_elementType>
{
private:
  std::vector<T_elementType> value;
  
public:
  ConventionalVector(const int rowCount) : InternalVector<T_elementType>(rowCount)
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

  virtual void accept(InternalVectorVisitor<T_elementType>& visitor)
  {
    visitor.visit(*this);
  }

  T_elementType* getValue()
  {
    assert(this->allocated);
    return &value[0];
  }

  virtual T_elementType getElementValue(const ElementIndex<vector>& index)
  {
    assert(this->allocated);
    return value[index.getRow()];
  }
};

template<typename T_elementType>
class ConventionalMatrix : public InternalMatrix<T_elementType>
{
private:
  std::vector<T_elementType> value;
  
public:
  ConventionalMatrix(const int rowCount, const int colCount) : InternalMatrix<T_elementType>(rowCount, colCount)
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

  virtual void accept(InternalMatrixVisitor<T_elementType>& visitor)
  {
    visitor.visit(*this);
  }

  T_elementType* getValue()
  {
    assert(this->allocated);
    return &value[0];
  }

  virtual T_elementType getElementValue(const ElementIndex<matrix>& index) 
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
