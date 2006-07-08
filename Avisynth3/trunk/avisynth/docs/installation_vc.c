/**
 * @page build_windows_vc Building Avisynth 3.0 for Windows using VC++ .NET 2003 (VC 7.1)
 *
 * @section summary Summary
 *
 * @li @ref build_vc_presentation
 * @li @ref build_vc_download_dependencies
 * @li @ref build_vc_install_stlport
 * @li @ref build_vc_install_boost
 * @li @ref build_vc_install_freetype
 * @li @ref build_vc_install_fontconfig
 * @li @ref build_vc_install_nasm
 * @li @ref build_vc_install_avisynth
 * @li @ref build_vc_install_remarks
 *
 * @section build_vc_presentation Presentation
 *
 * To successfully compile AviSynth v3 with Microsoft Visual C++ .NET
 * 2003, you first need to obtain some additional libraries and
 * tools. The order in which you compile them is important, so please
 * follow this small guide. The dependencies are the following:
 *
 * @li STLport (another port of the STL (Standard Template Library))
 * @li Boost (set of libraries, written in C++, used for the parser,
 * the threads, the pointers, etc...)
 * @li Freetype (a high quality font engine)
 * @li Fontconfig (library for font customization and configuration)
 * @li Nasm (the Netwide Assembler)
 *
 * I used a folder C:/tmp/ to put everything in. If you want to put
 * the stuff somewhere else, don't forget to change the paths
 * accordingly.
 *
 * @section build_vc_download_dependencies Download dependencies
 *
 * First, download the following libraries in the folder @em C:/tmp.
 * @li <a href="http://prdownloads.sourceforge.net/stlport/STLport-5.0.2.tar.bz2?download">STLport 5.0.2</a>
 * @li <a href="http://prdownloads.sourceforge.net/boost/boost_1_33_1.zip?download">Boost 1.33.1</a>
 * @li <a href="http://prdownloads.sourceforge.net/boost/boost-jam-3.1.12-1-ntx86.zip?download">bjam (make-like utility)</a>
 * @li <a href="http://prdownloads.sourceforge.net/freetype/freetype-2.2.1.tar.bz2?download">Freetype 2.2.1</a>
 * @li <a href="http://fontconfig.org/release/fontconfig-2.3.2.tar.gz">Fontconfig 2.3.2</a>
 * @li <a href="http://prdownloads.sourceforge.net/nasm/nasm-0.98.39-win32.zip?download">nasm 0.98.39</a>
 *
 * @section build_vc_install_stlport Installation of STLport 5.0.2
 *
 * Unpack the archive @em STLport-5.0.2.tar.bz2 to @em C:/tmp (you can
 * unpack tar.bz2 files with winzip).
 *
 * Open up a command prompt and give the following commands (assuming
 * everything ended up in a folder C:/tmp/STLPORT/): 
 *
 * @code
C:/tmp/STLport> "c:/Program files/Microsoft Visual Studio .NET 2003/Common7/Tools/vsvars32.bat"
C:/tmp/STLport> cd build/lib
C:/tmp/STLport> nmake -f nmake-vc71.mak clean all
C:/tmp/STLport> cd ../../lib
C:/tmp/STLport> copy STLPORT_VC7146.dll %WinDir%/System32/
 * @endcode
 *
 * If you installed Visual Studio in a different location, don't forget
 * to adjust the path to vsvars32.bat. Don't forget the double quotes!
 *
 * @li In the top menu, go to Tools, Options.
 * @li In the Options box, select the Projects, VC++ Directories in
 * the tree on the left.
 * @li In the "Show directories for:" pull-down menu, choose "Include
 * files".
 * @li Click the "new Line" button (with the yellow folder on it).
 * @li Browse to C:/tmp/STLport/stlport/.
 * @li Click the "open" button.
 * @li If the new line is not at the top of the list, use the arrow
 * buttons on the right of the box to put the STLport path there.
 * @li In the "Show directories for:" pull-down menu, choose "Library
 * files".
 * @li Click the "new Line" button (with the yellow folder on it).
 * @li Browse to C:/tmp/STLport/lib/.
 * @li Click the "open" button.
 *
 * Close the dialog boxes and close VC.net.
 *
 * @section build_vc_install_boost Installation of Boost 1.33.1
 *
 * Unzip the file @em boost_1_33_1.zip located in @em C:/tmp/ in this
 * folder and, in the archive boost-jam-3.1.12-1-ntx86.zip, you will
 * find @em bjam.exe. Put this file in @em C:/tmp/boost_1_33_1/. To
 * compile boost, open up a command prompt and give the following
 * commands (assuming everything ended up in a folder
 * C:/tmp/boost_1_33_1/): 
 *
 * @code
C:/tmp/BOOST_1_33_1>"c:/Program files/Microsoft Visual Studio .NET 2003/Common7/Tools/vsvars32.bat" 
C:/tmp/BOOST_1_33_1>bjam "-sTOOLS=vc7.1-stlport" "-sSTLPORT_PATH=C:/tmp" --with-thread stage
 * @endcode
 *
 * You will see some errors about boost.python and file not found
 * error's. These can safely be ignored. The only important thing is
 * that when bjam is finished,
 * C:/tmp/boost_1_33_1/stage/lib/boost_thread-vc71-mt-p-1_33.lib was
 * created. Make sure that you copy this dll to your Windows system
 * folder.
 *
 * Next, add a new include path and library path, just as in
 * @ref build_vc_install_stlport :
 *
 * @li Add C:/tmp/boost_1_33_1/stage/lib/ to the Library paths.
 * @li Add C:/tmp/boost_1_33_1/boost/ to the Include path.
 *
 * @section build_vc_install_freetype Installation of Freetype 2.2.1
 *
 * Not done yet.
 *
 * @section build_vc_install_fontconfig Installation of Fonconfig 2.3.2
 *
 * Not done yet.
 *
 * @section build_vc_install_nasm Installation of Nasm
 *
 * unzip the file @em nasm/nasm-0.98.39-win32.zip located in
 * @em C:/tmp. Rename @em nasmw.exe to @em nasm.exe.
 *
 * Start up VC.net and
 *
 * @li In the top menu, go to Tools, Options.
 * @li In the Options box, select the Projects, VC++ Directories in
 * the tree on the left.
 * @li In the "Show directories for:" pull-down menu, choose
 * "Executable files".
 * @li Click the "new Line" button (with the yellow folder on it).
 * @li Browse to the folder where you unpacked nasm.
 * @li Click the open button. The path is now added to the list.
 *
 * Close the dialog boxes and close VC.net.
 *
 * @section build_vc_install_avisynth Installation of Avisynth 3.0
 *
 * Right now, the source code of Avisynth 3.0 is only available on
 * cvs. It is assumed you have installed WinCVS or TortoiseCVS,
 * otherwise you have to do that first. You can use either the gui to
 * get the avisynth_3_0 branch, or the command prompt (for W2K for
 * example: go to the desktop, right click on 'My Computer' ->
 * Properties -> Advanced tab -> Environment Variables -> select the
 * Variable 'path' -> edit -> add the path to WinCVS/TortoiseCVS where
 * cvs.exe is located) to get it:
 *
 * Make an empty folder called: C:/tmp (or whatever). Open a command
 * prompt and use the following commands:
 *
 * @code
cd C:/tmp
C:/tmp> cvs -d:pserver:anonymous@cvs.sourceforge.net:/cvsroot/avisynth2 co -r avisynth_3_0 avisynth
 * @endcode
 *
 * This will get you the AviSynth v3 branch (note "co" means "check
 * out", and "-r" will get you the branch).
 *
 * Now you can compile Avisynth 3.0:
 *
 * @li In VC.net, go to file, open solution.
 * @li Open C:/tmp/avs_v3/avisynth/build/win32/vc71/avisynth.sln.
 * @li In the menu, set the solution configuration pull-down to
 * "Release".
 * @li In the menu, select "Buid", "Build Solution".
 *
 * You now should have an avisynth.dll in
 * @em C:/tmp/avs_v3/avisynth/build/win32/vc71/Release/
 *
 * @section build_vc_install_remarks Additional remarks
 *
 * @li The avisynth.dll file will be quite big. It can be compressed using
 * <a href="http://upx.sourceforge.net/">UPX</a>:
 * @code
upx -9 -k avisynth.dll
 * @endcode
 * You may need to use the -f switch to force it to pack.
 *
 * @li The receipt above also works with the standard edition of VC++ .NET
 * 2003, which is also included with the learning edition. However,
 * this version of VC contains the non-optimizing compiler. As a
 * result, Avisynth will be a bit slower. There is a way around this:
 * the <a href="http://msdn.microsoft.com/visualc/vctoolkit2003/">Microsoft
 * Visual C++ Toolkit 2003</a>, a free download, contains the
 * optimizing version. Install it, and go to:
 * @code
C:/Program files/Microsoft Visual C++ Toolkit 2003/bin/
 * @endcode
 * Copy the files c2.dll and cl.exe (do make backups of the originals first!!!) to:
 * @code
C:/Program Files/Microsoft Visual Studio .NET 2003/Vc7/bin/
 * @endcode
 * The optimization options in the gui will still be greyed
 * out. However, optimization switches already in a project file will
 * be passed on to the compiler. And programs compiled on the command
 * prompt will use it directly. 
 *
 */
