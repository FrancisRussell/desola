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

#ifndef DESOLA_LITERAL_HPP
#define DESOLA_LITERAL_HPP

#include <cassert>
#include <cstddef>
#include <boost/scoped_ptr.hpp>
#include <boost/static_assert.hpp>
#include <desola/Desola_fwd.hpp>

namespace desola
{

namespace detail
{

template<typename exprType, typename T_element>
class Literal : public ExprNode<exprType, T_element>
{
  // Never instantiate
  BOOST_STATIC_ASSERT(sizeof(T_element)==0);
};

template<typename T_element>
class Literal<scalar, T_element> : public ExprNode<scalar, T_element>
{
protected:
  boost::scoped_ptr< InternalScalar<T_element> > value;

  virtual void update(ExprNode<scalar, T_element>& previous, ExprNode<scalar, T_element>& next) {}
  virtual void update(ExprNode<vector, T_element>& previous, ExprNode<vector, T_element>& next) {}
  virtual void update(ExprNode<matrix, T_element>& previous, ExprNode<matrix, T_element>& next) {}
	
public:
  Literal(InternalScalar<T_element>* const val) : value(val)
  {
    assert(val != NULL);
  }

  InternalScalar<T_element>& getValue()
  {
    return *value;
  }

  void accept(ExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }

  void accept(LiteralVisitor<T_element>& v)
  {
    v.visit(*this);
  }

  virtual void internal_evaluate()
  {
    // We don't do anything for literals
  }

  virtual T_element getElementValue()
  {
    return value->getElementValue();
  }

  virtual Maybe<double> getFlops() const
  {
    return 0.0;
  }
};

template<typename T_element>
class Literal<vector, T_element> : public ExprNode<vector, T_element>
{
protected:
  boost::scoped_ptr< InternalVector<T_element> > value;

  virtual void update(ExprNode<scalar, T_element>& previous, ExprNode<scalar, T_element>& next) {}
  virtual void update(ExprNode<vector, T_element>& previous, ExprNode<vector, T_element>& next) {}
  virtual void update(ExprNode<matrix, T_element>& previous, ExprNode<matrix, T_element>& next) {}

  inline static const boost::array<std::size_t, 1> getDimensions(const InternalVector<T_element>& internal)
  {
    const boost::array<std::size_t, 1> dims = { {internal.getRowCount()} };
    return dims;
  }
  
public:
  Literal(InternalVector<T_element>* const val) : ExprNode<vector, T_element>(getDimensions(*val)), value(val)
  {
    assert(val != NULL);
  }

  InternalVector<T_element>& getValue()
  {
    return *value;
  }

  void accept(ExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }

  void accept(LiteralVisitor<T_element>& v)
  {
    v.visit(*this);
  }

  virtual void internal_evaluate()
  {
    // We don't do anything for literals
  }

  virtual T_element getElementValue(const ElementIndex<vector>& index)
  {
    return value->getElementValue(index);
  }

  virtual Maybe<double> getFlops() const
  {
    return 0.0;
  }
};

template<typename T_element>
class Literal<matrix, T_element> : public ExprNode<matrix, T_element>
{
protected:
  boost::scoped_ptr< InternalMatrix<T_element> > value;

  virtual void update(ExprNode<scalar, T_element>& previous, ExprNode<scalar, T_element>& next) {}
  virtual void update(ExprNode<vector, T_element>& previous, ExprNode<vector, T_element>& next) {}
  virtual void update(ExprNode<matrix, T_element>& previous, ExprNode<matrix, T_element>& next) {}

  inline static const boost::array<std::size_t, 2> getDimensions(const InternalMatrix<T_element>& internal)
  {
    const boost::array<std::size_t, 2> dims = { {internal.getRowCount(), internal.getColCount()} };
    return dims;
  }
  
public:
  Literal(InternalMatrix<T_element>* const val) : ExprNode<matrix, T_element>(getDimensions(*val)), value(val)
  {
    assert(val != NULL);
  }

  InternalMatrix<T_element>& getValue()
  {
    return *value;
  }

  void accept(ExpressionNodeVisitor<T_element>& v)
  {
    v.visit(*this);
  }

  void accept(LiteralVisitor<T_element>& v)
  {
    v.visit(*this);
  }

  virtual void internal_evaluate()
  {
    // We don't do anything for literals
  }

  virtual T_element getElementValue(const ElementIndex<matrix>& index)
  {
    return value->getElementValue(index);
  }

  virtual Maybe<double> getFlops() const
  {
    return 0.0;
  }

  virtual Maybe<std::size_t> nnz() const
  {
    return value->nnz();
  }
};

}

}
#endif
