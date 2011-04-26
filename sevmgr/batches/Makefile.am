# sevmgr/batches
include $(top_srcdir)/Makefile.common

##
# Binaries (batches)
bin_PROGRAMS = sevmgr
generated_FILES = request.csv

##
MAINTAINERCLEANFILES = Makefile.in
CLEANFILES = $(generated_FILES) ${bin_PROGRAMS:%=%.log}

##
#
sevmgr_SOURCES = $(sevmgr_batches_fct_cc_sources)
sevmgr_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
sevmgr_LDADD =
sevmgr_LDFLAGS = $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/sevmgr/core/libsevmgr.la
