include $(top_srcdir)/binaries_common.mk

noinst_HEADERS = solver_options.hpp  statistics_generator.hpp

bin_PROGRAMS = identity_cg identity_qmr identity_cgs identity_bicg identity_bicgstab identity_tfqmr identity_cheby identity_richardson

identity_cg_SOURCES = identity_cg.cpp solver_options.cpp
identity_cg_LDFLAGS = ${BOOST_PROGRAM_OPTIONS_LIB} ${BOOST_FILESYSTEM_LIB}

identity_qmr_SOURCES = identity_qmr.cpp solver_options.cpp
identity_qmr_LDFLAGS = ${BOOST_PROGRAM_OPTIONS_LIB} ${BOOST_FILESYSTEM_LIB}

identity_cgs_SOURCES = identity_cgs.cpp solver_options.cpp
identity_cgs_LDFLAGS = ${BOOST_PROGRAM_OPTIONS_LIB} ${BOOST_FILESYSTEM_LIB}

identity_bicg_SOURCES = identity_bicg.cpp solver_options.cpp
identity_bicg_LDFLAGS = ${BOOST_PROGRAM_OPTIONS_LIB} ${BOOST_FILESYSTEM_LIB}

identity_bicgstab_SOURCES = identity_bicgstab.cpp solver_options.cpp
identity_bicgstab_LDFLAGS = ${BOOST_PROGRAM_OPTIONS_LIB} ${BOOST_FILESYSTEM_LIB}

identity_tfqmr_SOURCES = identity_tfqmr.cpp solver_options.cpp
identity_tfqmr_LDFLAGS = ${BOOST_PROGRAM_OPTIONS_LIB} ${BOOST_FILESYSTEM_LIB}

identity_cheby_SOURCES = identity_cheby.cpp solver_options.cpp
identity_cheby_LDFLAGS = ${BOOST_PROGRAM_OPTIONS_LIB} ${BOOST_FILESYSTEM_LIB}

identity_richardson_SOURCES = identity_richardson.cpp solver_options.cpp
identity_richardson_LDFLAGS = ${BOOST_PROGRAM_OPTIONS_LIB} ${BOOST_FILESYSTEM_LIB}

