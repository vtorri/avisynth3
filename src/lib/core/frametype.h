// Avisynth v3.0 alpha.  Copyright 2003 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_FRAMETYPE_H__
#define __AVS_FRAMETYPE_H__


namespace avs {



/////////////////////////////////////////////////////////////////////////////////
//  FrameType
//
//  enum of the various types a frame may have
//
enum FrameType
{
  FIELD_TOP,
  FIELD_BOTTOM,
  PROGRESSIVE,
  INTERLACED_TFF,
  INTERLACED_BFF,
  UNKNOWN  
};


} //namespace avs

#endif //__AVS_FRAMETYPE_H__
