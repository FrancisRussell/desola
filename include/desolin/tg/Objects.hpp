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

#include <boost/functional/hash.hpp>
#include <typeinfo>
#include <cassert>
#include <cstddef>
#include <string>
#include <map>
#include <boost/format.hpp>
#include <TaskGraph>
#include <desolin/tg/Desolin_tg_fwd.hpp>

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
  virtual ~TGScalar() {}
};

template<typename T_element>
class TGVector
{
public:
  virtual const TGScalarExpr<T_element> getExpression(const tg::TaskExpression& row) const = 0;
  virtual void setExpression(const tg::TaskExpression& row, const TGScalarExpr<T_element>& e) = 0;
  virtual void addExpression(const tg::TaskExpression& row, const TGScalarExpr<T_element>& e) = 0;
  virtual int getRows() const = 0;
  virtual InternalVector<T_element>* createInternalRep() const = 0;
  virtual bool isParameter() const = 0;
  virtual void addParameterMappings(InternalVector<T_element>& internal, ParameterHolder& params) const = 0;
  virtual std::size_t hashValue() const = 0;
  virtual bool matches(const TGVector& v) const = 0;
  virtual ~TGVector() {}
};

template<typename T_element>
class TGMatrix
{
public:
  virtual const TGScalarExpr<T_element> getExpression(const tg::TaskExpression& row, const tg::TaskExpression& col) const = 0;
  virtual void setExpression(const tg::TaskExpression& row, const tg::TaskExpression& col, const TGScalarExpr<T_element>& e) = 0;  
  virtual void addExpression(const tg::TaskExpression& row, const tg::TaskExpression& col, const TGScalarExpr<T_element>& e) = 0;
  virtual int getRows() const = 0;
  virtual int getCols() const = 0;
  virtual InternalMatrix<T_element>* createInternalRep() const = 0;
  virtual bool isParameter() const = 0;
  virtual void addParameterMappings(InternalMatrix<T_element>& internal, ParameterHolder& params) const = 0;
  virtual std::size_t hashValue() const = 0;
  virtual bool matches(const TGMatrix& m) const = 0;
  virtual ~TGMatrix() {}
};

template<typename T_element>
class TGScalarExpr
{
  const tg::TaskExpression expr;

public:
  TGScalarExpr() : expr(0)
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
  tg::TaskScalarVariable value;

  static tg::TaskScalarVariable createValue(bool param, const std::string& name)
  {
    if (param)
    {
      tParameter(tVarNamedTemplateType(T_element, val, name.c_str()));
      return val;
    }
    else
    {
      tVarNamedTemplateType(T_element, val, name.c_str());
      return val;
    }
  }

  static inline const std::string getPrefix()
  {
    return std::string("convScalar");
  }
  
public:
  TGConventionalScalar(NameGenerator& generator, const ConventionalScalar<T_element>& internal) : parameter(true), name(generator.getName(getPrefix())), value(createValue(true, name))
  {
  }

  TGConventionalScalar(bool param, NameGenerator& generator, const ExprNode<scalar, T_element>& s) : parameter(param), name(generator.getName(getPrefix())), value(createValue(param, name))
  {
  }

  const TGScalarExpr<T_element> getExpression() const
  {
    return TGScalarExpr<T_element>(value);
  }

  void setExpression(const TGScalarExpr<T_element>& e)
  {
    value = e.getExpression();
  }

  void addExpression(const TGScalarExpr<T_element>& e)
  {
    value += e.getExpression();
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
    std::size_t seed = boost::hash<std::string>()(typeid(*this).name());
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
  const int rows;
  tg::TaskArray<1> value;

  static tg::TaskArray<1> createValue(bool param, const std::string& name, int rows)
  {
    unsigned dims[1] = {rows};
    if (param)
    {
      tParameter(tArrayFromListNamed(T_element, val, 1, dims, name.c_str()));
      return val;
    }
    else
    {
      tArrayFromListNamed(T_element, val, 1, dims, name.c_str());
      return val;
    }
  }

  static inline const std::string getPrefix()
  {
    return std::string("convVector");
  }
  
public:
  TGConventionalVector(NameGenerator& generator, const ConventionalVector<T_element>& internal) :  parameter(true), name(generator.getName(getPrefix())), rows(internal.getRowCount()), value(createValue(true, name, rows))
  {
  }

  TGConventionalVector(bool param, NameGenerator& generator, const ExprNode<vector, T_element>& v) :  parameter(param), name(generator.getName(getPrefix())), rows(v.getRowCount()), value(createValue(param, name, rows))
  {
  }
  
  const TGScalarExpr<T_element> getExpression(const tg::TaskExpression& row) const
  {
    return TGScalarExpr<T_element>(value[row]);
  }

  void setExpression(const tg::TaskExpression& row, const TGScalarExpr<T_element>& e)
  {
    value[row] = e.getExpression();
  }

  void addExpression(const tg::TaskExpression& row, const TGScalarExpr<T_element>& e)
  {
    value[row] += e.getExpression();
  }

  virtual int getRows() const
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
    std::size_t seed = boost::hash<std::string>()(typeid(*this).name());
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
  const int rows;
  const int cols;
  tg::TaskArray<2>  value;

  static tg::TaskArray<2> createValue(bool param, const std::string& name, int rows, int cols)
  {
    unsigned dims[2] = {rows, cols};
    if (param)
    {
      tParameter(tArrayFromListNamed(T_element, val, 2, dims, name.c_str()));
      return val;
    }
    else
    {
      tArrayFromListNamed(T_element, val, 2, dims, name.c_str());      
      return val;
    }
  }

  static inline const std::string getPrefix()
  {
    return std::string("convMatrix");
  }
      
public:
  TGConventionalMatrix(NameGenerator& generator, ConventionalMatrix<T_element>& internal) : parameter(true), name(generator.getName(getPrefix())),  rows(internal.getRowCount()), cols(internal.getColCount()), value(createValue(true, name, rows, cols))
  {
  }

  TGConventionalMatrix(bool param, NameGenerator& generator, const ExprNode<matrix, T_element>& m) :  parameter(param), name(generator.getName(getPrefix())), rows(m.getRowCount()), cols(m.getColCount()), value(createValue(param, name, rows, cols))
  {
  }

  const TGScalarExpr<T_element> getExpression(const tg::TaskExpression& row, const tg::TaskExpression& col) const
  {
    return TGScalarExpr<T_element>(value[row][col]);
  }

  void setExpression(const tg::TaskExpression& row, const tg::TaskExpression& col, const TGScalarExpr<T_element>& e)
  {
    value[row][col] = e.getExpression();
  }

  void addExpression(const tg::TaskExpression& row, const tg::TaskExpression& col, const TGScalarExpr<T_element>& e)
  {
    value[row][col] += e.getExpression();
  }
  
  virtual int getRows() const
  {
    return rows;
  }

  virtual int getCols() const
  {
    return cols;
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
    std::size_t seed = boost::hash<std::string>()(typeid(*this).name());
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
