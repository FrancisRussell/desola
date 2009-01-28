#ifndef DESOLA_MAYBE_HPP
#define DESOLA_MAYBE_HPP

#include "Exceptions.hpp"
#include <iosfwd>
#include <boost/static_assert.hpp>
#include <boost/operators.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_convertible.hpp>

namespace desola
{

class Nothing {};

template<typename T>
class Maybe : private boost::operators< Maybe<T> >
{
private:
  BOOST_STATIC_ASSERT(boost::is_arithmetic<T>::value);
  bool nothing;
  T val;

  template<typename T1> 
  friend class Maybe;

public:
  Maybe() : nothing(true)
  {
  }

  Maybe(const Nothing& n) : nothing(true)
  {
  }

  Maybe(const Maybe& m) : nothing(m.nothing), val(m.val)
  {
  }

  template<typename T1>
  Maybe(const Maybe<T1>& m) : nothing(m.nothing), val(m.val)
  {
    BOOST_STATIC_ASSERT((boost::is_convertible<T1, T>::value));
  }

  template<typename T1>
  Maybe(const T1& t) : nothing(false), val(t)
  {
    BOOST_STATIC_ASSERT((boost::is_convertible<T1, T>::value));
  }

  Maybe& operator=(const Maybe& m)
  {
    nothing = m.nothing;
    val = m.val;
    return *this;
  }
  
  Maybe& operator+=(const Maybe& x)
  {
    nothing = nothing || x.nothing;

    if (!nothing)
      val += x.val;
    return *this;
  }

  Maybe& operator-=(const Maybe& x)
  {
    nothing = nothing || x.nothing;

    if (!nothing)
      val -= x.value;
    return *this;
  }

  Maybe& operator*=(const Maybe& x)
  {
    nothing = nothing || x.nothing;

    if (!nothing)
      val *= x.val;
    return *this;
  }

  Maybe& operator/=(const Maybe& x)
  {
    nothing = nothing || x.nothing;

    if (!nothing)
      val /= x.val;
    return *this;
  }

  Maybe& operator++()
  {
    if (!nothing)
      ++val;
    return *this;
  }

  Maybe& operator--()
  {
    if (!nothing)
      --val;
    return *this;
  }

  bool hasValue() const
  {
    return !nothing;
  }

  const T value() const
  {
    if (nothing)
      throw DesolaLogicError("Attempted to retrieve value of an empty Maybe");
    else
      return val;
  }

  template<typename Visitor>
  void staticAccept(Visitor& v) const
  {
    if (nothing)
    {
      const Nothing nothing;
      v(nothing);
    }
    else
    {
      v(val);
    }
  }
};

template<typename T>
std::ostream& operator<< (std::ostream& o, const Maybe<T>& m)
{
  if (m.hasValue())
    return o << m.value();
  else
    return o << "Nothing";
}

}

#endif
