# This file should be included by any Makefiles in this package
# that need to be linked against the Desolin library.

AM_CPPFLAGS=-I$(top_srcdir)/include
LDADD=$(top_srcdir)/lib/libdesolin.la
