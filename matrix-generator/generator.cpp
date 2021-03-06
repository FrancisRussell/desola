#include "matrix.hpp"
#include "generator_options.hpp"
#include <ctime>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <iostream>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>

template<typename MatrixType>
void populate(MatrixType& matrix, const std::size_t bandwidth, const double density, const bool symmetric)
{
  const double weight = 1e15;
  assert(matrix.size1() == matrix.size2());

  // Place random values with appropriate bandwidth
  for(std::size_t row=0; row<matrix.size1(); ++row)
  {
    for(std::size_t col=std::max(static_cast<std::size_t>(0), row-bandwidth); col<std::min(matrix.size2(), row+bandwidth+1); ++col)
    {
      if (drand48 () < density)
      {
        const double value = drand48() * weight;
        matrix(row, col) += value;

        if (symmetric)
          matrix(col, row) += value;
      }
    }
  }

  // Make diagonally dominant
  for (typename MatrixType::iterator1 rowIter = matrix.begin1(); rowIter != matrix.end1(); ++rowIter) 
  {
    const typename MatrixType::value_type weight = 1e1;  // arbitrary value to increase the sum by to make strictly dominant
    typename MatrixType::value_type absSum;
    for(typename MatrixType::iterator2 colIter(rowIter.begin()); colIter != rowIter.end(); ++colIter)
    {
      if (colIter.index1() != colIter.index2())
        absSum += std::fabs(*colIter);
    }
    matrix(rowIter.index1(), rowIter.index1()) = absSum + weight;
  }
}

int main(int argc, char* argv[])
{
  GeneratorOptions options;
  options.processOptions(argc, argv);
  srand48(time(NULL));

  const unsigned size = options.getSize();
  const bool symmetric = options.getSymmetric();

  boost::numeric::ublas::mapped_matrix<double> matrix(size, size);

  populate(matrix, size/4, 4.0/size, symmetric);
  
  CCSMatrix<double> csrMatrix(matrix, symmetric);
  std::cout << "Generating " << (symmetric ? "a symmetric" : "an unsymmetric") << " matrix of size " << size << "." << std::endl;
  std::cout << "Number of non-zeros in generated matrix: " << csrMatrix.nnz() << "." << std::endl;

  csrMatrix.writeToFile(options.getOutputFile());
}
