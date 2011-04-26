## sevmgr/python sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = pysevmgr.py

# Library
lib_LTLIBRARIES = libpysevmgr.la

libpysevmgr_la_SOURCES = $(sevmgr_python_svc_sources)
libpysevmgr_la_CXXFLAGS = $(PYTHON_CFLAGS) $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
libpysevmgr_la_LIBADD =
libpysevmgr_la_LDFLAGS = $(PYTHON_LIBS) $(PYTHON_ADD_LIBS) \
	$(BOOST_PYTHON_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/sevmgr/core/libsevmgr.la \
	-version-info $(GENERIC_LIBRARY_VERSION)
