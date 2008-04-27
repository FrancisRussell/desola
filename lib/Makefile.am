lib_LTLIBRARIES = libdesolin-iohb.la libdesolin.la

libdesolin_la_CPPFLAGS = -I$(top_srcdir)/include
libdesolin_la_SOURCES = Exceptions.cpp ConfigurationManager.cpp StatisticsCollector.cpp tg/Exceptions.cpp tg/NameGenerator.cpp tg/ParameterHolder.cpp
libdesolin_la_LDFLAGS = -ldesolin-iohb -ltaskgraph

libdesolin_iohb_la_CPPFLAGS = -I$(top_srcdir)/include/desolin/iohb
libdesolin_iohb_la_SOURCES = iohb/iohb.c iohb/mmio.c
