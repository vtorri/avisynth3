// Avisynth v3.0 alpha.  Copyright 2004 David Pierre - Ben Rudiak-Gould et al.
// http://www.avisynth.org

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA, or visit
// http://www.gnu.org/copyleft/gpl.html .
//
// Linking Avisynth statically or dynamically with other modules is making a
// combined work based on Avisynth.  Thus, the terms and conditions of the GNU
// General Public License cover the whole combination.


#ifndef __AVS_DEFINE_H__
#define __AVS_DEFINE_H__


# ifdef _MSC_VER
#   define AVS_NORETURN  __declspec(noreturn)
#   define AVS_NOVTABLE  __declspec(novtable)
#   define AVS_DLLEXPORT __declspec(dllexport)
#   define AVS_DLLIMPORT __declspec(dllimport)
#   define AVS_STDCALL   __stdcall
#   define AVS_CDECL     __cdecl
# else //other plaforms
#   define AVS_NOVTABLE
#   define AVS_NORETURN  __attribute__((noreturn))
    //FIXME: Only gcc 3.5/4.0 supports visibility - note for later
#   define AVS_DLLEXPORT
#   define AVS_DLLIMPORT
#   define AVS_STDCALL   __attribute((__stdcall__))
#   define AVS_CDECL     __attribute((__cdecl__))
# endif //end for platforms

#endif //__AVS_DEFINE_H__

