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

#ifndef DESOLIN_TG_OBJECTS_HPP
#define DESOLIN_TG_OBJECTS_HPP

#include <typeinfo>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <string>
#include <map>
#include <TaskGraph>
#include <boost/functional/hash.hpp>
#include <boost/function.hpp>
#include "Desolin_tg_fwd.hpp"
#include "TaskGraphWrappers.hpp"

namespace desolin
{

namespace detail
{
	
template<typename T_element>
class TGScalar
{
public:
  virtual const TGScalarExpr<T_element> getExpression() const = 0;
  virtual void setExpression(const TGScalarExpr<T_element>& e) = 0;
  virtual void addExpression(const TGScalarExpr<T_element>& e) = 0;
  virtual InternalScalar<T_element>* createInternalRep() const = 0;
  virtual bool isParameter() const = 0;
  virtual void addParameterMappings(InternalScalar<T_element>& internal, ParameterHolder& params) const = 0;
  virtual std::size_t hashValue() const = 0;
  virtual bool matches(const TGScalar& s) const = 0;
  virtual void createTaskGraphVariable() = 0;
  virtual ~TGScalar() {}
};

template<typename T_element>
class TGVector
{
public:
  virtual const TGScalarExpr<T_element> getExpression(const tg::TaskExpression& row) const = 0;
  virtual void setExpression(const tg::TaskExpression& row, const TGScalarExpr<T_element>& e) = 0;
  virtual void addExpression(const tg::TaskExpression& row, const TGScalarExpr<T_element>& e) = 0;
  virtual std::size_t getRows() const = 0;
  virtual InternalVector<T_element>* createInternalRep() const = 0;
  virtual bool isParameter() const = 0;
  virtual void addParameterMappings(InternalVector<T_element>& internal, ParameterHolder& params) const = 0;
  virtual std::size_t hashValue() const = 0;
  virtual bool matches(const TGVector& v) const = 0;
  virtual void createTaskGraphVariable() = 0;
  virtual ~TGVector() {}
};

template<typename T_element>
class TGMatrix
{
public:
  typedef boost::function<void (NameGenerator& generator, const tg::TaskExpression& row, const tg::TaskExpression& col, const TGScalarExpr<T_element>& value)> MatrixIterationCallback;

  virtual const TGScalarExpr<T_element> getExpression(NameGenerator& generator, const tg::TaskExpression& row, const tg::TaskExpression& col) const = 0;
  virtual void setExpression(NameGenerator& generator, const tg::TaskExpression& row, const tg::TaskExpression& col, const TGScalarExpr<T_element>& e) = 0;  
  virtual void addExpression(NameGenerator& generator, const tg::TaskExpression& row, const tg::TaskExpression& col, const TGScalarExpr<T_element>& e) = 0;
  virtual std::size_t getRows() const = 0;
  virtual std::size_t getCols() const = 0;
  virtual void iterateDense(NameGenerator& generator, MatrixIterationCallback& callback) const = 0;
  virtual void iterateSparse(NameGenerator& generator, MatrixIterationCallback& callback) const = 0;
  virtual InternalMatrix<T_element>* createInternalRep() const = 0;
  virtual bool isParameter() const = 0;
  virtual void addParameterMappings(InternalMatrix<T_element>& internal, ParameterHolder& params) const = 0;
  virtual std::size_t hashValue() const = 0;
  virtual bool matches(const TGMatrix& m) const = 0;
  virtual void createTaskGraphVariable() = 0;
  virtual ~TGMatrix() {}
};

template<typename T_element>
class TGScalarExpr
{
  const tg::TaskExpression expr;

public:
  TGScalarExpr() : expr(T_element())
  {
  }
  
  TGScalarExpr(const tg::TaskExpression& e) : expr(e)
  {
  }

  inline const tg::TaskExpression getExpression() const
  {
    return expr;
  }

  inline const TGScalarExpr mul(const TGScalarExpr& right) const
  {
    return TGScalarExpr(expr*right.expr);
  }

  inline const TGScalarExpr div(const TGScalarExpr& right) const
  {
    return TGScalarExpr(expr/right.expr);
  }

  inline const TGScalarExpr add(const TGScalarExpr& right) const
  {
    return TGScalarExpr(expr+right.expr);
  }

  inline const TGScalarExpr sub(const TGScalarExpr& right) const
  {
    return TGScalarExpr(expr-right.expr);
  }

  inline const TGScalarExpr sqrt() const
  {
    return TGScalarExpr(tSqrt(expr));
  }

  inline const TGScalarExpr abs() const
  {
    return TGScalarExpr(tAbs(expr));
  }

  inline const TGScalarExpr negate() const
  {
    return TGScalarExpr(-expr);
  }
};

template<typename T_element>
class TGConventionalScalar : public TGScalar<T_element>
{
  friend class Mapper;
  
private:
  const bool parameter;
  const std::string name;
  TaskScalarVariableWrapper<T_element> value;

  static inline const std::string getPrefix()
  {
    return std::string("convScalar");
  }
  
public:
  TGConventionalScalar(NameGenerator& generator, const ConventionalScalar<T_element>& internal) : parameter(true), name(generator.getName(getPrefix())), value(true, name)
  {
  }

  TGConventionalScalar(bool param, NameGenerator& generator, const ExprNode<scalar, T_element>& s) : parameter(param), name(generator.getName(getPrefix())), value(param, name)
  {
  }

  const TGScalarExpr<T_element> getExpression() const
  {
    return TGScalarExpr<T_element>(*value);
  }

  void setExpression(const TGScalarExpr<T_element>& e)
  {
    *value = e.getExpression();
  }

  void addExpression(const TGScalarExpr<T_element>& e)
  {
    *value += e.getExpression();
  }

  virtual InternalScalar<T_element>* createInternalRep() const
  {
    assert(parameter);
    return new ConventionalScalar<T_element>();
  }

  virtual bool isParameter() const
  {
    return parameter;
  }

  virtual std::size_t hashValue() const
  {
    const char* nodeTypeString = typeid(*this).name();
    std::size_t seed = boost::hash_range(nodeTypeString, nodeTypeString+strlen(nodeTypeString));
    boost::hash_combine(seed, parameter);
    boost::hash_combine(seed, name);
    return seed;
  }

  virtual bool matches(const TGScalar<T_element>& s) const
  {
    if (typeid(s) == typeid(*this))
    {
      const TGConventionalScalar& right = static_cast<const TGConventionalScalar&>(s);
      return parameter == right.parameter &&
	     name == right.name;
    }
    else
    {
      return false;
    }
  }
  
  virtual void createTaskGraphVariable()
  {
    value.instantiate();
  }

  virtual void addParameterMappings(InternalScalar<T_element>& internal, ParameterHolder& params) const
  {
    assert(parameter);
    Mapper mapper(*this, params);
    internal.accept(mapper);
  }

  class Mapper : public InternalScalarVisitor<T_element>
  {
  private:
    const TGConventionalScalar<T_element>& internal;
    ParameterHolder& parameterHolder;
  
  public:
    Mapper(const TGConventionalScalar<T_element>& i, ParameterHolder& p) : internal(i), parameterHolder(p)
    {
    }
    
    virtual void visit(ConventionalScalar<T_element>& s)
    {
      assert(s.isAllocated());
      parameterHolder.addParameter(internal.name, s.getValue()); 
    }
  };
};

template<typename T_element>
class TGConventionalVector : public TGVector<T_element>
{
private:
  const bool parameter;
  const std::string name;
  const std::size_t rows;
  TaskArrayWrapper<T_element, 1> value;

  static inline const std::string getPrefix()
  {
    return std::string("convVector");
  }
  
public:
  TGConventionalVector(NameGenerator& generator, const ConventionalVector<T_element>& internal) :  parameter(true), name(generator.getName(getPrefix())), rows(internal.getRowCount()), value(true, name, rows)
  {
  }

  TGConventionalVector(bool param, NameGenerator& generator, const ExprNode<vector, T_element>& v) :  parameter(param), name(generator.getName(getPrefix())), rows(v.getRowCount()), value(param, name, rows)
  {
  }
  
  const TGScalarExpr<T_element> getExpression(const tg::TaskExpression& row) const
  {
    return TGScalarExpr<T_element>((*value)[row]);
  }

  void setExpression(const tg::TaskExpression& row, const TGScalarExpr<T_element>& e)
  {
    (*value)[row] = e.getExpression();
  }

  void addExpression(const tg::TaskExpression& row, const TGScalarExpr<T_element>& e)
  {
    (*value)[row] += e.getExpression();
  }

  virtual std::size_t getRows() const
  {
    return rows;
  }

  virtual InternalVector<T_element>* createInternalRep() const
  {
    assert(parameter);
    return new ConventionalVector<T_element>(rows);
  }

  virtual bool isParameter() const
  {
    return parameter;
  }

  virtual void addParameterMappings(InternalVector<T_element>& internal, ParameterHolder& params) const
  {
    assert(parameter);
    Mapper mapper(*this, params);
    internal.accept(mapper);
  }
  
  virtual std::size_t hashValue() const
  {
    const char* nodeTypeString = typeid(*this).name();
    std::size_t seed = boost::hash_range(nodeTypeString, nodeTypeString+strlen(nodeTypeString));
    boost::hash_combine(seed, parameter);
    boost::hash_combine(seed, rows);
    boost::hash_combine(seed, name);
    return seed;
  }

  virtual bool matches(const TGVector<T_element>& v) const
  {
    if (typeid(v) == typeid(*this))
    {
      const TGConventionalVector& right = static_cast<const TGConventionalVector&>(v);
      return parameter == right.parameter &&
             name == right.name &&
	     rows == right.rows;
    }
    else
    {
      return false;
    }
  }
    
  virtual void createTaskGraphVariable()
  {
    value.instantiate();
  }

  class Mapper : public InternalVectorVisitor<T_element>
  {
  private:
    const TGConventionalVector<T_element>& internal;
    ParameterHolder& parameterHolder;

  public:
    Mapper(const TGConventionalVector<T_element>& i, ParameterHolder& p) : internal(i), parameterHolder(p)
    {
    }

    virtual void visit(ConventionalVector<T_element>& v)
    {
      assert(v.isAllocated());
      parameterHolder.addParameter(internal.name, v.getValue());
    }
  };
    
};

template<typename T_element>
class TGConventionalMatrix : public TGMatrix<T_element>
{
private:
  const bool parameter;
  const std::string name;
  const std::size_t rows;
  const std::size_t cols;
  TaskArrayWrapper<T_element, 2> value;

  static inline const std::string getPrefix()
  {
    return std::string("convMatrix");
  }
      
public:
  typedef typename TGMatrix<T_element>::MatrixIterationCallback MatrixIterationCallback;

  TGConventionalMatrix(NameGenerator& generator, ConventionalMatrix<T_element>& internal) : parameter(true), name(generator.getName(getPrefix())),  rows(internal.getRowCount()), cols(internal.getColCount()), value(true, name, rows, cols)
  {
  }

  TGConventionalMatrix(bool param, NameGenerator& generator, const ExprNode<matrix, T_element>& m) :  parameter(param), name(generator.getName(getPrefix())), rows(m.getRowCount()), cols(m.getColCount()), value(param, name, rows, cols)
  {
  }

  const TGScalarExpr<T_element> getExpression(NameGenerator& generator, const tg::TaskExpression& row, const tg::TaskExpression& col) const
  {
    return TGScalarExpr<T_element>((*value)[row][col]);
  }

  void setExpression(NameGenerator& generator, const tg::TaskExpression& row, const tg::TaskExpression& col, const TGScalarExpr<T_element>& e)
  {
    (*value)[row][col] = e.getExpression();
  }

  void addExpression(NameGenerator& generator, const tg::TaskExpression& row, const tg::TaskExpression& col, const TGScalarExpr<T_element>& e)
  {
    (*value)[row][col] += e.getExpression();
  }
  
  virtual std::size_t getRows() const
  {
    return rows;
  }

  virtual std::size_t getCols() const
  {
    return cols;
  }

  virtual void iterateDense(NameGenerator& generator, MatrixIterationCallback& callback) const
  {
    using namespace tg;

    tVarNamed(unsigned, i, generator.getName("ConvMatrix_row").c_str());
    tVarNamed(unsigned, j, generator.getName("ConvMatrix_col").c_str());

    tFor(i, 0u, rows-1)
    {
      tFor(j, 0u, cols-1)
      {
        callback(generator, i, j, getExpression(generator, i, j));
      }
    }
  }

  virtual void iterateSparse(NameGenerator& generator, MatrixIterationCallback& callback) const
  {
    iterateDense(generator, callback);
  }

  virtual InternalMatrix<T_element>* createInternalRep() const
  {
    assert(parameter);
    return new ConventionalMatrix<T_element>(rows, cols);
  }

  virtual bool isParameter() const
  {
    return parameter;
  }

  virtual void addParameterMappings(InternalMatrix<T_element>& internal, ParameterHolder& params) const
  {
    assert(parameter);
    Mapper mapper(*this, params);
    internal.accept(mapper);
  }

  virtual std::size_t hashValue() const
  {
    const char* nodeTypeString = typeid(*this).name();
    std::size_t seed = boost::hash_range(nodeTypeString, nodeTypeString+strlen(nodeTypeString));
    boost::hash_combine(seed, parameter);
    boost::hash_combine(seed, rows);
    boost::hash_combine(seed, cols);
    boost::hash_combine(seed, name);
    return seed;
  }

  virtual bool matches(const TGMatrix<T_element>& m) const
  {
    if (typeid(m) == typeid(*this))
    {
      const TGConventionalMatrix& right = static_cast<const TGConventionalMatrix&>(m);
      return parameter == right.parameter &&
             name == right.name &&
             rows == right.rows &&
	     cols == right.cols;
    }
    else
    {
      return false;
    }
  }
   
  virtual void createTaskGraphVariable()
  {
    value.instantiate();
  }

  class Mapper : public InternalMatrixVisitor<T_element>
  {
  private:
    const TGConventionalMatrix<T_element>& internal;
    ParameterHolder& parameterHolder;

  public:
    Mapper(const TGConventionalMatrix<T_element>& i, ParameterHolder& p) : internal(i), parameterHolder(p)
    {
    }

    virtual void visit(ConventionalMatrix<T_element>& m)
    {
      assert(m.isAllocated());
      parameterHolder.addParameter(internal.name, m.getValue());
    }    

    virtual void visit(CRSMatrix<T_element>& m)
    {
      // FIXME: Handle this error better
      assert(0 && "Attempted to map parameters for CRSMatrix instead of a ConventionalMatrix");
    }
  };
};

template<typename T_element>
class TGCRSMatrix : public TGMatrix<T_element>
{
private:
  static inline std::string getColIndPrefix()
  {
    return std::string("colInd");
  }

  static inline std::string getRowPtrPrefix()
  {
    return std::string("rowPtr");
  }

  static inline std::string getValPrefix()
  {
    return std::string("val");
  }

  const bool parameter;
  const std::string col_ind_name;
  const std::string row_ptr_name;
  const std::string val_name; 
  const std::size_t nnz;
  const std::size_t rows;
  const std::size_t cols;
  TaskArrayWrapper<int, 1> col_ind;
  TaskArrayWrapper<int, 1> row_ptr;
  TaskArrayWrapper<T_element, 1> val;

public:
  typedef typename TGMatrix<T_element>::MatrixIterationCallback MatrixIterationCallback;

  TGCRSMatrix(NameGenerator& generator, CRSMatrix<T_element>& internal) : parameter(true), col_ind_name(generator.getName(getColIndPrefix())), 
                                                                          row_ptr_name(generator.getName(getRowPtrPrefix())), val_name(generator.getName(getValPrefix())),
                                                                          nnz(internal.nnz()), rows(internal.getRowCount()), cols(internal.getColCount()), 
                                                                          col_ind(true, col_ind_name, nnz), row_ptr(true, row_ptr_name, internal.row_ptr_size()),
                                                                          val(true, val_name, nnz)
  {
  }

  // TaskGraph code shouldn't generate CRS intermediates or returns so comment out this constructor for now
  //TGCRSMatrix(bool param, NameGenerator& generator, const ExprNode<matrix, T_element>& m) :  parameter(param), col_ind_name(generator.getName(getColIndPrefix())), 
  //                                                                        row_ptr_name(generator.getName(getRowPtrPrefix())), val_name(generator.getName(getValPrefix())),
  //                                                                        nnz(0), rows(m.getRowCount()), cols(m.getColCount()), col_ind(param, col_ind_name, 0), 
  //                                                                        row_ptr(param, row_ptr_name, 0), val(param, val_name, 0)
  //{
  //  //FIXME: Handle this error better
  //  assert(0 && "CRS sparse matrices should not be generated as intermediates or return values by TaskGraph code");
  //}

  const TGScalarExpr<T_element> getExpression(NameGenerator& generator, const tg::TaskExpression& row, const tg::TaskExpression& col) const
  {
    using namespace tg;

    tVarNamed(int, valPtr, generator.getName("valPtr").c_str());
    tVarTemplateTypeNamed(T_element, result, generator.getName("elementVal").c_str());

    result = T_element();

    tFor(valPtr, (*row_ptr)[row], (*row_ptr)[row+1]-1)
    {
      tIf((*col_ind)[valPtr] == col)
        result = (*val)[valPtr];
    }

    return TGScalarExpr<T_element>(result);
  }

  void setExpression(NameGenerator& generator, const tg::TaskExpression& row, const tg::TaskExpression& col, const TGScalarExpr<T_element>& e)
  {
    //FIXME: Handle this problem better
    assert(0 && "Editing a CRS Matrix inside TaskGraph code is not implemented.");
  }

  void addExpression(NameGenerator& generator, const tg::TaskExpression& row, const tg::TaskExpression& col, const TGScalarExpr<T_element>& e)
  {

    //FIXME: Handle this problem better
    assert(0 && "Editing a CRS Matrix inside TaskGraph code is not implemented.");
  }
  
  virtual std::size_t getRows() const
  {
    return rows;
  }

  virtual std::size_t getCols() const
  {
    return cols;
  }

  virtual InternalMatrix<T_element>* createInternalRep() const
  {
    assert(parameter);
    return new CRSMatrix<T_element>(rows, cols);
  }

  virtual bool isParameter() const
  {
    return parameter;
  }

  virtual void iterateDense(NameGenerator& generator, MatrixIterationCallback& callback) const
  {
    //FIXME: Implement me
    assert(0 && "Not yet implemented dense iteration over CRS matrix.");
  }

  virtual void iterateSparse(NameGenerator& generator, MatrixIterationCallback& callback) const
  {
    using namespace tg;
    const bool useSingleForImplementation = false;

    if (useSingleForImplementation)
    {
      tVarNamed(unsigned, valPtr, generator.getName("valPtr").c_str());
      tVarNamed(unsigned, currentRow, generator.getName("currentRow").c_str());

      currentRow = 0u;

      tFor(valPtr, 0u, nnz-1)
      {
        tWhile((*row_ptr)[currentRow+1] == valPtr)
          ++currentRow;

        callback(generator, currentRow, (*col_ind)[valPtr], TGScalarExpr<T_element>((*val)[valPtr]));
      }
    }
    else
    {
      tVarNamed(unsigned, row, generator.getName("row").c_str());
      tVarNamed(unsigned, valPtr, generator.getName("valPtr").c_str());
      tVarNamed(unsigned, valPtrStart, generator.getName("valPtrStart").c_str());
      tVarNamed(unsigned, valPtrEnd, generator.getName("valPtrEnd").c_str());

      tFor(row, 0u, rows-1)
      {
        valPtrStart = (*row_ptr)[row];
        valPtrEnd = (*row_ptr)[row+1]-1;

        tFor(valPtr, valPtrStart, valPtrEnd)
        {
          callback(generator, row, (*col_ind)[valPtr], TGScalarExpr<T_element>((*val)[valPtr]));
        }
      }
    }
  }

  virtual void addParameterMappings(InternalMatrix<T_element>& internal, ParameterHolder& params) const
  {
    assert(parameter);
    Mapper mapper(*this, params);
    internal.accept(mapper);
  }

  virtual std::size_t hashValue() const
  {
    const char* nodeTypeString = typeid(*this).name();
    std::size_t seed = boost::hash_range(nodeTypeString, nodeTypeString+strlen(nodeTypeString));
    boost::hash_combine(seed, parameter);
    boost::hash_combine(seed, rows);
    boost::hash_combine(seed, cols);
    boost::hash_combine(seed, col_ind_name);
    boost::hash_combine(seed, row_ptr_name);
    boost::hash_combine(seed, val_name);
    return seed;
  }

  virtual bool matches(const TGMatrix<T_element>& m) const
  {
    if (typeid(m) == typeid(*this))
    {
      const TGCRSMatrix& right = static_cast<const TGCRSMatrix&>(m);
      return parameter == right.parameter &&
             col_ind_name == right.col_ind_name &&
             row_ptr_name == right.row_ptr_name &&
             val_name == right.val_name &&
             rows == right.rows &&
	     cols == right.cols;
    }
    else
    {
      return false;
    }
  }
   
  virtual void createTaskGraphVariable()
  {
    col_ind.instantiate();
    row_ptr.instantiate();
    val.instantiate();
  }

  class Mapper : public InternalMatrixVisitor<T_element>
  {
  private:
    const TGCRSMatrix<T_element>& internal;
    ParameterHolder& parameterHolder;

  public:
    Mapper(const TGCRSMatrix<T_element>& i, ParameterHolder& p) : internal(i), parameterHolder(p)
    {
    }

    virtual void visit(CRSMatrix<T_element>& m)
    {
      assert(m.isAllocated());
      parameterHolder.addParameter(internal.col_ind_name, m.get_col_ind());
      parameterHolder.addParameter(internal.row_ptr_name, m.get_row_ptr());
      parameterHolder.addParameter(internal.val_name, m.get_val());
    }
    
    virtual void visit(ConventionalMatrix<T_element>& m)
    {
      // FIXME: Handle this error better
      assert(0 && "Attempted to map parameters for ConventionalMatrix instead of a CRSMatrix");
    }
  };
};


template<typename T_element>
class TGMatrixGen : public InternalMatrixVisitor<T_element>
{
  NameGenerator& generator;
  TGMatrix<T_element>* result;
public:
  TGMatrixGen(NameGenerator& g) : generator(g)
  {
  }
  
  void visit(ConventionalMatrix<T_element>& m)
  {
    result = new TGConventionalMatrix<T_element>(generator, m);
  }

  void visit(CRSMatrix<T_element>& m)
  {
    result = new TGCRSMatrix<T_element>(generator, m);
  }

  TGMatrix<T_element>* getResult() const
  {
    assert(result != NULL);
    return result;
  }
};

template<typename T_element>
class TGVectorGen : public InternalVectorVisitor<T_element>
{
  NameGenerator& generator;
  TGVector<T_element>* result;
public:
  TGVectorGen(NameGenerator& g) : generator(g)
  {
  }

  void visit(ConventionalVector<T_element>& m)
  {
    result = new TGConventionalVector<T_element>(generator, m);
  }
 
  TGVector<T_element>* getResult() const
  {
    assert(result != NULL);
    return result;
  }
};

template<typename T_element>
class TGScalarGen : public InternalScalarVisitor<T_element>
{
  NameGenerator& generator;
  TGScalar<T_element>* result;
public:
  TGScalarGen(NameGenerator& g) : generator(g)
  {
  }

  void visit(ConventionalScalar<T_element>& m)
  {
    result = new TGConventionalScalar<T_element>(generator, m);
  }
 
  TGScalar<T_element>* getResult() const
  {
    assert(result != NULL);
    return result;
  }
};

}

}
#endif
