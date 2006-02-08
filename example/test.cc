#include <desolin/Desolin.hpp>
#include <TaskGraph>
#include <vector>
#include <iostream>
using namespace desolin;

int main()
{
  Scalar<double> x(10);
  Scalar<double> y(44);
  Scalar<double> c = x+y;
  c = c+c;
  Vector<double> q(5, 4);
  Vector<double> r = q*c;

  Matrix<double> m1(5,4, 0);
  Matrix<double> m2(4,7, 0);
  Matrix<double> m3 = m1*m2;
  std::cout << "m3(1,1) = " << m3(1,1).value() << std::endl;
  m3(1,1) = c;
  std::cout << "m3(1,1) = " << m3 << q << std::endl;
  std::vector< Scalar<double> > hello;
  hello.push_back(m3(1,1) = c);

  harwell_boeing_stream<double> stream("./bcsstk14.rsa");
  Matrix<double> hb(stream);
}
