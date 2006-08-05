include $(top_srcdir)/binaries_common.mk

noinst_HEADERS = solver_options.hpp  statistics_generator.hpp

noinst_PROGRAMS = identity_cg identity_qmr identity_cgs identity_bicg identity_bicgstab identity_tfqmr identity_cheby identity_richardson

identity_cg_SOURCES = identity_cg.cpp
identity_cg_LDFLAGS = -lboost_program_options -lboost_filesystem

identity_qmr_SOURCES = identity_qmr.cpp
identity_qmr_LDFLAGS = -lboost_program_options -lboost_filesystem

identity_cgs_SOURCES = identity_cgs.cpp
identity_cgs_LDFLAGS = -lboost_program_options -lboost_filesystem

identity_bicg_SOURCES = identity_bicg.cpp
identity_bicg_LDFLAGS = -lboost_program_options -lboost_filesystem

identity_bicgstab_SOURCES = identity_bicgstab.cpp
identity_bicgstab_LDFLAGS = -lboost_program_options -lboost_filesystem

identity_tfqmr_SOURCES = identity_tfqmr.cpp
identity_tfqmr_LDFLAGS = -lboost_program_options -lboost_filesystem

identity_cheby_SOURCES = identity_cheby.cpp
identity_cheby_LDFLAGS = -lboost_program_options -lboost_filesystem

identity_richardson_SOURCES = identity_richardson.cpp
identity_richardson_LDFLAGS = -lboost_program_options -lboost_filesystem

EXTRA_DIST = rebuild_with_icpc
