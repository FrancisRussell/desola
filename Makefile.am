include_HEADERS =
EXTRA_HEADERS = 

AM_CPPFLAGS = -I$(srcdir)/include

lib_LTLIBRARIES = libdesolin.la
libdesolin_la_SOURCES = lib/Exceptions.cc lib/tg/Exceptions.cc lib/tg/NameGenerator.cc lib/tg/ParameterHolder.cc
libdesolin_la_LDFLAGS = -ltaskgraph

bin_PROGRAMS = test
test_SOURCES = example/test.cc
test_LDFLAGS = -ldesolin -L$(top_srcdir)
