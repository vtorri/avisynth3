##############################################################################
#
# - Unified Makefile for Avisynth 3.0 for *nix environments -
#
# Copyright(C) 2004-2006 Vincent Torri <vtorri at univ-evry dot fr>
#
#
# Description:
#  This Makefile allows building Avisynth 3.0 sources to obtain a shared
#  library and a static library. This Makefile uses variables defined in
#  the platform.inc file. This platform.inc file is usually created by the
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
#  - U means "_U_ninstalling"
#  - Cl means "_Cl_eaning"
#
# NB: (for mingw32/djgpp users)
#   These 2 environments do not provide a shell by default. So it's impossible
#   to use the configure script to generate a platform.inc file suitable for
#   your machine. You have two choices:
#    - install msys from the mingw project or install cygwin and then use
#      the configure script as on a unix system.
#    - write a platform.inc file by hand.
#
# PS: default build directory is "build", it fits naming conventions that
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
GST_VIDEO_OBJECTS  = $(GENERIC_GST_VIDEO_OBJECTS)
GST_AUDIO_OBJECTS  = $(GENERIC_GST_AUDIO_OBJECTS)

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
distdir=$(TARNAME)-$(VERSION)
distfile=$(distdir).tar

#-----------------------------------------------------------------------------
# The default rule
#-----------------------------------------------------------------------------

.SUFFIXES: .$(OBJECT_EXTENSION) .$(ASSEMBLY_EXTENSION) .cpp .c

all: $(PROG) $(STATIC_LIB) $(SHARED_LIB) $(GST_VIDEO_SINK_LIB) $(GST_AUDIO_SINK_LIB) $(DOC)
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
	$(AVS3_CXXFLAGS) $(PROFILE_CFLAGS) $(AVS3_CPPFLAGS) $< -o $(BUILD_DIR)/$@

#-----------------------------------------------------------------------------
# Generic C rule
#-----------------------------------------------------------------------------

.c.$(OBJECT_EXTENSION):
	@echo "  C: $(@D)/$(<F)"
	@$(INSTALL) -d $(BUILD_DIR)/$(@D)
	@$(CC) -c -fPIC $(ARCHITECTURE) $(BUS) $(ENDIANNESS) $(FEATURES) \
	$(CFLAGS) $(PROFILE_CFLAGS) $(CPPFLAGS) $(GST_CFLAGS) $(GTK_CFLAGS) $(X264_CFLAGS) $< -o $(BUILD_DIR)/$@


#-----------------------------------------------------------------------------
# Gstreamer sinks
#-----------------------------------------------------------------------------

$(GST_VIDEO_SINK_LIB): $(BUILD_DIR) $(GST_VIDEO_OBJECTS)
	@echo "  L: $(@F)"
	@cd $(BUILD_DIR) && $(CC) $(CFLAGS) $(LDFLAGS) $(PROFILE_LDFLAGS) $(SPECIFIC_LDFLAGS) -o $(GST_VIDEO_SINK_LIB) $(GST_VIDEO_OBJECTS) $(GSTBASE_LIBS)

$(GST_AUDIO_SINK_LIB): $(BUILD_DIR) $(GST_AUDIO_OBJECTS)
	@echo "  L: $(@F)"
	@cd $(BUILD_DIR) && $(CC) $(CFLAGS) $(LDFLAGS) $(PROFILE_LDFLAGS) $(SPECIFIC_LDFLAGS) -o $(GST_AUDIO_SINK_LIB) $(GST_AUDIO_OBJECTS) $(GSTBASE_LIBS)

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
#      1/ it links a pre shared lib (libavisynth.so-temp.4)
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
	@cd $(BUILD_DIR) && $(CXX) $(LDFLAGS) $(PROFILE_LDFLAGS) $(LIB_OBJECTS) $(ASM_OBJECTS) -o $(PRE_SHARED_LIB) $(SPECIFIC_LDFLAGS) $(AVS3_LIBS)

#-----------------------------------------------------------------------------
# Test program
#-----------------------------------------------------------------------------
$(PROG):  $(STATIC_LIB) $(BIN_OBJECTS)
	@echo "  P: $(@F)"
	@cd $(BUILD_DIR) && $(CC) -o $(PROG) $(LDFLAGS) $(PROFILE_LDFLAGS) $(BIN_OBJECTS) $(STATIC_LIB) $(AVS3_LIBS) $(GTK_LIBS) $(X264_LIBS)

#-----------------------------------------------------------------------------
# Documentation
#-----------------------------------------------------------------------------
$(DOC): $(BUILD_DIR)
	@echo "  B: documentation"
	@cd $(BUILD_DIR) && $(RM) doc
	@cd $(BUILD_DIR) && mkdir doc
	@cd $(SRC_DIR)/../docs/ && doxygen

#-----------------------------------------------------------------------------
# Installation
#-----------------------------------------------------------------------------

install: $(BUILD_DIR)/$(STATIC_LIB) $(BUILD_DIR)/$(SHARED_LIB) $(BUILD_DIR)/$(PROG) $(BUILD_DIR)/$(GST_VIDEO_SINK_LIB) $(BUILD_DIR)/$(GST_AUDIO_SINK_LIB)
	@if ! test "x$(PROG)" = "x"; then \
	  echo "  D: $(DESTDIR)$(bindir)"; \
	  $(INSTALL) -d $(DESTDIR)$(bindir); \
	  echo "  I: $(DESTDIR)$(bindir)/$(PROG)"; \
	  $(INSTALL) -m 755 $(BUILD_DIR)/$(PROG) $(DESTDIR)$(bindir)/$(PROG); \
	fi
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
	  dir="`echo $$i | sed 's,/[^/]*$$,,'`"; \
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
	@echo "  D: $(DESTDIR)$(pkgconfigdir)"
	@$(INSTALL) -d $(DESTDIR)$(pkgconfigdir)
	@echo "  I: $(DESTDIR)$(pkgconfigdir)/$(AVS_PC)"
	@$(INSTALL) -m 644 ./$(AVS_PC) $(DESTDIR)$(pkgconfigdir)
	@echo "  D: $(DESTDIR)$(GST_PLUGIN_PATH)"
	@$(INSTALL) -d $(DESTDIR)$(GST_PLUGIN_PATH)
	@echo "  I: $(DESTDIR)$(GST_PLUGIN_PATH)/$(GST_VIDEO_SINK_LIB)"
	@$(INSTALL) -m 644 $(BUILD_DIR)/$(GST_VIDEO_SINK_LIB) $(DESTDIR)$(GST_PLUGIN_PATH)/$(GST_VIDEO_SINK_LIB)
	@echo "  I: $(DESTDIR)$(GST_PLUGIN_PATH)/$(GST_AUDIO_SINK_LIB)"
	@$(INSTALL) -m 644 $(BUILD_DIR)/$(GST_AUDIO_SINK_LIB) $(DESTDIR)$(GST_PLUGIN_PATH)/$(GST_AUDIO_SINK_LIB)
	@if ! test "x$(DOC)" = "x"; then \
	  echo "  I: $(DESTDIR)$(docdir)"; \
	  $(INSTALL) -d $(DESTDIR)/$(docdir); \
	  $(INSTALL) -m 644 $(BUILD_DIR)/doc/html/* $(DESTDIR)/$(docdir); \
	fi

#-----------------------------------------------------------------------------
# Uninstallation
#-----------------------------------------------------------------------------

uninstall:
	@if ! test "x$(PROG)" = "x"; then \
	  echo "  U: $(DESTDIR)$(bindir)/$(PROG)"; \
	  $(RM) $(DESTDIR)$(bindir)/$(PROG); \
	fi
	@echo "  U: $(DESTDIR)$(libdir)/$(SHARED_LIB)"
	@$(RM) $(DESTDIR)$(libdir)/$(SHARED_LIB)
	@echo "  U: $(DESTDIR)$(libdir)/libavisynth.$(SHARED_EXTENSION).$(API_MAJOR)"
	@$(RM) $(DESTDIR)$(libdir)/libavisynth.$(SHARED_EXTENSION).$(API_MAJOR)
	@echo "  U: $(DESTDIR)$(libdir)/libavisynth.$(SHARED_EXTENSION)"
	@$(RM) $(DESTDIR)$(libdir)/libavisynth.$(SHARED_EXTENSION)
	@echo "  U: $(DESTDIR)$(libdir)/$(STATIC_LIB)"
	@$(RM) $(DESTDIR)$(libdir)/$(STATIC_LIB)
	@echo "  U: $(DESTDIR)$(includedir)"
	@$(RM) $(DESTDIR)$(includedir)
	@echo "  U: $(DESTDIR)$(pkgconfigdir)/$(AVS_PC)"
	@$(RM) $(DESTDIR)$(pkgconfigdir)/$(AVS_PC)
	@echo "  U: $(DESTDIR)$(GST_PLUGIN_PATH)/$(GST_VIDEO_SINK_LIB)"
	@$(RM) $(DESTDIR)$(GST_PLUGIN_PATH)/$(GST_VIDEO_SINK_LIB)
	@echo "  U: $(DESTDIR)$(GST_PLUGIN_PATH)/$(GST_AUDIO_SINK_LIB)"
	@$(RM) $(DESTDIR)$(GST_PLUGIN_PATH)/$(GST_AUDIO_SINK_LIB)
	@if ! test "x$(DOC)" = "x"; then \
	  echo "  U: $(DESTDIR)$(docdir)"; \
	  $(RM) $(DESTDIR)/$(docdir); \
	fi

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
	@echo "  H: headers dependancies"
	@$(RM) $(BUILD_DIR)/.depend
	@touch $(BUILD_DIR)/.depend
	@cd $(SRC_DIR) && \
	find . -name '*.cpp' | while read i; do \
	  filename="`echo $$i | sed 's:^\./::'`"; \
	  dir="`echo $$i | sed 's,[^/]*$$,,'`"; \
	  echo $$filename; \
	  (echo -n "$$dir" && $(CXX) -MM -g0 $(AVS3_CPPFLAGS) $(GTK_CFLAGS) $$filename) 1>> ../build/linux/$(BUILD_DIR)/.depend; \
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
	@$(RM) $(distfile).gz
	@$(RM) $(distfile).bz2

mrproper: distclean
	@echo "  Cl: Bootstrapped build files"
	@$(RM) configure
	@$(RM) missing
	@$(RM) mkinstalldirs
	@$(RM) aclocal.m4

distdir:
	@echo "  T: $(distfile)"
	@cd ../../ && \
	test ! -d $(distdir) \
	|| { find $(distdir) -type d ! -perm -200 -exec chmod u+w {} ';' \
	&& $RM$ $(distdir); };
	@mkdir -p $(distdir)/build/linux/m4
	@mkdir -p $(distdir)/build/win32/vc71
	@mkdir $(distdir)/docs
	@mkdir $(distdir)/src
	@cp -p ../../TODO $(distdir)
	@cp -p ../INSTALL $(distdir)/build
	@cp -p \
	  Makefile \
	  $(AVS_PC).in \
	  bootstrap.sh \
	  configure \
	  configure.in \
	  config.sub \
	  config.guess \
	  install-sh \
	  gen_sources.sh.in \
	  libavisynth.def \
	  platform.inc.in \
	  $(distdir)/build/linux
	@cd m4 && cp -p \
	  boost.m4 \
	  gst-element-check.m4 \
	  pkg-get-variable.m4 \
	  pkg.m4 \
	  stlport.m4 \
	  visibility.m4 \
	  ../$(distdir)/build/linux/m4
	@cd ../win32/vc71 && cp -p \
	  avisynth.def \
	  avisynth.rc \
	  avisynth.sln \
	  avisynth.vcproj \
	  ../../linux/$(distdir)/build/win32/vc71
	@cd ../../docs && cp -p \
	  Doxyfile \
	  README \
	  avisynth.css \
	  doxygen.css \
	  doxygen.png \
	  installation.c \
	  installation_mingw.c \
	  installation_vc.c \
	  installation_linux.c \
	  ../build/linux/$(distdir)/docs
	@cd $(SRC_DIR)/.. && find src \( -name '*.h' -o -name '*.c' -o -name '*.cpp' \) | while read i; do \
	  dir="`echo $$i | /bin/sed 's,/[^/]*$$,,'`"; \
	  if ! test -d "build/linux/$(distdir)/$$dir" ; then \
	    mkdir -p build/linux/$(distdir)/$$dir; \
	  fi; \
	  cp -p $$i build/linux/$(distdir)/$$dir; \
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
