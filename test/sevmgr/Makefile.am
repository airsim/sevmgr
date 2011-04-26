## test/sevmgr sub-directory
include $(top_srcdir)/Makefile.common
include $(srcdir)/sources.mk

##
SUBDIRS =

EXTRA_DIST =

##
nofail_PRG = EventQueueManagementTestSuite
fail_PRG =

#
MAINTAINERCLEANFILES = Makefile.in
CLEANFILES = ${nofail_PRG:%=%_utfresults.xml} ${nofail_PRG:%=%.log}

#
check_PROGRAMS = $(nofail_PRG) $(fail_PRG)
TESTS = $(check_PROGRAMS)
XFAIL_TESTS = $(fail_PRG)

#
EventQueueManagementTestSuite_SOURCES = $(sevmgr_test_bin_cc_sources)
EventQueueManagementTestSuite_CXXFLAGS = $(BOOST_CFLAGS) $(STDAIR_CFLAGS)
EventQueueManagementTestSuite_LDADD = 
EventQueueManagementTestSuite_LDFLAGS = \
	$(BOOST_LIBS) $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(STDAIR_LIBS) \
	$(top_builddir)/sevmgr/core/libsevmgr.la

