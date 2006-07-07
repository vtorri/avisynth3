/**
 * @page build_windows_mingw Building Avisynth 3.0 with Msys/Mingw
 *
 * @section summary Summary
 *
 * @li @ref build_mingw_presentation
 * @li @ref build_mingw_download_dependencies
 * @li @ref build_mingw_install_msys_mingw
 * @li @ref build_mingw_install_stlport
 * @li @ref build_mingw_install_boost
 * @li @ref build_mingw_install_freetype
 * @li @ref build_mingw_install_fontconfig
 * @li @ref build_mingw_install_nasm
 * @li @ref build_mingw_install_avisynth
 *
 * @section build_mingw_presentation Presentation
 *
 * To install Avisynth 3.0 with MinGW, we will need to install MinGW
 * and the following libraries:
 *
 * @li STLPort (another port of the STL (Standard Template Library)
 * @li Boost (set of libraries, written in C++, used for the parser,
 * the threads, the pointers, etc...)
 * @li Freetype (a high quality font engine)
 * @li Fontconfig (library for font customization and configuration)
 *
 * This section explains how to install MinGW (with MSYS) and these
 * libraries, taking into account the system. We will download all the
 * libraries in the same directory, say \em avs_main_dir, in the home
 * directory. In a terminal, type
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
 * @section build_mingw_download_dependencies Download dependencies
 *
 * First, download the following libraries in a folder, say @em C:/tmp.
 * @li <a href=http://prdownloads.sourceforge.net/mingw/MSYS-1.0.11-2004.04.30-1.exe?download>MSYS 1.0.11</a>
 * @li <a href=http://prdownloads.sf.net/mingw/MinGW-5.0.3.exe?download>MinGW 5.0.3</a>
 * @li <a href=http://prdownloads.sourceforge.net/stlport/STLport-5.0.2.tar.bz2?download>STLport 5.0.2</a>
 * @li <a href=http://prdownloads.sourceforge.net/boost/boost_1_33_1.tar.bz2?download>Boost 1.33.1</a>
 * @li <a href=http://prdownloads.sourceforge.net/boost/boost-jam-3.1.12-1-ntx86.zip?download>bjam (make-like utility)</a>
 * @li <a href=http://prdownloads.sourceforge.net/freetype/freetype-2.2.1.tar.bz2?download>Freetype 2.2.1</a>
 * @li <a href=http://fontconfig.org/release/fontconfig-2.3.2.tar.gz>Fontconfig 2.3.2</a>
 * @li <a href=http://prdownloads.sourceforge.net/nasm/nasm-0.98.39-win32.zip?download>nasm 0.98.39</a>
 *
 * @section build_mingw_install_msys_mingw Installation of MSYS / MinGW
 *
 * From its home page, <a href="http://www.mingw.org">MinGW</a> is a
 * collection of freely available and freely distributable Windows
 * specific header files and import libraries combined with GNU
 * toolsets that allow one to produce native Windows programs that do
 * not rely on any 3rd-party C runtime DLLs." In particular, the GNU
 * gcc compiler is available for windows natively.
 *
 * Again from the MinGW home page, "MSYS is A Minimal SYStem to
 * provide POSIX/Bourne configure scripts the ability to execute and
 * create a Makefile used by make." In particular, it allows to use
 * tools like autoconf, automake, libtool, make, exactly like on a Unix
 * system. This simplify the work when trying to make Avisynth
 * compiling natively on windows and linux without too much extra
 * work.
 *
 * The plan of this section is the following:
 *
 * @li @ref build_mingw_install_msys
 * @li @ref build_mingw_install_mingw
 * @li @ref build_mingw_setting_environment
 *
 * @subsection build_mingw_install_msys Installation of MSYS
 *
 * MSYS is installed from an executable file which has been downloaded
 * in the previous section.
 * When you are asked to install it in a subdirectory, choose for
 * example 'C:/Msys/1.0'. @b Warning: do not install it in any forlder
 * that contains an empty space, like 'C:/Program Files'. Otherwise,
 * the g++ compiler will not be able to compile any program.
 *
 * @subsection build_mingw_install_mingw Installation of MinGW
 *
 * MinGW is installed from an installer that download the needed
 * programs from
 * <a href="http://www.sourceforge.net">SourceForge</a>.
 * The installer has been downloaded previously.
 * Execute the file and choose the "current" version of the files to
 * install. Click on OK. Then, select only g++ in the packages to
 * install. Click on OK. When you are asked to choose the folder where
 * to install it, choose 'C:/Msys/1.0/mingw'.
 *
 * @subsection build_mingw_setting_environment Setting the environment
 *
 * All the programs that don't rely explicitely on mingw will be
 * installed in /usr/local/bin (that is in
 * C:/Msys/1.0/local/bin). So you have to add to the PATH variable the
 * folders C:/Msys/1.0/local/bin and C:/Msys/1.0/mingw/bin. To do
 * this, for W2K for example : go to the desktop, right click on 'My
 * Computer' -> Properties -> Advanced tab -> Environment Variables ->
 * select the Variable 'path' -> edit -> add the two folders above:
 *
 * @code
***;C:/Msys/1.0/bin;C:/Msys/1.0/mingw/bin
 * @endcode
 *
 * All the libraries that don't rely explicitely on mingw will be
 * installed in /usr/local/lib (that is in
 * C:/Msys/1.0/local/lib). So, to tell MinGW that libraries are in the
 * this directory, you need to edit the file
 * C:/Msys/1.0/etc/profile. Add this line:
 *
 * @code
export LD_LIBRARY_PATH="/usr/local/lib"
 * @endcode
 *
 * just before the last line (which should be cd "$HOME").
 *
 *
 * Finally, to check whether Msys and MinGW are installed correctly:
 *
 *
 * W2K: go to the desktop, double click on 'MSYS'. The MINGW32
 * terminal (or shell) will pop up, and you will see something like
 * this
 *
 * @code
Wilbert@WD ~
$ 
 * @endcode
 *
 * Try the following (the commands are
 * <a href="http://www.oreillynet.com/linux/cmd/">linux commands</a>):
 *
 * @code
Wilbert@WD ~
$ g++
 * @endcode
 *
 * if you installed everything correctly, you should get
 *
 * @code
$ g++
g++.exe: no input files
 * @endcode
 *
 * @section build_mingw_install_stlport Installation of STLport 5.0.2
 *
 * Install STLport in the @em /usr/local directory. OPen a MSYS terminal:
 *
 * @code
cd /c/tmp
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
cp lib/libstlport* /usr/local/lib/
cp -R stlport /usr/local/include/
 * @endcode
 *
 * @note
 * STLport will be automatically found if installed in @em /usr/local.
 *
 * @section build_mingw_install_boost Installation of Boost 1.33.1
 *
 * Install Boost in the @em /usr/local directory:
 *
 * @code
cd ..
tar jxvf boost_1_33_1.tar.bz2
unzip boost-jam-3.1.12-1-ntx86.zip
cd boost_1_33_1
../boost-jam-3.1.12-1-ntx86/bjam "-sTOOLS=mingw" --prefix=/usr/local --with-thread install
 * @endcode
 *
 * @section build_mingw_install_freetype Installation of Freetype 2.2.1
 *
 * Avisynth 3.0 displays text with a very efficient font engine, named
 * Freetype. Install it in the @em /usr/local directory:
 *
 * @code
cd ..
tar jxvf freetype-2.2.1.tar.bz2
cd freetype-2.2.1
./configure --prefix=/usr/local --enable-shared
make
make install
 * @endcode
 *
 * @section build_mingw_install_fontconfig Installation of Fonconfig 2.3.2
 *
 * Install Fontconfig in the @em /usr/local directory:
 *
 * @code
cd ..
tar jxvf fontconfig-2.3.2.tar.bz2
cd fontconfig-2.3.2
./configure --prefix=/usr/local
make
make install
 * @endcode
 *
 * @section build_mingw_install_nasm Installation of Nasm
 *
 * Install nasm.exe in the @em /usr/local/bin directory:
 *
 * @code
cd ..
unzip nasm-0.98.39-win32.zip
cd nasm-0.98.39 
cp nasmw.exe /usr/local/bin/nasm.exe
 * @endcode
 *
 * @section build_mingw_install_avisynth Installation of Avisynth 3.0
 *
 * Now, we may build Avisynth 3.0 ! Just after having installed 3 more
 * libraries.
 *
 * Ok, ok, I'm kidding...
 *
 * First we have to download the source code which is in cvs:
 *
 * @code
cd ..
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
cp -R circular_buffer* boost /usr/local/include/boost-1_33_1/
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
