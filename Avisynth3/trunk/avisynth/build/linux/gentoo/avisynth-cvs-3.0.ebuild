# Copyright 1999-2006 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header$

inherit eutils cvs

DESCRIPTION="Live CVS ebuild for the AviSynth frameserver"
HOMEPAGE="http://avisynth3.unite-video.com/"
SRC_URI=""
LICENSE="GPL-2"
SLOT="0"
KEYWORDS="~x86 ~amd64"
IUSE="debug doc ffmpeg gtk profile"

DEPEND="x86? ( >=dev-lang/nasm-0.98.39-r3 )
	doc? ( >=app-doc/doxygen-1.4.4 )
	ffmpeg? ( >=media-plugins/gst-plugins-ffmpeg-0.10.1 )
	gtk? ( >=x11-libs/gtk+-2.8.12 >=media-video/x264-svn-encoder-20061014 )
	>=dev-libs/boost-1.33.1 >=dev-libs/boost-circular-buffer-3.7
	>=media-libs/fontconfig-2.3.2-r1 >=media-libs/freetype-2.1.10
	>=media-libs/gstreamer-0.10.8 >=media-libs/gst-plugins-base-0.10.8
	>=dev-libs/STLport-5.1.0"

ECVS_SERVER="avisynth2.cvs.sourceforge.net:/cvsroot/avisynth2"
ECVS_USER="anonymous"
ECVS_PASS=""
ECVS_AUTH="pserver"
ECVS_MODULE="avisynth"
ECVS_BRANCH="avisynth_3_0"

S=${WORKDIR}/${ECVS_MODULE}

pkg_setup() {
	if ! built_with_use dev-libs/boost threads ; then
		eerror "dev-libs/boost must be built with the threads use flag enabled"
		die "boost with threads required"
	fi

	if use debug && ! built_with_use dev-libs/boost debug ; then
		eerror "If you wish to build avisynth with debug support, then you must"
		eerror "also build dev-libs/boost with debug support"
		die "boost with debug required for avisynth with debug"
	fi

	if use gtk && ! built_with_use media-video/x264-svn-encoder gtk ; then
		eerror "If you wish to build the optional (GTK) GUI encoding program"
		eerror "then you must build media-video/x264-svn-encoder with gtk support"
		die "x264-svn-encoder with gtk required for avisynth with gtk"
	fi
}

src_unpack() {
	cvs_src_unpack
	#unpack ${A}
	cd ${S}
	# Dependency calculation is only useful for checking the freshness of a
	# build directory. Portage always starts from a clean one, so we save
	# ourselves some build time by removing the checks.
	epatch ${FILESDIR}/gentoo-remove-unnecessary-depend-check.patch
	cd ${S}/build/linux
	sh bootstrap.sh
}

src_compile() {
	cd ${S}/build/linux

	econf --with-stlport-path=/usr --with-stlport-lib-name=libstlport.so \
		--with-stlport-lib-debug-name=libstlportg.so --with-boost-path=/usr \
		--with-boost-includedir-path=/usr/include \
		--with-boost-lib-debug-name=boost_thread-gcc-mt-d \
		--with-boost-lib-name=boost_thread-gcc-mt $(use_enable doc ) \
		$(use_enable gtk gui ) $(use_enable debug core-debug ) \
		$(use_enable profile ) || die "Configure failed"

	emake || die "Build failed"
}

src_install() {
	cd ${S}/build/linux
	make DESTDIR=${D} install || die "Installation failed"
}

pkg_postinst() {
	if ! use ffmpeg ; then
		einfo "It is recommended that you install media-plugins/gst-plugins-ffmpeg"
		einfo "You must use version 0.10.1 or higher. This will allow AviSynth to"
		einfo "open video using most common codecs (XviD, Huffyuv, etc). Enable"
		einfo "the ffmpeg USE flag to make gst-plugins-ffmpeg a dependency."
	fi
}
