include_HEADERS = $(findheaders)

AM_CPPFLAGS = $(TASKINCLUDEPATHS) -I./include
AM_LDFLAGS = $(TASKLIBPATHS) $(TASKLIBS)

lib_LTLIBRARIES = libdesolin.la
libdesolin_la_SOURCES = lib/Exceptions.cc lib/tg/Exceptions.cc lib/tg/NameGenerator.cc lib/tg/ParameterHolder.cc
libdesolin_la_LDFLAGS = -ltaskgraph

bin_PROGRAMS = test
test_SOURCES = example/test.cc
test_LDADD = libdesolin.la
