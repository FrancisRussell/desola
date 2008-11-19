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

#ifndef DESOLIN_TG_EXPRESSION_NODE_HPP
#define DESOLIN_TG_EXPRESSION_NODE_HPP

#include <set>
#include <map>
#include <string>
#include <cassert>
#include <cstddef>
#include <memory>
#include <boost/functional/hash.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/type_traits/add_const.hpp>  
#include <boost/variant.hpp>
#include <boost/foreach.hpp>
#include <desolin/tg/Desolin_tg_fwd.hpp>

namespace desolin
{

namespace detail
{

template<typename exprType>
class TGElementIndex
{
};

template<>
class TGElementIndex<tg_vector>
{
private:
  const std::size_t row;

public:
  TGElementIndex(const std::size_t r) : row(r)
  {
  }

  TGElementIndex(const ElementIndex<vector>& index) : row(index.getRow())
  {
  }

  const inline std::size_t getRow() const
  {
    return row;
  }

  inline bool operator==(const TGElementIndex& i) const
  {
    return row == i.row;
  }

  inline bool operator<(const TGElementIndex& i) const
  {
    return row < i.row;
  }
};

std::size_t hash_value(const TGElementIndex<tg_vector>& index)
{
  return boost::hash<int>()(index.getRow());
}

template<>
class TGElementIndex<tg_matrix>
{
private:
  const std::size_t row;
  const std::size_t col;
  
public:
  TGElementIndex(const std::size_t r, const std::size_t c) : row(r), col(c)
  {
  }

  TGElementIndex(const ElementIndex<matrix>& index) : row(index.getRow()), col(index.getCol())
  {
  }

  const inline std::size_t getRow() const
  {
    return row;
  }

  const inline std::size_t getCol() const
  {
    return col;
  }

  inline bool operator==(const TGElementIndex& i) const
  {
    return row == i.row && col == i.col;
  }

  inline bool operator<(const TGElementIndex& i) const
  {
    if(row != i.row)
    {
      return row < i.row;
    }
    else
    {
      return col < i.col;
    }
  }
};

std::size_t hash_value(const TGElementIndex<tg_matrix>& index)
{
  std::size_t seed = boost::hash<std::size_t>()(index.getRow());
  boost::hash_combine(seed, index.getCol());
  return seed;
}

template<typename tgExprType, typename T_element>
class TGOutputReference
{
private:
  TGExpressionNode<T_element>* node;
  std::size_t index;

public:
  TGOutputReference(TGExpressionNode<T_element>* const n, const std::size_t i) : node(n), index(i)
  {
  }

  TGOutputReference(const TGOutputReference& ref) : node(ref.node), index(ref.index)
  {
  }

  TGOutputReference& operator=(const TGOutputReference& ref)
  {
    node = ref.node;
    index = ref.index;
    return *this;
  }

  inline bool operator==(const TGOutputReference& ref) const
  {
    return node == ref.node && index == ref.index;
  }

  bool operator<(const TGOutputReference& ref) const
  {
    if (node < ref.node) return true;
    if (index < ref.index) return true;
    return false;
  }

  inline bool isParameter() const
  {
    return node->isParameter(index);
  }

  inline std::size_t getIndex() const
  {
    return index;
  }

  inline TGExpressionNode<T_element>* getExpressionNode() const
  {
    return node;
  }

  const typename TGInternalType<tgExprType, T_element>::type& getInternal() const
  {
    return *boost::get<typename TGInternalType<tgExprType, T_element>::type*>(node->getInternal(index));
  }

  typename TGInternalType<tgExprType, T_element>::type& getInternal()
  {
    return *boost::get<typename TGInternalType<tgExprType, T_element>::type*>(node->getInternal(index));
  }
};


template<typename T_candidate, typename T_replacement, typename T_element>
struct ReplaceOutputReference
{
  inline bool operator()(TGOutputReference<T_candidate, T_element>& ref, 
    const TGOutputReference<T_replacement, T_element>& from, const TGOutputReference<T_replacement, T_element>& to) const
  {
    return false;
  }
};

template<typename T_replacement, typename T_element>
struct ReplaceOutputReference<T_replacement, T_replacement, T_element>
{
  inline bool operator()(TGOutputReference<T_replacement, T_element>& ref, 
    const TGOutputReference<T_replacement, T_element>& from, const TGOutputReference<T_replacement, T_element>& to) const
  {
    const bool replace = (from == ref);
    if (replace) ref = to;
    return replace;
  }
};

template<typename T_element>
class TGExpressionNode
{
private:
  // Forbid copying and assignment
  TGExpressionNode(const TGExpressionNode&);
  TGExpressionNode& operator=(const TGExpressionNode&);

  typedef boost::mpl::vector<TGScalar<T_element>*, TGVector<T_element>*, TGMatrix<T_element>*> internal_types;
  typedef typename boost::mpl::transform< internal_types, boost::add_const<boost::mpl::_1> >::type internal_types_const;
  std::vector<TGExpressionNode<T_element>*> dependencies;
  std::vector<TGExpressionNode<T_element>*> reverseDependencies;

  struct InternalTypeComparator : public boost::static_visitor<bool>
  {
    template<typename T>
    inline bool operator()(const T* const  a, const T* const b) const
    {
      return a->matches(*b);
    }

    template<typename T1, typename T2>
    inline bool operator()(const T1* const  a, const T2* const b) const
    {
      return false;
    }
  };

protected:
  template<typename exprType>
  static inline bool isEqual(const TGOutputReference<exprType, T_element>& ref1, const TGOutputReference<exprType, T_element>& ref2, 
    const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings)
  {
    if (ref1.getIndex() != ref2.getIndex())
      return false;

    typedef typename std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>::const_iterator NodeMappingIter;
    const NodeMappingIter nodeMappingIter(mappings.find(ref1.getExpressionNode()));
    assert(nodeMappingIter != mappings.end());

    return nodeMappingIter->second == ref2.getExpressionNode();
  }

  void registerDependency(TGExpressionNode<T_element>* const dependency)
  {
    assert(dependency != NULL);
    dependencies.push_back(dependency);
    dependency->registerReverseDependency(this);
  }

  void registerReverseDependency(TGExpressionNode<T_element>* const revDependency)
  {
    assert(revDependency != NULL);
    reverseDependencies.push_back(revDependency);
  }

  void unregisterReverseDependency(TGExpressionNode<T_element>* const revDependency)
  {
    assert(revDependency != NULL);
    const typename std::vector<TGExpressionNode<T_element>*>::iterator location = 
      std::find(reverseDependencies.begin(), reverseDependencies.end(), revDependency);
    assert(location != reverseDependencies.end());
    reverseDependencies.erase(location);
  }

  template<typename exprType>
  void alterDependencyGeneral(const TGOutputReference<exprType, T_element>& previous, TGOutputReference<exprType, T_element>& next)
  {
    // Unlike in the general expression node class, this needs to replace all dependencies on a given node,
    // not just the first one. This is because the node replacement is not being handled by the node being
    // replaced, and this method won't be called multiple times if we have more than one dependency on the
    // node being replaced.
    
    for(typename std::vector<TGExpressionNode<T_element>*>::iterator depIter = dependencies.begin(); depIter != dependencies.end(); ++depIter)
    {
      if (previous.getExpressionNode() == *depIter)
      {
        *depIter = next.getExpressionNode();
        next.getExpressionNode()->registerReverseDependency(this);
        previous.getExpressionNode()->unregisterReverseDependency(this);
      }
    }

    alterDependencyImpl(previous, next);
  }

  virtual void alterDependencyImpl(const TGOutputReference<tg_scalar, T_element>& previous, TGOutputReference<tg_scalar, T_element>& next) = 0;
  virtual void alterDependencyImpl(const TGOutputReference<tg_vector, T_element>& previous, TGOutputReference<tg_vector, T_element>& next) = 0;
  virtual void alterDependencyImpl(const TGOutputReference<tg_matrix, T_element>& previous, TGOutputReference<tg_matrix, T_element>& next) = 0;

public:
  typedef typename boost::make_variant_over<internal_types>::type internal_variant_type;
  typedef typename boost::make_variant_over<internal_types_const>::type const_internal_variant_type;

  TGExpressionNode()
  {
  }

  bool isEqual(const TGExpressionNode& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    const std::size_t numOutputs = getNumOutputs();

    if (numOutputs != node.getNumOutputs())
      return false;

    for(std::size_t index=0; index < numOutputs; ++index)
    {
      const bool equal = boost::apply_visitor(InternalTypeComparator(), getInternal(index), node.getInternal(index));
      if (!equal)
        return false;
    }

    return true;
  }
  
  virtual void accept(TGExpressionNodeVisitor<T_element>& visitor) = 0;
  virtual void createTaskGraphVariable() = 0;

  std::vector<TGExpressionNode<T_element>*> getDependencies() const
  {
    return dependencies;
  }

  std::vector<TGExpressionNode<T_element>*> getReverseDependencies() const
  {
    return reverseDependencies;
  }

  std::size_t numDependencies() const
  {
    return dependencies.size();
  }

  std::size_t numReverseDependencies() const
  {
    return reverseDependencies.size();
  }

  virtual std::size_t getNumOutputs() const = 0;
  virtual bool isParameter(const std::size_t index) const = 0;
  virtual internal_variant_type getInternal(const std::size_t index) = 0;
  virtual const internal_variant_type getInternal(const std::size_t index) const = 0;

  void alterDependency(const TGOutputReference<tg_scalar, T_element>& previous, TGOutputReference<tg_scalar, T_element>& next)
  {
    alterDependencyGeneral(previous, next);
  }

  void alterDependency(const TGOutputReference<tg_vector, T_element>& previous, TGOutputReference<tg_vector, T_element>& next)
  {
    alterDependencyGeneral(previous, next);
  }

  void alterDependency(const TGOutputReference<tg_matrix, T_element>& previous, TGOutputReference<tg_matrix, T_element>& next)
  {
    alterDependencyGeneral(previous, next);
  }

  virtual ~TGExpressionNode()
  {
    // We shouldn't have anything depending on this node
    assert(reverseDependencies.empty());

    BOOST_FOREACH(TGExpressionNode<T_element>* dep, dependencies)
    {
      dep->unregisterReverseDependency(this);
    }
  }
};

template<typename exprType, typename T_element>
class TGExprNode : public TGExpressionNode<T_element>
{
private:
  typedef typename TGInternalType<exprType, T_element>::type T_internal;
  std::auto_ptr<T_internal> internal;

public:
  typedef typename TGExpressionNode<T_element>::internal_variant_type internal_variant_type;
  typedef typename TGExpressionNode<T_element>::const_internal_variant_type const_internal_variant_type;

  inline bool isEqual(const TGExprNode& node, const std::map<const TGExpressionNode<T_element>*, const TGExpressionNode<T_element>*>& mappings) const
  {
    return TGExpressionNode<T_element>::isEqual(node, mappings);
  }
  
  TGExprNode(T_internal* const i) : internal(i)
  {
  }

  void createTaskGraphVariable()
  {
    internal->createTaskGraphVariable();
  }

  inline T_internal& getInternal()
  {
    return *internal;
  }

  T_internal* releaseInternal()
  {
    return internal.release();
  }

  inline const T_internal& getInternal() const
  {
    return *internal;
  }

  virtual std::size_t getNumOutputs() const
  {
    return 1;
  }

  bool isParameter(const std::size_t index) const
  {
    if (index == 0)
    {
      return internal->isParameter();
    }
    else
    {
      assert(false && "isParameter call for internal representation with index>0 for node with only one output.");
    }
  }

  virtual internal_variant_type getInternal(const std::size_t index)
  {
    if (index == 0)
    {
      return internal_variant_type(internal.get());
    }
    else
    {
      assert(false && "getInternal call for internal representation with index>0 for node with only one output.");
    }
  }

  virtual const internal_variant_type getInternal(const std::size_t index) const
  {
    if (index == 0)
    {
      return internal_variant_type(internal.get());
    }
    else
    {
      assert(false && "getInternal call for internal representation with index>0 for node with only one output.");
    }
  }

};

}

}
#endif
