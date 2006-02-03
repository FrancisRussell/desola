include_HEADERS = include/desolin/Desolin_fwd.hpp

AM_CPPFLAGS = -I$(srcdir)/include

lib_LTLIBRARIES = libdesolin.la
libdesolin_la_SOURCES = lib/Exceptions.cc
libdesolin_la_LIBADD = libdesolintg.la

noinst_LTLIBRARIES = libdesolintg.la
libdesolintg_la_SOURCES = lib/tg/Exceptions.cc lib/tg/NameGenerator.cc lib/tg/ParameterHolder.cc
