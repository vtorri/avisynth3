// Avisynth v3.0 alpha.  Copyright 2003 Ben Rudiak-Gould et al.
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


#include "videoinfo.h"


const int ColorSpace::flagList[] = { 
  1<<ColorSpace::BGR | 1<<ColorSpace::INTERLEAVED,
  1<<ColorSpace::BGR | 1<<ColorSpace::INTERLEAVED,
  1<<ColorSpace::YUV | 1<<ColorSpace::INTERLEAVED,
  1<<ColorSpace::YUV | 1<<ColorSpace::PLANAR,
  1<<ColorSpace::YUV | 1<<ColorSpace::PLANAR
};


const ColorSpace VideoInfo::CS_BGR24 = I_BGR24;
const ColorSpace VideoInfo::CS_BGR32 = I_BGR32;
const ColorSpace VideoInfo::CS_YUY2  = I_YUY2;
const ColorSpace VideoInfo::CS_YV12  = I_YV12;
const ColorSpace VideoInfo::CS_I420  = I_I420;

