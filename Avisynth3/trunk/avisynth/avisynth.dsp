# Microsoft Developer Studio Project File - Name="avisynth" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=avisynth - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "avisynth.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "avisynth.mak" CFG="avisynth - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "avisynth - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "avisynth - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "avisynth - Win32 Profile" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "avisynth - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AVISYNTH_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /Zd /Ot /Og /Ob2 /D "NDEBUG" /D "INC_OLE2" /D "STRICT" /D "WIN32" /D "_WIN32" /D "_MT" /D "_DLL" /D "_MBCS" /D "_USRDLL" /D "AVISYNTH_EXPORTS" /YX /FD /Gs /GF /c
# SUBTRACT CPP /Ox /Oa /Ow /Oi /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 msacm32.lib quartz.lib ddraw.lib amstrmid.lib vfw32.lib kernel32.lib advapi32.lib version.lib user32.lib gdi32.lib ole32.lib uuid.lib winmm.lib /nologo /dll /machine:I386
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build
InputPath=.\Release\avisynth.dll
SOURCE="$(InputPath)"

"$(SystemRoot)\system32\avisynth.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	release\upx -9 release\avisynth.dll 
	copy Release\avisynth.dll $(SystemRoot)\system32 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "avisynth - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AVISYNTH_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "DEBUG" /D "INC_OLE2" /D "STRICT" /D "WIN32" /D "_WIN32" /D "_MT" /D "_DLL" /D "_MBCS" /D "_USRDLL" /D "AVISYNTH_EXPORTS" /Fr /YX /FD /GZ /GF /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 msacm32.lib quartz.lib ddraw.lib amstrmid.lib vfw32.lib kernel32.lib advapi32.lib version.lib user32.lib gdi32.lib ole32.lib uuid.lib winmm.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /incremental:no /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Debug\avisynth.dll $(SystemRoot)\system32
# End Special Build Tool

!ELSEIF  "$(CFG)" == "avisynth - Win32 Profile"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "avisynth___Win32_Profile"
# PROP BASE Intermediate_Dir "avisynth___Win32_Profile"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Profile"
# PROP Intermediate_Dir "Profile"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /GX /O2 /Ob2 /I "c:\platsdk\include" /I "c:\platsdk\classes\base" /D "NDEBUG" /D "INC_OLE2" /D "STRICT" /D "WIN32" /D "_WIN32" /D "_MT" /D "_DLL" /D "_MBCS" /D "_USRDLL" /D "AVISYNTH_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GR /GX /Zi /O2 /Ob2 /D "NDEBUG" /D "INC_OLE2" /D "STRICT" /D "WIN32" /D "_WIN32" /D "_MT" /D "_DLL" /D "_MBCS" /D "_USRDLL" /D "AVISYNTH_EXPORTS" /FR /YX /FD /Gh /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 msvcrt.lib c:\p\mm\mpeg2dll\release\mpeg2dll.lib ddraw.lib amstrmid.lib vfw32.lib kernel32.lib advapi32.lib version.lib user32.lib gdi32.lib ole32.lib uuid.lib /nologo /dll /machine:I386
# SUBTRACT BASE LINK32 /pdb:none /debug /nodefaultlib
# ADD LINK32 penter.lib msvcrt.lib msacm32.lib quartz.lib ddraw.lib amstrmid.lib vfw32.lib kernel32.lib advapi32.lib version.lib user32.lib gdi32.lib ole32.lib uuid.lib winmm.lib /nologo /dll /debug /machine:I386 /nodefaultlib
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Desc=Copying to windows
PostBuild_Cmds=copy Profile\avisynth.dll $(SystemRoot)\system32
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "avisynth - Win32 Release"
# Name "avisynth - Win32 Debug"
# Name "avisynth - Win32 Profile"
# Begin Group "Avisynth Core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\avisynth.def
# End Source File
# Begin Source File

SOURCE=.\avisynth.rc
# End Source File
# Begin Source File

SOURCE=.\src\core\blitter.h
# End Source File
# Begin Source File

SOURCE=.\src\core\block.cpp
# End Source File
# Begin Source File

SOURCE=.\src\core\block.h
# End Source File
# Begin Source File

SOURCE=.\src\core\block_base.h
# End Source File
# Begin Source File

SOURCE=.\src\core\block_recycler.cpp
# End Source File
# Begin Source File

SOURCE=.\src\core\block_recycler.h
# End Source File
# Begin Source File

SOURCE=.\src\core\bufferwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\core\bufferwindow.h
# End Source File
# Begin Source File

SOURCE=.\src\core\cache.h
# End Source File
# Begin Source File

SOURCE=.\src\core\cache_concrete.cpp
# End Source File
# Begin Source File

SOURCE=.\src\core\cache_concrete.h
# End Source File
# Begin Source File

SOURCE=.\src\core\clip.h
# End Source File
# Begin Source File

SOURCE=.\src\core\colorspace.h
# End Source File
# Begin Source File

SOURCE=.\src\core\cow_shared_ptr.h
# End Source File
# Begin Source File

SOURCE=.\src\core\cpu.h
# End Source File
# Begin Source File

SOURCE=.\src\core\dimension.h
# End Source File
# Begin Source File

SOURCE=.\src\core\ownedblock.cpp
# End Source File
# Begin Source File

SOURCE=.\src\core\ownedblock.h
# End Source File
# Begin Source File

SOURCE=.\src\core\plane.h
# End Source File
# Begin Source File

SOURCE=.\src\core\property.h
# End Source File
# Begin Source File

SOURCE=.\src\core\runtime_environment.cpp
# End Source File
# Begin Source File

SOURCE=.\src\core\runtime_environment.h
# End Source File
# Begin Source File

SOURCE=.\src\core\runtime_environment_concrete.h
# End Source File
# Begin Source File

SOURCE=.\src\core\sampletype.h
# End Source File
# Begin Source File

SOURCE=.\src\core\timer.h
# End Source File
# Begin Source File

SOURCE=.\src\core\vecteur.h
# End Source File
# Begin Source File

SOURCE=.\src\core\videoinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\core\videoinfo.h
# End Source File
# End Group
# Begin Group "Filters"

# PROP Default_Filter ""
# Begin Group "Audio"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\filters\audio\killaudio.cpp
# End Source File
# Begin Source File

SOURCE=.\src\filters\audio\killaudio.h
# End Source File
# Begin Source File

SOURCE=.\src\filters\audio\killvideo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\filters\audio\killvideo.h
# End Source File
# End Group
# Begin Group "Hierarchy"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\filters\cachingclip.h
# End Source File
# Begin Source File

SOURCE=.\src\filters\childclip.h
# End Source File
# Begin Source File

SOURCE=.\src\filters\foldedclip.h
# End Source File
# Begin Source File

SOURCE=.\src\filters\foldedsimplifiable.h
# End Source File
# Begin Source File

SOURCE=.\src\filters\refactorable.h
# End Source File
# Begin Source File

SOURCE=.\src\filters\simplifiable.h
# End Source File
# End Group
# Begin Group "Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\filters\source\voidclip.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\distrib\AviSynth.ico
# End Source File
# Begin Source File

SOURCE=.\src\folder.h
# End Source File
# End Target
# End Project
