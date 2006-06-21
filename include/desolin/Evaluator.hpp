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

#ifndef DESOLIN_EVALUATOR_HPP
#define DESOLIN_EVALUATOR_HPP

#include <set>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <desolin/Desolin_fwd.hpp>

namespace desolin_internal
{

template <typename T_element>
class Evaluator
{
public:
  virtual std::set<ExpressionNode<T_element>*> claimNodes(const std::vector<ExpressionNode<T_element>*>& sortedUnclaimed) = 0;
  virtual void generateEvaluatedNodes() = 0;
  virtual void evaluate() = 0;
  virtual ~Evaluator() {}
};

template<typename T_element>
class EvaluatorFactory
{
public:
  virtual boost::shared_ptr< Evaluator<T_element> > createEvaluator(EvaluationStrategy<T_element>& strategy) = 0;
  virtual ~EvaluatorFactory() {}
};

}
#endif
