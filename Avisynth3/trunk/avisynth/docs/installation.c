/**
 * @page doc_installation AviSynth 3.0 Build Documentation
 * @version 0.3.0
 * @author Wilbert Dijkhof < wilbertdijkhof at yahoo dot com >
 * @author Vincent Torri < vtorri at univ-evry dot fr >
 * @date 2006
 *
 * @section summary Summary
 *
 * @li @ref build_intro
 * @li @subpage build_windows_mingw
 * @li @subpage build_windows_vc
 * @li @subpage build_linux
 *
 * @section build_intro Introduction
 *
 * Avisynth 3.0 can be built from sources on Windows and on several
 * Unix systems. The development being done on Windows using  VC++
 * .NET 2003 (VC 7.1) and Linux x86 using g++, it is known to compile
 * on this platforms. Nevertheless, some tests have been made on
 * Windows with Msys/MinGW (that is, with g++ on Windows) and Linux
 * amd64.
 *
 * On Windows as well as on Linux/Unix, if you compile Avisynth from
 * sources, you will need some dependencies. They are described in
 * each of the sections. Nevertheless, if your system has already some
 * (all) of these depdendencies, it's useless to install them.
 *
 * In any case, you'll have to have some good knowledge in order to
 * compile Avisynth from source.
 */





/**
The Windows version of AviSynth v3 can be built using MinGW (g++) or
VC++ .NET 2003 (VC 7.1). The Linux version requires g++. The plan of
this installation guide is the following.

\ref AVS-Win-MinGW

\ref AVS-Win-net


@section AVS-Win-net 2. Building AviSynth v3 for Windows using VC++ .NET 2003 (VC 7.1)

To successfully compile AviSynth v3 with Microsoft Visual C++ .NET 2003, you first need to obtain some additional libraries and tools. The order in which you compile them is important, so please follow this small guide.

I used a folder C:/dev/ to put everything in. If you want to put the stuff somewhere else, don't forget to change the paths accordingly.


@subsection inst_avs 2.1 Obtaining the AviSynth v3 source

Get the sources from their <a href=http://sourceforge.net/projects/avisynth2/>website</a>.

It is assumed you have installed WinCVS or TortoiseCVS, otherwise you have to do that first. You can use either the gui to get the avisynth_3_0 branch, or the command prompt (for W2K for example: go to the desktop, right click on 'My Computer' -> Properties -> Advanced tab -> Environment Variables -> select the Variable 'path' -> edit -> add the path to WinCVS/TortoiseCVS where cvs.exe is located) to get it:

Make an empty folder called: C:/dev (or whatever). Open a command prompt and use the following commands

@code
cd C:/dev
C:/dev> cvs -d:pserver:anonymous@cvs.sourceforge.net:/cvsroot/avisynth2 co -r avisynth_3_0 avisynth
@endcode

This will get you the AviSynth v3 branch (note "co" means "check out", and "-r" will get you the branch).


@subsection net_nasm 2.2 Obtaining and installing NASM assembler

Get <a href=http://prdownloads.sourceforge.net/nasm/nasm-0.98.36-win32.zip?download>nasm version 0.98.36</a>. This is not the latest version, but 0.98.38 has been known to cause problems. Unzip it. Rename nasmw.exe to nasm.exe. Next, set the path to nasm.exe in VC:

Start up VC.net and

- In the top menu, go to Tools, Options.
- In the Options box, select the Projects, VC++ Directories in the tree on the left.
- In the "Show directories for:" pull-down menu, choose "Executable files".
- Click the "new Line" button (with the yellow folder on it).
- Browse to the folder where you unpacked nasm.
- Click the open button. The path is now added to the list.

Close the dialog boxes and close VC.net.


@subsection net_stl 2.3 Obtaining and compiling the STLport library

Get <a href=http://prdownloads.sourceforge.net/stlport/STLport-5.0.0.tar.gz?download>STLport-5.0.0</a>, and unpack the archive to C:/dev (you can unpack tar.gz files with winzip).

Open up a command prompt and give the following commands (assuming everything ended up in a folder C:/dev/STLPORT/):

@code
C:/dev/STLport> "c:/Program files/Microsoft Visual Studio .NET 2003/Common7/Tools/vsvars32.bat"
C:/dev/STLport> cd build/lib
C:/dev/STLport> nmake -f nmake-vc71.mak clean all
C:/dev/STLport> cd ../../lib
C:/dev/STLport> copy STLPORT_VC7146.dll %WinDir%/System32/
@endcode

If you installed Visual Studio in a different location, don't forget to adjust the path to vsvars32.bat. Don't forget the double quotes!

- In the top menu, go to Tools, Options.
- In the Options box, select the Projects, VC++ Directories in the tree on the left.
- In the "Show directories for:" pull-down menu, choose "Include files".
- Click the "new Line" button (with the yellow folder on it).
- Browse to C:/dev/STLport/stlport/.
- Click the "open" button.
- If the new line is not at the top of the list, use the arrow buttons on the right of the box to put the STL path there.
- In the "Show directories for:" pull-down menu, choose "Library files".
- Click the "new Line" button (with the yellow folder on it).
- Browse to C:/dev/STLport/lib/.
- Click the "open" button.

Close the dialog boxes and close VC.net.


@subsection net_boost 2.4 Obtaining and compiling the Boost library

Get <a href=http://prdownloads.sourceforge.net/boost/boost_1_33_0.zip?download>boost_1_33_0.zip</a> and unzip the archive to c:/dev.

You now should have a folder C:/dev/boost_1_33_0/

You also need an add-on to boost: circular_buffer. This is included with the AviSynth source and can be found here:
C:/dev/avs_v3/avisynth/build/circular_buffer_v3.6.zip

- Unpack the C:/dev/avs_v3/avisynth/build/circular_buffer_v3.6.zip to C:/dev/.
- Copy the contents of C:/dev/circular_buffer/boost/ to C:/dev/boost_1_33_0/boost/.
- Copy the contents of C:/dev/circular_buffer/libs/ to C:/dev/boost_1_33_0/libs/.

To compile boost, you need bjam, a "make" replacement. Get it <a href=http://prdownloads.sourceforge.net/boost/boost-jam-3.1.10-1-ntx86.zip>here</a>:

In it you will find bjam.exe. Copy bjam.exe to C:/dev/boost_1_33_0/

Next, compile Boost. Open up a command prompt and give the following commands (assuming everything ended up in a folder C:/dev/boost_1_33_0/):

@code
C:/dev/BOOST_1_33_0>"c:/Program files/Microsoft Visual Studio .NET 2003/Common7/Tools/vsvars32.bat" 
C:/dev/BOOST_1_33_0>bjam "-sTOOLS=vc7.1-stlport" "-sSTLPORT_PATH=C:/dev" stage
@endcode

You will see some errors about boost.python and file not found error's. These can safely be ignored. The only important thing is that when bjam is finished,
C:/dev/boost_1_33_0/stage/lib/boost_thread-vc71-mt-p-1_33.lib was created. Make sure that you copy this dll to your Windows system folder.

Next, add a new include path and library path, just as in \ref net_stl:

- Add C:/dev/boost_1_33_0/stage/lib/ to the Library paths.
- Add C:/dev/boost_1_33_0/boost/ to the Include path.


@subsection net_free 2.5 Obtaining and compiling Freetype

(This step may become obsolete shortly.)

Get <a href=http://prdownloads.sourceforge.net/freetype/ft219.zip>freetype 2.1.9 (ft219.zip)</a> and unpack the zip to C:/dev.

Edit C:/dev/freetype-2.1.9/include/freetype/config/ftconfig.h, look up line 276 and change

@code
#define FT_EXPORT( x )  extern "C" x
@endcode
into
@code
#define FT_EXPORT( x )  extern "C" x __cdecl
@endcode

(If you use notepad, which does not have a line count, do a search on "#define FT_EXPORT( x )  extern "C"". It only appears once in this file.)

- In VC.net, go to file, open solution.
- Open C:/dev/freetype-2.1.9/builds/win32/visualc/freetype.sln.
- In the menu, set the solution configuration pull-down to "Release".
- In the menu, select "Build", "Build Solution".

You will get a bunch of warnings, but it should compile ok.

Next, add a new include path and library path, just as in \ref net_stl:

- Add C:/dev/freetype-2.1.9/objs/ to the Library paths.
- Add C:/dev/freetype-2.1.9/include/ to the Include path.
 

@subsection net_comp 2.6 Compiling AviSynth v3

- In VC.net, go to file, open solution.
- Open C:/dev/avs_v3/avisynth/build/win32/vc71/avisynth.sln.
- In the menu, set the solution configuration pull-down to "Release".
- In the menu, select "Buid", "Build Solution".

You now should have an avisynth.dll in C:/dev/avs_v3/avisynth/build/win32/vc71/Release/


@subsection net_add1 2.7 Additional remarks I - UPX

The avisynth.dll will be quite big. It can be compressed using <a href=http://upx.sourceforge.net/>UPX</a>:

@code
upx -9 -k avisynth.dll
@endcode

You may need to use the -f switch to force it to pack.


@subsection net_add2 2.8 Additional remarks II: Standard edition

The receipt above also works with the standard edition of VC++ .NET 2003, which is also included with the learning edition. However, this version of VC contains the non-optimizing compiler. As a result, AviSynth will be a bit slower.

There is a way around this: the <a href=http://msdn.microsoft.com/visualc/vctoolkit2003/>Microsoft Visual C++ Toolkit 2003</a>, a free download, contains the optimizing version. Install it, and go to:

C:/Program files/Microsoft Visual C++ Toolkit 2003/bin/

Copy the files c2.dll and cl.exe (do make backups of the originals first!!!) to:

C:/Program Files/Microsoft Visual Studio .NET 2003/Vc7/bin/

The optimization options in the gui will still be greyed out. However, optimization switches already in a project file will be passed on to the compiler. And programs compiled on the command prompt will use it directly.

*/
