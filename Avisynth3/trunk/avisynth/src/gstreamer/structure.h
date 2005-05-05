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


#ifndef __AVS_GSTREAMER_STRUCTURE_H__
#define __AVS_GSTREAMER_STRUCTURE_H__

//gstreamer includes
#include <gst/gststructure.h>


namespace avs { namespace gstreamer {



//////////////////////////////////////////////////////////////////////////////////////////
//  Structure
//
//  C++ friendly wrapper around gstreamer GstStructure struct
//
class Structure : public GstStructure
{

private:  //declared but not implemented

  Structure();
  ~Structure();


public:  //get field methods

  int GetIntField(char const * name) const;
  double GetDoubleField(char const * name) const;
  unsigned long GetFourCCField(char const * name) const;

};



} } //namespace avs::gstreamer

#endif //__AVS_GSTREAMER_STRUCTURE_H__
