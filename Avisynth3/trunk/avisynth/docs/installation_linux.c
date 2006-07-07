/**
 * @page build_linux Building AviSynth 3.0 for GNU/Linux (using g++)
 *
 * @section summary Summary
 *
 * @li @ref build_linux_presentation
 * @li @ref build_linux_download_dependencies
 * @li @ref build_linux_install_stlport
 * @li @ref build_linux_install_boost
 * @li @ref build_linux_install_freetype
 * @li @ref build_linux_install_fontconfig
 * @li @ref build_linux_install_gstreamer
 * @li @ref build_linux_install_nasm
 * @li @ref build_linux_install_avisynth
 *
 * @section build_linux_presentation Presentation
 *
 * To install Avisynth 3.0, we will need the following libraries:
 *
 * @li STLPort (another port of the STL (Standard Template Library)
 * @li Boost (set of libraries, written in C++, used for the parser,
 * the threads, the pointers, etc...)
 * @li Freetype (a high quality font engine)
 * @li Fontconfig (library for font customization and configuration)
 * @li Gstreamer (multimedia framework)
 *
 * This section explains how to install these libraries, taking into
 * account the system. We will download all the libraries in the same
 * directory, say \em avs_main_dir, in the home directory. In a
 * terminal, type
 * @code
cd
mkdir avs_main_dir
 * @endcode
 *
 * @note
 * Shell commands are given throughout this section. Commands in each
 * subsection are independant and can (have to...) be used as is to
 * install the libraries.
 *
 * @section build_linux_download_dependencies Download dependencies
 *
 * First, download the following libraries in the \em avs_main_dir
 * directory:
 * @li <a href=http://prdownloads.sourceforge.net/stlport/STLport-5.0.2.tar.bz2?download>STLport 5.0.2</a>
 * @li <a href=http://prdownloads.sourceforge.net/boost/boost_1_33_1.tar.bz2?download>Boost 1.33.1</a>
 * @li <a href=http://prdownloads.sourceforge.net/boost/boost-jam-3.1.12-1-linuxx86.tgz?download>bjam (make-like utility)</a>
 * @li <a href=http://prdownloads.sourceforge.net/freetype/freetype-2.2.1.tar.bz2?download>Freetype 2.2.1</a>
 * @li <a href=http://fontconfig.org/release/fontconfig-2.3.2.tar.gz>Fontconfig 2.3.2</a>
 * @li <a href=http://liboil.freedesktop.org/download/liboil-0.3.9.tar.gz>liboil 0.3.9</a>
 * @li <a href=http://gstreamer.freedesktop.org/src/gstreamer/gstreamer-0.10.8.tar.bz2>Gstreamer 0.10.8</a>
 * @li <a href=http://gstreamer.freedesktop.org/src/gst-plugins-base/gst-plugins-base-0.10.8.tar.bz2>Gst-Plugins-Base 0.10.8</a>
 * @li <a href=http://gstreamer.freedesktop.org/src/gst-plugins-good/gst-plugins-good-0.10.3.tar.bz2>Gst-Plugins-Good 0.10.3</a>
 * @li <a href=http://gstreamer.freedesktop.org/src/gst-plugins-bad/gst-plugins-bad-0.10.3.tar.bz2>Gst-Plugins-Bad 0.10.3</a>
 * @li <a href=http://gstreamer.freedesktop.org/src/gst-plugins-ugly/gst-plugins-ugly-0.10.3.tar.bz2>Gst-Plugins-Ugly 0.10.3</a>
 * @li <a href=http://gstreamer.freedesktop.org/src/gstffmpeg/gst-ffmpeg-0.10.1.tar.bz2>Gst-FFmpeg-Ugly 0.10.1</a>
 * @li <a href=http://prdownloads.sourceforge.net/nasm/nasm-0.98.39.tar.bz2?download>nasm 0.98.39</a>
 *
 * @section build_linux_install_stlport Installation of STLport 5.0.2
 *
 * Install STLport in the @em /usr/local directory:
 *
 * @code
cd avs_main_dir
tar jxvf STLport-5.0.2.tar.bz2
cd STLport/build/lib
make -f gcc.mak depend
make -f gcc.mak
make -f gcc.mak all-static
make -f gcc.mak install
make -f gcc.mak install-static
 * @endcode
 *
 * The libraries are located in @em STLport/lib and the headers in
 * @em STLport/stlport.
 *
 * @li If one wants to install STLPort in @em /usr/local, go the the
 * @em STLport directory and copy the files manually (while being
 * root, of course):
 *
 * @code
cd ../..
su <type the root password>
cp lib/libstlport* /usr/local/lib/
cp -R stlport /usr/local/include/
<Ctrl-d>
 * @endcode
 *
 * @note
 * STLport will be automatically found if installed in @em /usr/local.
 *
 * @li If one wants to install it locally, say for example
 * @em \$HOME/local, just do that:
 *
 * @code
cd ../..
cp lib/libstlport* $HOME/local/lib/
cp -R stlport $HOME/local/include/
 * @endcode
 *
 * @section build_linux_install_boost Installation of Boost 1.33.1
 *
 * Install Boost in the @em /usr/local directory:
 *
 * @code
cd
cd avs_main_dir
tar jxvf boost_1_33_1.tar.bz2
tar zxvf boost-jam-3.1.12-1-linuxx86.tgz
cd boost_1_33_1
su <type the root password>
../boost-jam-3.1.12-1-linuxx86/bjam "-sTOOLS=gcc" --prefix=/usr/local --with-thread install
<Ctrl-d>
 * @endcode
 *
 * If one wants to install it locally (like in @ref build_linux_install_stlport),
 * just replace @em --prefix=/usr/local by @em --prefix=\$HOME/local/.
 *
 * Remark that Boost will be automatically found if installed in @em /usr/local.
 *
 * @note
 * For more informations about installing Boost, go to the
 * <a href=http://www.boost.org/more/getting_started.html>Installation</a>
 * section of Boost.
 *
 * @section build_linux_install_freetype Installation of Freetype 2.2.1
 *
 * Avisynth 3.0 displays text with a very efficient font engine, named
 * Freetype. Install it in the @em /usr/local directory:
 *
 * @code
cd
cd avs_main_dir
tar jxvf freetype-2.2.1.tar.bz2
cd freetype-2.2.1
./configure --prefix=/usr/local
make
su <type the root password>
make install
<Ctrl-d>
 * @endcode
 *
 * If one wants to install it elsewhere, change the value given to the
 * @em --prefix parameter.
 *
 * @section build_linux_install_fontconfig Installation of Fonconfig 2.3.2
 *
 * Install Fontconfig in the @em /usr/local directory:
 *
 * @code
cd
cd avs_main_dir
tar jxvf fontconfig-2.3.2.tar.bz2
cd fontconfig-2.3.2
./configure --prefix=/usr/local
make
su <type the root password>
make install
<Ctrl-d>
 * @endcode
 *
 * If one wants to install it
 * elsewhere, change the value given to the @em --prefix parameter.
 *
 * @section build_linux_install_gstreamer Installation of Gstreamer 0.10 and al.
 *
 * Gstreamer is a multimedia framework, similar to DirectShow in the
 * Windows(tm) world. Gstreamer 0.10 is now supported. Gstreamer is only
 * the core of the framework. To decode the streams, you need to install
 * several other pckages : Gst-Plugins-Base, Gst-Plugins-Good,
 * Gst-Plugins-Bad, Gst-Plugins-Ugly, Gst-FFmpeg. In addition, Gstreamer
 * relies now on a small library, that provides several optimized
 * functions that are video-related.
 *
 * Install these libraries in the @em /usr/local directory:
 *
 * @code
cd ..
tar zxvf liboil-0.3.9.tar.gz
cd liboil-0.3.9
./configure --prefix=/usr/local
make
su <type the root password>
make install
<Ctrl-d>
 * @endcode
 *
 * @code
cd ..
tar jxvf gstreamer-0.10.8.tar.bz2
cd gstreamer-0.10.8
./configure --prefix=/usr/local
make
su <type the root password>
make install
<Ctrl-d>
 * @endcode
 *
 * @code
d ..
tar jxvf gst-plugins-base-0.10.8.tar.bz2
cd gst-plugins-base-0.10.8
./configure --prefix=/usr/local
make
su <type the root password>
make install
<Ctrl-d>
 * @endcode
 *
 * @code
cd ..
tar jxvf gst-plugins-good-0.10.3.tar.bz2
cd gst-plugins-good-0.10.3
./configure --prefix=/usr/local
make
su <type the root password>
make install
<Ctrl-d> 
 * @endcode
 *
 * @code
cd ..
tar jxvf gst-plugins-bad-0.10.3.tar.bz2
cd gst-plugins-bad-0.10.3
./configure --prefix=/usr/local
make
su <type the root password>
make install
<Ctrl-d>
 * @endcode
 *
 * @code
cd ..
tar jxvf gst-plugins-ugly-0.10.3.tar.bz2
cd gst-plugins-ugly-0.10.3
./configure --prefix=/usr/local
make
su <type the root password>
make install
<Ctrl-d>
 * @endcode
 *
 * @code
cd ..
tar jxvf gst-ffmpeg-0.10.1.tar.bz2
cd gst-ffmpeg-0.10.1
./configure --prefix=/usr/local
make
su <type the root password>
make install
<Ctrl-d>
 * @endcode
 *
 * If you want to install it elsewhere, change the value given to the
 * @em --prefix parameter. 
 *
 * @section build_linux_install_nasm Installation of an assembler
 *
 * Install nasm in the @em /usr/local directory:
 *
 * @code
cd ../
tar jxvf nasm-0.98.39.tar.bz2
cd nasm-0.98.39 
./configure --prefix=/usr/local
make
su <type the root password>
make install
<Ctrl-d>
 * @endcode
 *
 * If one wants to install it elsewhere, change the value given to the
 * @em --prefix parameter.
 *
 * @section build_linux_install_avisynth Installation of Avisynth 3.0
 *
 * Now, we may build Avisynth 3.0 ! Just after having installed 3 more
 * libraries.
 *
 * Ok, ok, I'm kidding...
 *
 * First we have to download the source code which is in cvs:
 *
 * @code
cd
cd avs_main_dir
cvs -z3 -d:pserver:anonymous@avisynth2.cvs.sourceforge.net:/cvsroot/avisynth2 login
cvs -z3 -d:pserver:anonymous@avisynth2.cvs.sourceforge.net:/cvsroot/avisynth2 co -r avisynth_3_0 avisynth
 * @endcode
 *
 * Avisynth 3.0 needs an extension to boost, not yet included in the
 * current version of boost. This extension is provided in the \em build
 * directory of avisynth. It suffices to unzip it and copy the files to
 * the boost include directory:
 *
 * @code
cd avisynth/build
tar jxvf circular_buffer_v3.7.tar.bz2
cd circular_buffer
 * @endcode
 *
 * @li If Boost is installed in @em /usr/local, you have to be root:
 * @code
su <enter the root password>
cp -R circular_buffer* boost /usr/local/include/boost-1_33_1/
<Ctrl-d>
 * @endcode
 *
 * @li If Boost is installed in, say @em \$HOME/local:
 *
 * @code
cp -R circular_buffer* boost $HOME/local/include/boost-1_33_1/
 * @endcode
 *
 * And now, we can configure Avisynth 3.0:
 *
 * @code
cd ../linux
./booststrap.sh
./configure
 * @endcode
 *
 * If configure is not able to find automatically one or more library,
 * we have to specify its path. Assuming that you have
 * installed everything in @em /usr/:
 *
 * @code
./configure --with-stl-path=/usr --with-boost-path=/usr
 * @endcode
 *
 * If you have installed some of the above libraries elsewhere, just
 * replace @em /usr by the correct path. The configure script should be
 * able to find automatically Freetype.
 *
 * @note
 * If you want to make a debug build of Avisynth 3.0, just pass to the
 * @em configure script the options @em --enable-core-debug (for
 * Avisynth itself).
 * @note
 * If you want to build this documentation,  just pass to the
 * @em configure script the options @em --enable-doc
 *
 * To build Avisynth, we use the usual commands:
 *
 * @code
make
make install
 * @endcode
 */
