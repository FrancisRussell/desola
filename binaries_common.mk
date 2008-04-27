# This file should be included by any Makefiles in this package
# that need to be linked against the Desolin library.

AM_CPPFLAGS=-I$(top_srcdir)/include

DESOLIN_LIB = -L$(top_srcdir)/lib/ -ldesolin
DESOLIN_IOHB_LIB = -L$(top_srcdir)/lib/ -ldesolin-iohb
