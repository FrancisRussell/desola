#ifndef DESOLIN_TG_OBJECTS_HPP
#define DESOLIN_TG_OBJECTS_HPP

#include <boost/functional/hash.hpp>
#include <typeinfo>
#include <cassert>
#include <string>
#include <map>
#include <boost/format.hpp>
#include <TaskGraph>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin_internal
{
	
template<typename T_elementType>
class TGScalar
{
public:
  virtual const TGScalarExpr<T_elementType> getExpression() const = 0;
  virtual void setExpression(const TGScalarExpr<T_elementType>& e) = 0;
  virtual void addExpression(const TGScalarExpr<T_elementType>& e) = 0;
  virtual ~TGScalar() {}
  virtual InternalScalar<T_elementType>* createInternalRep() const = 0;
  virtual bool isParameter() const = 0;
  virtual void addParameterMappings(InternalScalar<T_elementType>& internal, ParameterHolder& params) const = 0;
  virtual std::size_t hashValue() const = 0;
};

template<typename T_elementType>
class TGVector
{
public:
  virtual const TGScalarExpr<T_elementType> getExpression(const tg::TaskExpression& row) const = 0;
  virtual void setExpression(const tg::TaskExpression& row, const TGScalarExpr<T_elementType>& e) = 0;
  virtual void addExpression(const tg::TaskExpression& row, const TGScalarExpr<T_elementType>& e) = 0;
  virtual int getRows() const = 0;
  virtual ~TGVector() {}
  virtual InternalVector<T_elementType>* createInternalRep() const = 0;
  virtual bool isParameter() const = 0;
  virtual void addParameterMappings(InternalVector<T_elementType>& internal, ParameterHolder& params) const = 0;
  virtual std::size_t hashValue() const = 0;
};

template<typename T_elementType>
class TGMatrix
{
public:
  virtual const TGScalarExpr<T_elementType> getExpression(const tg::TaskExpression& row, const tg::TaskExpression& col) const = 0;
  virtual void setExpression(const tg::TaskExpression& row, const tg::TaskExpression& col, const TGScalarExpr<T_elementType>& e) = 0;  
  virtual void addExpression(const tg::TaskExpression& row, const tg::TaskExpression& col, const TGScalarExpr<T_elementType>& e) = 0;
  virtual int getRows() const = 0;
  virtual int getCols() const = 0;
  virtual ~TGMatrix() {}
  virtual InternalMatrix<T_elementType>* createInternalRep() const = 0;
  virtual bool isParameter() const = 0;
  virtual void addParameterMappings(InternalMatrix<T_elementType>& internal, ParameterHolder& params) const = 0;
  virtual std::size_t hashValue() const = 0;
};

template<typename T_elementType>
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

template<typename T_elementType>
class TGConventionalScalar : public TGScalar<T_elementType>
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
      tParameter(tVarNamedTemplateType(T_elementType, val, name.c_str()));
      return val;
    }
    else
    {
      tVarNamedTemplateType(T_elementType, val, name.c_str());
      return val;
    }
  }

  static inline const std::string getPrefix()
  {
    return std::string("convScalar");
  }
  
public:
  TGConventionalScalar(NameGenerator& generator, const ConventionalScalar<T_elementType>& internal) : parameter(true), name(generator.getName(getPrefix())), value(createValue(true, name))
  {
  }

  TGConventionalScalar(bool param, NameGenerator& generator, const ExprNode<scalar, T_elementType>& s) : parameter(param), name(generator.getName(getPrefix())), value(createValue(param, name))
  {
  }

  const TGScalarExpr<T_elementType> getExpression() const
  {
    return TGScalarExpr<T_elementType>(value);
  }

  void setExpression(const TGScalarExpr<T_elementType>& e)
  {
    value = e.getExpression();
  }

  void addExpression(const TGScalarExpr<T_elementType>& e)
  {
    value += e.getExpression();
  }
  

  virtual InternalScalar<T_elementType>* createInternalRep() const
  {
    assert(parameter);
    return new ConventionalScalar<T_elementType>();
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

  virtual void addParameterMappings(InternalScalar<T_elementType>& internal, ParameterHolder& params) const
  {
    assert(parameter);
    Mapper mapper(*this, params);
    internal.accept(mapper);
  }

  class Mapper : public InternalScalarVisitor<T_elementType>
  {
  private:
    const TGConventionalScalar<T_elementType>& internal;
    ParameterHolder& parameterHolder;
  
  public:
    Mapper(const TGConventionalScalar<T_elementType>& i, ParameterHolder& p) : internal(i), parameterHolder(p)
    {
    }
    
    virtual void visit(ConventionalScalar<T_elementType>& s)
    {
      assert(s.isAllocated());
      parameterHolder.addParameter(internal.name, s.getValue()); 
    }
  };
};

template<typename T_elementType>
class TGConventionalVector : public TGVector<T_elementType>
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
      tParameter(tArrayFromListNamed(T_elementType, val, 1, dims, name.c_str()));
      return val;
    }
    else
    {
      tArrayFromListNamed(T_elementType, val, 1, dims, name.c_str());
      return val;
    }
  }

  static inline const std::string getPrefix()
  {
    return std::string("convVector");
  }
  
public:
  TGConventionalVector(NameGenerator& generator, const ConventionalVector<T_elementType>& internal) :  parameter(true), name(generator.getName(getPrefix())), rows(internal.getRowCount()), value(createValue(true, name, rows))
  {
  }

  TGConventionalVector(bool param, NameGenerator& generator, const ExprNode<vector, T_elementType>& v) :  parameter(param), name(generator.getName(getPrefix())), rows(v.getRowCount()), value(createValue(param, name, rows))
  {
  }
  
  const TGScalarExpr<T_elementType> getExpression(const tg::TaskExpression& row) const
  {
    return TGScalarExpr<T_elementType>(value[row]);
  }

  void setExpression(const tg::TaskExpression& row, const TGScalarExpr<T_elementType>& e)
  {
    value[row] = e.getExpression();
  }

  void addExpression(const tg::TaskExpression& row, const TGScalarExpr<T_elementType>& e)
  {
    value[row] += e.getExpression();
  }

  virtual int getRows() const
  {
    return rows;
  }

  virtual InternalVector<T_elementType>* createInternalRep() const
  {
    assert(parameter);
    return new ConventionalVector<T_elementType>(rows);
  }

  virtual bool isParameter() const
  {
    return parameter;
  }

  virtual void addParameterMappings(InternalVector<T_elementType>& internal, ParameterHolder& params) const
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
    
  class Mapper : public InternalVectorVisitor<T_elementType>
  {
  private:
    const TGConventionalVector<T_elementType>& internal;
    ParameterHolder& parameterHolder;

  public:
    Mapper(const TGConventionalVector<T_elementType>& i, ParameterHolder& p) : internal(i), parameterHolder(p)
    {
    }

    virtual void visit(ConventionalVector<T_elementType>& v)
    {
      assert(v.isAllocated());
      parameterHolder.addParameter(internal.name, v.getValue());
    }
  };
    
};

template<typename T_elementType>
class TGConventionalMatrix : public TGMatrix<T_elementType>
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
      tParameter(tArrayFromListNamed(T_elementType, val, 2, dims, name.c_str()));
      return val;
    }
    else
    {
      tArrayFromListNamed(T_elementType, val, 2, dims, name.c_str());      
      return val;
    }
  }

  static inline const std::string getPrefix()
  {
    return std::string("convMatrix");
  }
      
public:
  TGConventionalMatrix(NameGenerator& generator, ConventionalMatrix<T_elementType>& internal) : parameter(true), name(generator.getName(getPrefix())),  rows(internal.getRowCount()), cols(internal.getColCount()), value(createValue(true, name, rows, cols))
  {
  }

  TGConventionalMatrix(bool param, NameGenerator& generator, const ExprNode<matrix, T_elementType>& m) :  parameter(param), name(generator.getName(getPrefix())), rows(m.getRowCount()), cols(m.getColCount()), value(createValue(param, name, rows, cols))
  {
  }

  const TGScalarExpr<T_elementType> getExpression(const tg::TaskExpression& row, const tg::TaskExpression& col) const
  {
    return TGScalarExpr<T_elementType>(value[row][col]);
  }

  void setExpression(const tg::TaskExpression& row, const tg::TaskExpression& col, const TGScalarExpr<T_elementType>& e)
  {
    value[row][col] = e.getExpression();
  }

  void addExpression(const tg::TaskExpression& row, const tg::TaskExpression& col, const TGScalarExpr<T_elementType>& e)
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

  virtual InternalMatrix<T_elementType>* createInternalRep() const
  {
    assert(parameter);
    return new ConventionalMatrix<T_elementType>(rows, cols);
  }

  virtual bool isParameter() const
  {
    return parameter;
  }

  virtual void addParameterMappings(InternalMatrix<T_elementType>& internal, ParameterHolder& params) const
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
   
  class Mapper : public InternalMatrixVisitor<T_elementType>
  {
  private:
    const TGConventionalMatrix<T_elementType>& internal;
    ParameterHolder& parameterHolder;

  public:
    Mapper(const TGConventionalMatrix<T_elementType>& i, ParameterHolder& p) : internal(i), parameterHolder(p)
    {
    }

    virtual void visit(ConventionalMatrix<T_elementType>& m)
    {
      assert(m.isAllocated());
      parameterHolder.addParameter(internal.name, m.getValue());
    }
  };    
};

template<typename T_elementType>
class TGMatrixGen : public InternalMatrixVisitor<T_elementType>
{
  NameGenerator& generator;
  TGMatrix<T_elementType>* result;
public:
  TGMatrixGen(NameGenerator& g) : generator(g)
  {
  }
  
  void visit(ConventionalMatrix<T_elementType>& m)
  {
    result = new TGConventionalMatrix<T_elementType>(generator, m);
  }

  TGMatrix<T_elementType>* getResult() const
  {
    assert(result != NULL);
    return result;
  }
};

template<typename T_elementType>
class TGVectorGen : public InternalVectorVisitor<T_elementType>
{
  NameGenerator& generator;
  TGVector<T_elementType>* result;
public:
  TGVectorGen(NameGenerator& g) : generator(g)
  {
  }

  void visit(ConventionalVector<T_elementType>& m)
  {
    result = new TGConventionalVector<T_elementType>(generator, m);
  }
 
  TGVector<T_elementType>* getResult() const
  {
    assert(result != NULL);
    return result;
  }
};

template<typename T_elementType>
class TGScalarGen : public InternalScalarVisitor<T_elementType>
{
  NameGenerator& generator;
  TGScalar<T_elementType>* result;
public:
  TGScalarGen(NameGenerator& g) : generator(g)
  {
  }

  void visit(ConventionalScalar<T_elementType>& m)
  {
    result = new TGConventionalScalar<T_elementType>(generator, m);
  }
 
  TGScalar<T_elementType>* getResult() const
  {
    assert(result != NULL);
    return result;
  }
};

}
#endif
