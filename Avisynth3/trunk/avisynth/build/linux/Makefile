##############################################################################
#
# - Unified Makefile for XviD for *nix environments -
#
# Copyright(C) 2003-2004 Edouard Gomez <ed.gomez@free.fr>
#
#
# Description:
#  This Makefile allows building XviD sources to obtain a shared library
#  and a static library. This Makefile uses variables defined in the
#  platform.inc file. This platform.inc file is usually created by the
#  ./configure script whenever a unix shell is available.
#
# Makefile functional dependencies:
#  - echo
#  - rm (with option -r and -f)
#  - cd
#  - make VPATH support (eg: GNU make, solaris 8 make)
#  - ar
#
# Building output:
#  - C means "_C_ompiling"
#  - A means "_A_ssembling"
#  - I means "_I_nstalling"
#  - D means "creating _D_irectory"
#  - Cl means "_Cl_eaning"
#
# NB: (for mingw32/djgpp users)
#   These 2 environments do not provide a shell by default. So it's impossible
#   to use the configure script to generate a platform.inc file suitable for
#   your machine. You have two choices:
#    - install minsys from the mingw project or install cygwin and then use
#      the configure script as on a unix system.
#    - write a platform.inc file by hand.
#
# PS: default build directory is "=build", it fits naming conventions that
#     make the arch/tla revision control program ignore files contained in
#     this directory during commits operations. This choice is completly
#     arbitrary, but try not to change it.
#
##############################################################################

include sources.inc
include platform.inc

RM = rm -rf

##############################################################################
#
# Build rules
#
##############################################################################

# Their Objects
GST_OBJECTS  = $(GENERIC_GST_OBJECTS)
BIN_OBJECTS  = $(GENERIC_BIN_OBJECTS)
LIB_OBJECTS  = $(GENERIC_LIB_OBJECTS)
ASM_OBJECTS  = $(ASSEMBLY_OBJECTS)

OBJECTS  = $(BIN_OBJECTS)
OBJECTS += $(LIB_OBJECTS)
OBJECTS += $(ASM_OBJECTS)

# The VPATH mechanism could use a "per target" build directory
# To keep it simple at the moment, the directory is fixed to "build"
BUILD_DIR = build
VPATH     = $(SRC_DIR):$(BUILD_DIR)

# The variables used for the dist rule
distdir=$(PACKAGE)-$(VERSION)
distfile=$(distdir).tar

#-----------------------------------------------------------------------------
# The default rule
#-----------------------------------------------------------------------------

.SUFFIXES: .$(OBJECT_EXTENSION) .$(ASSEMBLY_EXTENSION) .cpp .c

all: $(GST_SINK_LIB) $(STATIC_LIB) $(SHARED_LIB) $(PROG)
	@echo
	@echo "---------------------------------------------------------------"
	@echo " Avisynth has been successfully built."
	@echo
	@echo " * Binaries are currently located in the '$(BUILD_DIR)' directory"
	@echo " * To install them on your system, you can run '# make install'"
	@echo "   as root."
	@echo "---------------------------------------------------------------"
	@echo
	@echo " * Installation......: make install"
	@echo " * Install path......: $(prefix)"
	@echo

$(OBJECTS): platform.inc

$(BUILD_DIR):
	@echo "  D: $(BUILD_DIR)"
	@$(INSTALL) -d $(BUILD_DIR)

#-----------------------------------------------------------------------------
# Generic assembly rule
#-----------------------------------------------------------------------------

.$(ASSEMBLY_EXTENSION).$(OBJECT_EXTENSION):
	@echo "  A: $(@D)/$(<F)"
	@$(INSTALL) -d $(BUILD_DIR)/$(@D)
	@$(AS) $(AFLAGS) $< -o $(BUILD_DIR)/$@

#-----------------------------------------------------------------------------
# Generic C++ rule
#-----------------------------------------------------------------------------

.cpp.$(OBJECT_EXTENSION):
	@echo "  C: $(@D)/$(<F)"
	@$(INSTALL) -d $(BUILD_DIR)/$(@D)
	@$(CXX) -c $(ARCHITECTURE) $(BUS) $(ENDIANNESS) $(FEATURES) \
	$(AVS3_DEBUG_CFLAGS) $(AVS3_CPPFLAGS) $(AVS3_CXXFLAGS) $< -o $(BUILD_DIR)/$@

#-----------------------------------------------------------------------------
# Generic C rule
#-----------------------------------------------------------------------------

.c.$(OBJECT_EXTENSION):
	@echo "  C: $(@D)/$(<F)"
	@$(INSTALL) -d $(BUILD_DIR)/$(@D)
	@$(CC) -c $(ARCHITECTURE) $(BUS) $(ENDIANNESS) $(FEATURES) \
	$(CFLAGS) $(GST_CFLAGS) $(GTK_CFLAGS) $(X264_CFLAGS) $< -o $(BUILD_DIR)/$@


#-----------------------------------------------------------------------------
# Gstreamer sink
#-----------------------------------------------------------------------------

$(GST_SINK_LIB): $(GST_OBJECTS)
	@echo "  L: $(@F)"
	@cd $(BUILD_DIR) && $(CC) $(LDFLAGS)  -o $(GST_SINK_LIB) $(GST_OBJECTS) $(GSTBASE_LIBS) $(SPECIFIC_LDFLAGS)

#-----------------------------------------------------------------------------
# Static Library
#-----------------------------------------------------------------------------

$(STATIC_LIB): .depend $(LIB_OBJECTS) $(ASM_OBJECTS) $(SRC_LIB_GENERIC)
	@echo "  L: $(@F)"
	@cd $(BUILD_DIR) && ar rc $(@F) $(LIB_OBJECTS) $(ASM_OBJECTS) && $(RANLIB) $(@F)

#-----------------------------------------------------------------------------
# Shared Library
#
# NB: This rule is used a nasty way by the MacOSX module build process
#     In this only case, it uses the SPECIFIC_LDFLAGS to append an additionnal
#     linking step:
#      1/ it links a pre shared lib (libxvidcore.so-temp.4)
#      2/ it links that pre shared lib outputing the real shared lib (module)
#     In all other cases this rule is straight forward and simple.
#     PRE_SHARED_LIB == SHARED_LIB and no nasty command appending.
#
# NB': we copy the def file for the win32 target, the file is unused on other
#      platforms
#-----------------------------------------------------------------------------

$(SHARED_LIB): .depend $(LIB_OBJECTS) $(ASM_OBJECTS)
	@echo "  L: $(@F)"
	@$(INSTALL) -m 644 libavisynth.def $(BUILD_DIR)/libavisynth.def
	@cd $(BUILD_DIR) && $(CXX) $(LDFLAGS) $(LIB_OBJECTS) $(ASM_OBJECTS) -o $(PRE_SHARED_LIB) $(SPECIFIC_LDFLAGS) $(AVS3_LIBS)

#-----------------------------------------------------------------------------
# Test program
#-----------------------------------------------------------------------------
$(PROG): .depend $(BIN_OBJECTS) $(STATIC_LIB)
	@echo "  P: $(@F)"
	@cd $(BUILD_DIR) && $(CC) -o $(PROG) $(BIN_OBJECTS) $(AVS3_LIBS) $(STATIC_LIB) $(ASM_OBJECTS) $(GTK_LIBS) $(X264_LIBS)

#-----------------------------------------------------------------------------
# Installation
#-----------------------------------------------------------------------------

install: $(BUILD_DIR)/$(STATIC_LIB) $(BUILD_DIR)/$(SHARED_LIB) $(BUILD_DIR)/$(PROG) $(BUILD_DIR)/$(GST_SINK_LIB)
	@echo "  D: $(DESTDIR)$(bindir)"
	@$(INSTALL) -d $(DESTDIR)$(bindir)
	@echo "  I: $(DESTDIR)$(bindir)/$(BIN_OBJECT)"
	@$(INSTALL) -m 755 $(BUILD_DIR)/$(PROG) $(DESTDIR)$(bindir)/$(PROG)
	@echo "  D: $(DESTDIR)$(libdir)"
	@$(INSTALL) -d $(DESTDIR)$(libdir)
	@echo "  I: $(DESTDIR)$(libdir)/$(SHARED_LIB)"
	@$(INSTALL) -m 644 $(BUILD_DIR)/$(SHARED_LIB) $(DESTDIR)$(libdir)/$(SHARED_LIB)
	@echo "  I: $(DESTDIR)$(libdir)/libavisynth.$(SHARED_EXTENSION).$(API_MAJOR)"
	@$(LN_S) -f $(libdir)/$(SHARED_LIB) $(DESTDIR)$(libdir)/libavisynth.$(SHARED_EXTENSION).$(API_MAJOR)
	@echo "  I: $(DESTDIR)$(libdir)/libavisynth.$(SHARED_EXTENSION)"
	@$(LN_S) -f $(libdir)/$(SHARED_LIB) $(DESTDIR)$(libdir)/libavisynth.$(SHARED_EXTENSION)
	@echo "  I: $(DESTDIR)$(libdir)/$(STATIC_LIB)"
	@$(INSTALL) -m 644 $(BUILD_DIR)/$(STATIC_LIB) $(DESTDIR)$(libdir)/$(STATIC_LIB)
	@echo "  D: $(DESTDIR)$(includedir)"
	@$(INSTALL) -d $(DESTDIR)$(includedir)
	@cd $(SRC_DIR) && \
	find . -name '*.h' | while read i; do \
	  filename="`echo $$i | sed 's:^\./::'`"; \
	  dir="`echo $$filename | sed 's:\(.*\)/.*:\1:'`"; \
	  if test "$$dir" = "$$filename" ; then \
	    dir=""; \
	  fi; \
	  if test "$$dir" != "avisynth_c/private"; then \
	    install -d $(DESTDIR)$(includedir)/$$dir; \
	    echo "  I: $(DESTDIR)$(includedir)/$$filename"; \
	    if test "$$filename" = "core.h" ; then \
	      install -m 644 $$filename $(DESTDIR)$(includedir)/avisynth.h; \
	    else \
	      install -m 644 $$filename $(DESTDIR)$(includedir)/$$filename; \
	    fi; \
	  fi \
	done
	@echo "  D: $(DESTDIR)$(datadir)"
	@$(INSTALL) -d $(DESTDIR)$(datadir)
	@echo "  D: $(DESTDIR)$(pkgconfigdir)"
	@$(INSTALL) -d $(DESTDIR)$(pkgconfigdir)
	@echo "  I: $(DESTDIR)$(pkgconfigdir)/$(AVS_PC)"
	@$(INSTALL) -m 644 ./$(AVS_PC) $(DESTDIR)$(pkgconfigdir)
	@echo "  D: $(DESTDIR)$(GST_PLUGIN_PATH)"
	@$(INSTALL) -d $(DESTDIR)$(GST_PLUGIN_PATH)
	@echo "  I: $(DESTDIR)$(GST_PLUGIN_PATH)/$(GST_SINK_LIB)"
	@$(INSTALL) -m 755 $(BUILD_DIR)/$(GST_SINK_LIB) $(DESTDIR)$(GST_PLUGIN_PATH)/$(GST_SINK_LIB)

#-----------------------------------------------------------------------------
# Platorm specific file -- dumb rules for people executing make before
# ./configure or even ./bootstrap.sh
#-----------------------------------------------------------------------------

platform.inc: configure platform.inc.in
	./configure

configure:
	./bootstrap.sh

#-----------------------------------------------------------------------------
# .depend targets
#-----------------------------------------------------------------------------

.depend: $(BUILD_DIR)
	@echo "  H: headers dependances"
	@$(RM) $(BUILD_DIR)/.depend
	@touch $(BUILD_DIR)/.depend
	@cd $(SRC_DIR) && \
	find . -name '*.cpp' | while read i; do \
	  echo $$filename; \
	  filename="`echo $$i | sed 's:^\./::'`"; \
	  dir="`echo $$filename | sed 's:\(.*\)/.*:\1:'`"; \
	  (echo -n "$$dir/" && $(CXX) -MM -g0 $(AVS3_CPPFLAGS) $(GTK_CFLAGS) $$filename) 1>> ../build/linux/$(BUILD_DIR)/.depend; \
	done

#-----------------------------------------------------------------------------
# .PHONY targets
#-----------------------------------------------------------------------------

.PHONY: mrproper distclean clean dist info \
	list-objects list-targets list-install-path list-flags

clean:
	@echo "  Cl: Build directory"
	@$(RM) $(BUILD_DIR)

distclean: clean
	@echo "  Cl: Generated build files"
	@$(RM) sources.inc
	@$(RM) platform.inc
	@$(RM) $(AVS_PC)
	@$(RM) gen_sources.sh
	@$(RM) config.log
	@$(RM) config.status
	@$(RM) autom4te.cache

mrproper: distclean
	@echo "  Cl: Bootstrapped build files"
	@$(RM) configure
	@$(RM) missing
	@$(RM) mkinstalldirs
	@$(RM) aclocal.m4

distdir:
	@cd ../../ && \
	test ! -d $(distdir) \
	|| { find $(distdir) -type d ! -perm -200 -exec chmod u+w {} ';' \
	&& $RM$ $(distdir); };
	@mkdir $(distdir)
	@cp -pR ../win32 \
	../INSTALL \
	../linux/Makefile ../linux/$(AVS_PC).in ../linux/bootstrap.sh ../linux/configure ../linux/configure.in ../linux/config.h.in ../linux/config.sub ../linux/config.guess ../linux/gen_sources.sh.in ../linux/libavisynth.def ../linux/platform.inc.in ../linux/m4 \
	../../docs/Doxyfile ../../docs/README ../../docs/avisynth.css ../../docs/gendoc ../../docs/installdocs.c.in \
	$(SRC_DIR)/ $(distdir)
	@find $(distdir) -path '*CVS' | while read i; do \
	  $(RM) $$i; \
	done

dist-tar: distdir
	@tar cf $(distfile) $(distdir)

dist-gzip: dist-tar
	@echo "  T: $(distfile).gz"
	@gzip -9 -c $(distfile) > $(distfile).gz

dist-bzip2: dist-tar
	@echo "  T: $(distfile).bz2"
	@bzip2 -9 -c $(distfile) > $(distfile).bz2

dist: dist-gzip dist-bzip2
	@$(RM) $(distdir) $(distfile)

list-objects:
	@echo
	@echo "---------------------------------------------------------------"
	@echo "Object files used for this build"
	@echo "---------------------------------------------------------------"
	@echo
	@echo $(LIB_OBJECTS)
	@echo
	@echo $(ASM_OBJECTS)
	@echo
	@echo $(BIN_OBJECTS)
	@echo

list-targets:
	@echo
	@echo "---------------------------------------------------------------"
	@echo "Target Libraries"
	@echo "---------------------------------------------------------------"
	@echo
	@echo Shared library: $(SHARED_LIB)
	@echo Static library: $(STATIC_LIB)
	@echo

list-install-path:
	@echo
	@echo "---------------------------------------------------------------"
	@echo "Install Paths"
	@echo "---------------------------------------------------------------"
	@echo
	@echo Binary  Directory: $(bindir)
	@echo Include Directory: $(includedir)/avisynth-3.0
	@echo Library Directory: $(libdir)
	@echo

list-flags:
	@echo
	@echo "---------------------------------------------------------------"
	@echo "Using FLAGS"
	@echo "---------------------------------------------------------------"
	@echo
	@echo CXXFLAGS=$(CXXFLAGS) $(SPECIFIC_CFLAGS)
	@echo
	@echo STLPORT_CFLAGS=$(STLPORT_CFLAGS)
	@echo
	@echo BOOST_CFLAGS=$(BOOST_CFLAGS)
	@echo
	@echo GST_CFLAGS=$(GST_CFLAGS)
	@echo
	@echo FREETYPE_CFLAGS=$(FREETYPE_CFLAGS)
	@echo

info: list-objects list-flags list-targets list-install-path

ifneq ($(wildcard $(BUILD_DIR)/.depend),)
include $(BUILD_DIR)/.depend
endif
