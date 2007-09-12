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
AX_GCC_ARCHFLAG([no], [CXXFLAGS="$CXXFLAGS $ax_cv_gcc_archflag" CCFLAGS="$CCFLAGS $ax_cv_gcc_archflag"], [])

AC_ARG_WITH([taskgraph],
            AC_HELP_STRING([--with-taskgraph=PFX], [Use the TaskGraph installation in PFX]),
            [
              if test -f "$withval/include/TaskGraph"; then
                CXXFLAGS="$CXXFLAGS -I$withval/include"
                LDFLAGS="$LDFLAGS -R$withval/lib -L$withval/lib"
              else
                AC_MSG_ERROR("$withval does not appear to be a prefix for a TaskGraph installation")
              fi
            ]
            )

AC_ARG_WITH([mtl],
          AC_HELP_STRING([--with-mtl=PFX], [Use the MTL installation in PFX]),
	  [
	    if test -f "$withval/include/mtl/mtl.h"; then
              CXXFLAGS="$CXXFLAGS -I$withval/include"
	    else
	      AC_MSG_ERROR("$withval does not appear to be a prefix for a MTL installation")
             fi
          ]
          )

AX_BOOST_BASE([1.33])
AX_BOOST_PROGRAM_OPTIONS
AX_BOOST_FILESYSTEM

if test "$want_boost" = no; then
  AC_MSG_FAILURE([Boost is a required dependency and cannot be disabled.])
fi


AC_TRY_COMPILE([@%:@include <TaskGraph>], [], AC_MSG_NOTICE([Found TaskGraph]), AC_MSG_FAILURE([Failed to find TaskGraph]))
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
		 test-matrices/Makefile
		 scripts/Makefile])
AC_OUTPUT
