#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([Desolin], [0.1], [fpr02@doc.ic.ac.uk])
AC_CONFIG_SRCDIR([lib/Exceptions.cpp])
AC_LANG_CPLUSPLUS
AM_INIT_AUTOMAKE([subdir-objects])
AM_PROG_CC_C_O

# Checks for programs.
AC_PROG_CXX
AC_PROG_LIBTOOL

# Checks for libraries.
CXXFLAGS="-O3 -Wall -pedantic -pipe"
AX_GCC_ARCHFLAG([no], [CXXFLAGS="$CXXFLAGS $ax_cv_gcc_archflag" CCFLAGS="$CCFLAGS $ax_cv_gcc_archflag"])

AC_ARG_WITH([taskgraph-prefix],
            AC_HELP_STRING([--with-taskgraph-prefix=PFX], [Use the TaskGraph installation in PFX]),
            [
              if test -f "$withval/include/TaskGraph"; then
                CPPFLAGS="$CPPFLAGS -I$withval/include"
                LDFLAGS="$LDFLAGS -R$withval/lib -L$withval/lib"
              else
                AC_MSG_ERROR("$withval does not appear to be a prefix for a TaskGraph installation")
              fi
            ]
            )

AC_ARG_WITH([boost-prefix],
           AC_HELP_STRING([--with-boost-prefix=PFX], [Use the Boost installation in PFX]),
           [
             if test -f "$withval/include/boost/version.hpp"; then
               CPPFLAGS="$CPPFLAGS -I$withval/include"
               LDFLAGS="$LDFLAGS -R$withval/lib -L$withval/lib"
             else
               AC_MSG_ERROR("$withval does not appear to be a prefix for a Boost installation")
             fi
           ]
           )

AC_ARG_WITH([mtl-prefix],
          AC_HELP_STRING([--with-mtl-prefix=PFX], [Use the MTL installation in PFX]),
	  [
	    if test -f "$withval/include/mtl/mtl.h"; then
              CPPFLAGS="$CPPFLAGS -I$withval/include"
	    else
	      AC_MSG_ERROR("$withval does not appear to be a prefix for a MTL installation")
             fi
          ]
          )

AC_TRY_COMPILE([@%:@include <TaskGraph>], [], AC_MSG_NOTICE([Found TaskGraph]), AC_MSG_FAILURE([Failed to find TaskGraph]))
AC_TRY_COMPILE([@%:@include <boost/version.hpp>], [], AC_MSG_NOTICE([Found Boost]), AC_MSG_FAILURE([Failed to find Boost]))
AC_TRY_COMPILE([@%:@include <mtl/mtl.h>], [], [AC_MSG_NOTICE([Found MTL. Building MTL examples.]); using_mtl=yes], [AC_MSG_NOTICE([Failed to find MTL. Not building MTL examples.]); using_mtl=no])

AM_CONDITIONAL([build_mtl_examples], [test "$using_mtl" = yes])


# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_HEADER_STDC
AC_CONFIG_FILES([Makefile
                 include/Makefile
		 lib/Makefile
		 example/Makefile
		 itl-tests/Makefile
		 mtl-itl-tests/Makefile
		 test-matrices/Makefile])
AC_OUTPUT
