#ifndef DESOLIN_TG_HIGH_LEVEL_FUSER_HPP
#define DESOLIN_TG_HIGH_LEVEL_FUSER_HPP

#include<vector>

namespace desolin
{

namespace detail
{

template<typename T_element>
class MatrixVectorMultiplyFinder : public TGExpressionNodeVisitor<T_element>
{
private:
  std::vector<TGMatrixVectorMult<T_element>*> multiplies;

public:
  std::vector<TGMatrixVectorMult<T_element>*> getMultiplies() const
  {
    return multiplies;
  }

  void visit(TGElementGet<tg_vector, T_element>& e) {}
  void visit(TGElementGet<tg_matrix, T_element>& e) {}

  void visit(TGElementSet<tg_vector, T_element>& e) {}
  void visit(TGElementSet<tg_matrix, T_element>& e) {}

  void visit(TGLiteral<tg_scalar, T_element>& e) {}
  void visit(TGLiteral<tg_vector, T_element>& e) {}
  void visit(TGLiteral<tg_matrix, T_element>& e) {}

  void visit(TGMatrixMult<T_element>& e) {}

  void visit(TGMatrixVectorMult<T_element>& e)
  {
    multiplies.push_back(&e);
  }

  void visit(TGVectorDot<T_element>& e) {}
  void visit(TGVectorCross<T_element>& e) {}
  void visit(TGVectorTwoNorm<T_element>& e) {}
  void visit(TGMatrixTranspose<T_element>& e) {}

  void visit(TGPairwise<tg_scalar, T_element>& e) {}
  void visit(TGPairwise<tg_vector, T_element>& e) {}
  void visit(TGPairwise<tg_matrix, T_element>& e) {}

  void visit(TGScalarPiecewise<tg_scalar, T_element>& e) {}
  void visit(TGScalarPiecewise<tg_vector, T_element>& e) {}
  void visit(TGScalarPiecewise<tg_matrix, T_element>& e) {}

  void visit(TGNegate<tg_scalar, T_element>& e) {}
  void visit(TGNegate<tg_vector, T_element>& e) {}
  void visit(TGNegate<tg_matrix, T_element>& e) {}

  void visit(TGAbsolute<T_element>& e) {}
  void visit(TGSquareRoot<T_element>& e) {}
};

template<typename T_element>
class HighLevelFuser
{
private:
  TGExpressionGraph<T_element>& graph;

public:
  HighLevelFuser(TGExpressionGraph<T_element>& g) : graph(g)
  {
  }

  void execute()
  {
    MatrixVectorMultiplyFinder<T_element> finder;
    graph.accept(finder);

    const std::vector<TGMatrixVectorMult<T_element>*> multiplies(finder.getMultiplies());
    std::cout << "Number of mat-vec muls: " << multiplies.size() << std::endl;
  }
};

}

}

#endif
