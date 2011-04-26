# sevmgr/core
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

## Source directory

MAINTAINERCLEANFILES = Makefile.in

SUBDIRS =


# Library
lib_LTLIBRARIES = libsevmgr.la

libsevmgr_la_SOURCES = $(sevmgr_service_h_sources) $(sevmgr_service_cc_sources)
libsevmgr_la_LIBADD = \
	$(top_builddir)/sevmgr/basic/libsevmgrbas.la \
	$(top_builddir)/sevmgr/bom/libsevmgrbom.la \
	$(top_builddir)/sevmgr/factory/libsevmgrfac.la \
	$(top_builddir)/sevmgr/command/libsevmgrcmd.la \
	$(top_builddir)/sevmgr/service/libsevmgrsvc.la
libsevmgr_la_LDFLAGS = \
	$(BOOST_DATE_TIME_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) $(STDAIR_LIBS) \
	-version-info $(GENERIC_LIBRARY_VERSION)

