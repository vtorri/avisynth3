# Copyright 1999-2006 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header$

DESCRIPTION="A circular buffer type for boost"
HOMEPAGE="http://www.boost.org/"
SRC_URI="circular_buffer_v${PV}.tar.bz2"
LICENSE="freedist Boost-1.0"
SLOT="0"
KEYWORDS="x86"
RESTRICT="fetch"
RDEPEND=">=dev-libs/boost-1.33.1"

S=${WORKDIR}/circular_buffer

src_unpack() {
	unpack ${A}
	rm -rf ${S}/circular_buffer/CVS
}

src_install() {
	mkdir -p ${D}/usr/include/boost || die "cannot mkdir"
	cp -R ${S}/circular_buffer* ${D}/usr/include/boost/ ||
		die "failed to copy files"
}
