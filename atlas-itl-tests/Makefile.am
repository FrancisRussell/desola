include $(top_srcdir)/binaries_common.mk

noinst_HEADERS = solver_options.hpp  statistics_generator.hpp
noinst_PROGRAMS = identity_cg identity_qmr identity_cgs identity_bicg identity_bicgstab identity_tfqmr identity_cheby identity_richardson

%.cpp:: ../benchmarks-common/%.cpp
	cp ../benchmarks-common/$@ .

nodist_identity_cg_SOURCES = identity_cg.cpp
identity_cg_SOURCES = solver_options.cpp
identity_cg_CXXFLAGS = -I${top_srcdir}/blas-common
identity_cg_LDFLAGS = ${BOOST_PROGRAM_OPTIONS_LIB} ${BOOST_FILESYSTEM_LIB} ${BLAS_LIBS} ${DESOLA_IOHB_LIB}

nodist_identity_qmr_SOURCES = identity_qmr.cpp 
identity_qmr_SOURCES = solver_options.cpp
identity_qmr_CXXFLAGS = -I${top_srcdir}/blas-common
identity_qmr_LDFLAGS = ${BOOST_PROGRAM_OPTIONS_LIB} ${BOOST_FILESYSTEM_LIB} ${BLAS_LIBS} ${DESOLA_IOHB_LIB}

nodist_identity_cgs_SOURCES = identity_cgs.cpp
identity_cgs_SOURCES = solver_options.cpp
identity_cgs_CXXFLAGS = -I${top_srcdir}/blas-common
identity_cgs_LDFLAGS = ${BOOST_PROGRAM_OPTIONS_LIB} ${BOOST_FILESYSTEM_LIB} ${BLAS_LIBS} ${DESOLA_IOHB_LIB}

nodist_identity_bicg_SOURCES = identity_bicg.cpp
identity_bicg_SOURCES = solver_options.cpp
identity_bicg_CXXFLAGS = -I${top_srcdir}/blas-common
identity_bicg_LDFLAGS = ${BOOST_PROGRAM_OPTIONS_LIB} ${BOOST_FILESYSTEM_LIB} ${BLAS_LIBS} ${DESOLA_IOHB_LIB}

nodist_identity_bicgstab_SOURCES = identity_bicgstab.cpp
identity_bicgstab_SOURCES = solver_options.cpp
identity_bicgstab_CXXFLAGS = -I${top_srcdir}/blas-common
identity_bicgstab_LDFLAGS = ${BOOST_PROGRAM_OPTIONS_LIB} ${BOOST_FILESYSTEM_LIB} ${BLAS_LIBS} ${DESOLA_IOHB_LIB}

nodist_identity_tfqmr_SOURCES = identity_tfqmr.cpp
identity_tfqmr_SOURCES = solver_options.cpp
identity_tfqmr_CXXFLAGS = -I${top_srcdir}/blas-common
identity_tfqmr_LDFLAGS = ${BOOST_PROGRAM_OPTIONS_LIB} ${BOOST_FILESYSTEM_LIB} ${BLAS_LIBS} ${DESOLA_IOHB_LIB}

nodist_identity_cheby_SOURCES = identity_cheby.cpp
identity_cheby_SOURCES = solver_options.cpp
identity_cheby_CXXFLAGS = -I${top_srcdir}/blas-common
identity_cheby_LDFLAGS = ${BOOST_PROGRAM_OPTIONS_LIB} ${BOOST_FILESYSTEM_LIB} ${BLAS_LIBS} ${DESOLA_IOHB_LIB}

nodist_identity_richardson_SOURCES = identity_richardson.cpp
identity_richardson_SOURCES = solver_options.cpp
identity_richardson_CXXFLAGS = -I${top_srcdir}/blas-common
identity_richardson_LDFLAGS = ${BOOST_PROGRAM_OPTIONS_LIB} ${BOOST_FILESYSTEM_LIB} ${BLAS_LIBS} ${DESOLA_IOHB_LIB}
