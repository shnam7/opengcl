# Microsoft Developer Studio Project File - Name="opengcl_static" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=opengcl_static - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "opengcl_static.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "opengcl_static.mak" CFG="opengcl_static - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "opengcl_static - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "opengcl_static - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "opengcl_static - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "out/Release"
# PROP Intermediate_Dir "out/Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c /Tp
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"out\Release/opengcls.lib"

!ELSEIF  "$(CFG)" == "opengcl_static - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "opengcl_static___Win32_Debug"
# PROP BASE Intermediate_Dir "opengcl_static___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "out/Debug"
# PROP Intermediate_Dir "out/Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c /Tp
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"out\Debug/opengclsd.lib"

!ENDIF 

# Begin Target

# Name "opengcl_static - Win32 Release"
# Name "opengcl_static - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\gmbox.c
# End Source File
# Begin Source File

SOURCE=..\src\gmmap.cpp
# End Source File
# Begin Source File

SOURCE=..\src\gque.c
# End Source File
# Begin Source File

SOURCE=..\src\gsocket.c
# End Source File
# Begin Source File

SOURCE=..\src\gsocket_x.cpp
# End Source File
# Begin Source File

SOURCE=..\src\gthread_x.cpp
# End Source File
# Begin Source File

SOURCE=..\src\gtime.c
# End Source File
# Begin Source File

SOURCE=..\src\gtok.c
# End Source File
# Begin Source File

SOURCE=..\src\gutils.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\gmbox.h
# End Source File
# Begin Source File

SOURCE=..\include\gmmap.h
# End Source File
# Begin Source File

SOURCE=..\include\gque.h
# End Source File
# Begin Source File

SOURCE=..\include\gsocket.h
# End Source File
# Begin Source File

SOURCE=..\include\gthread.h
# End Source File
# Begin Source File

SOURCE=..\include\gtime.h
# End Source File
# Begin Source File

SOURCE=..\include\gtok.h
# End Source File
# Begin Source File

SOURCE=..\include\gutils.h
# End Source File
# End Group
# End Target
# End Project
