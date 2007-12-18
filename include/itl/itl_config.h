/* itl/itl_config.h.  Generated automatically by configure.  */
/* itl/itl_config.h.in.  Generated automatically from configure.in by autoheader.  */
#ifndef ITL_CONFIG_H
#define ITL_CONFIG_H

/* #undef  ITL_NO_SSTREAM */
/* #undef  ITL_NO_STD_ABS */
/* #undef  ITL_NO_STD_SQRT */
/* #undef  ITL_NO_LIMITS */
#define  ITL_HAVE_CMATH 1


#if defined(_MSC_VER) && !defined(__MWERKS__)
#define _MSVCPP_ _MSC_VER
#endif

#if defined(__sgi) && !defined(__GNUC__) && (_COMPILER_VERSION < 730)
#  define ITL_NO_TEMPLATE
#elif defined(__GNUC__) && (__GNUC__ < 9)
#  define ITL_NO_TEMPLATE
#elif defined(__KCC_VERSION) && (__KCC_VERSION < 3400)
#  define ITL_NO_TEMPLATE
#elif defined(__MWERKS__)
#  define ITL_NO_TEMPLATE
#endif

# ifndef __STL_DEFAULT_ALLOCATOR
#   ifdef __STL_USE_OLD_SGI_ALLOCATOR
#     define __STL_DEFAULT_ALLOCATOR(T) alloc
#   else
#     define __STL_DEFAULT_ALLOCATOR(T) allocator< T >
#   endif
# endif

#ifdef ITL_NO_TEMPLATE
#define ITL_TEMPLATE
#else
#define ITL_TEMPLATE template
#endif

#ifdef ITL_HAVE_CMATH
#include <cmath>
#endif

#ifdef ITL_NO_STD_SQRT
#include <math.h>
namespace std {
  using ::sqrt;
}
#endif

#endif /* ITL_CONFIG_H */
