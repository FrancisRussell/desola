if build_mtl_examples
  OPTIONAL_MTL_DIR = mtl-itl-tests
endif

if build_blas_examples
  OPTIONAL_BLAS_DIR = blas-itl-tests
endif

if build_imkl_examples
  OPTIONAL_IMKL_DIR = imkl-itl-tests
endif

SUBDIRS = include lib example itl-tests test-matrices benchmarks-common matrix-generator scripts $(OPTIONAL_MTL_DIR) $(OPTIONAL_BLAS_DIR) $(OPTIONAL_IMKL_DIR)

EXTRA_DIST = NOTICE LICENSE LICENSE.itl LICENSE.mtl

