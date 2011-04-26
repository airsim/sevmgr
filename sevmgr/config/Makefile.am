include $(top_srcdir)/Makefile.common

## Source directory

DISTCLEANFILES = sevmgr-paths.hpp

MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = sevmgr-paths.hpp

# Targets
all-local: sevmgr-paths.hpp

sevmgr-paths.hpp: Makefile
	@echo '#ifndef __@PACKAGE_NAME@_PATHS_HPP' > $@
	@echo '#define __@PACKAGE_NAME@_PATHS_HPP' >> $@
	@echo >> $@
	@echo '#define PACKAGE "sevmgr"' >> $@
	@echo '#define PACKAGE_NAME "@PACKAGE_NAME@"' >> $@
	@echo '#define PACKAGE_VERSION "@VERSION@"' >> $@
	@echo '#define PREFIXDIR "$(prefix)"' >> $@
	@echo '#define BINDIR "$(bindir)"' >> $@
	@echo '#define LIBEXECDIR "$(libexecdir)"' >> $@
	@echo '#define DATADIR "$(datadir)"' >> $@
	@echo '#define DOCDIR "$(docdir)"' >> $@
	@echo '#define STDAIR_SAMPLE_DIR "$(STDAIR_SAMPLE_DIR)"' >> $@
	@echo >> $@
	@echo '#endif // __@PACKAGE_NAME@_PATHS_HPP' >> $@
	@echo >> $@

