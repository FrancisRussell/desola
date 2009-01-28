# This file should be included by any Makefiles in this package
# that need to be linked against the Desola library.

AM_CPPFLAGS=-I$(top_srcdir)/include

DESOLA_LIB = -L$(top_srcdir)/lib/ -ldesola
DESOLA_IOHB_LIB = -L$(top_srcdir)/lib/ -ldesola-iohb
