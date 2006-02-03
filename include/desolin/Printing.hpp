#include <iostream>
#include <desolin/Desolin_fwd.hpp>

template<typename T_element>
std::ostream& operator<< (std::ostream& o, const desolin::Scalar<T_element>& s)
{
  return o << s.value();
}

template<typename T_element>
std::ostream& operator<< (std::ostream& o, const desolin::Vector<T_element>& v)
{
  const int rows = v.numRows();
  o << "Vector: " << rows << std:: endl << "[";
  for(int row=0; row<rows; ++row)
  {
    o.width(10);
    o << v(row).value();
    if (row!=rows-1)
      o <<", ";
  }
  o << "]";
  return o << std::endl;
}

template<typename T_element>
std::ostream& operator<< (std::ostream& o, const desolin::Matrix<T_element>& m)
{
  const int rows = m.numRows();
  const int cols = m.numCols();

  o << "Matrix: " << rows << " * " << cols << std::endl << "[" << std::endl;
  for(int row=0; row<rows; ++row)
  {
    o << "[";
    for(int col=0; col<cols; ++col)
    {
      o.width(10);
      o << m(row,col).value();
      if (col!=cols-1)
        o <<", ";
    }
    o << "]" << std::endl;
  }
  o << "]";
  return o << std::endl;
}
