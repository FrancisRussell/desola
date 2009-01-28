lib_LTLIBRARIES = libdesola-iohb.la libdesola.la

libdesola_la_CPPFLAGS = -I$(top_srcdir)/include
libdesola_la_SOURCES = Exceptions.cpp ConfigurationManager.cpp StatisticsCollector.cpp tg/Exceptions.cpp tg/NameGenerator.cpp tg/ParameterHolder.cpp
libdesola_la_LDFLAGS = -ldesola-iohb -ltaskgraph

libdesola_iohb_la_CPPFLAGS = -I$(top_srcdir)/include/desola/iohb
libdesola_iohb_la_SOURCES = iohb/iohb.c iohb/mmio.c
