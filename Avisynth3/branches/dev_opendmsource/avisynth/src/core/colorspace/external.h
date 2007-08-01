// Avisynth v3.0 alpha.  Copyright 2005 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_COLORSPACE_EXTERNAL_H__
#define __AVS_COLORSPACE_EXTERNAL_H__

//avisynth include
#include "../colorspace.h"


namespace avs { namespace colorspace {



/////////////////////////////////////////////////////////////////////////////////////////
//  colorspace::External
//
//  ColorSpace subinterface that external colorspaces MUST implement as well
//
class AVS_NOVTABLE External : public virtual ColorSpace
{


public:  //ColorSpace interface

  virtual ID id() const { return I_EXTERNAL; }


public:  //cspace::External interface

  //conversion methods
  virtual PClip ConvertTo(PClip const& clip, PColorSpace const& target) const = 0;
  virtual PClip ConvertFrom(PClip const& clip, PColorSpace const& source) const = 0;

};



} } //namespace avs::colorspace

#endif //__AVS_COLORSPACE_EXTERNAL_H__
