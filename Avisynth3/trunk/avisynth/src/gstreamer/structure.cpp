// Avisynth v3.0 alpha.  Copyright 2005-2006 David Pierre - Ben Rudiak-Gould et al.
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


//macro include
#include "../config.h"

#ifdef AVS_HAS_GSTREAMER_SOURCE

//avisynth includes
#include "structure.h"

//stlport include
#include <cassert>

//boost includes
#include <boost/rational.hpp>     //so Fraction typedef is defined

//gstreamer include
#include <gst/gstvalue.h>


namespace avs { namespace gstreamer {



int Structure::GetIntField(char const * name) const
{
  int result;

  bool success = gst_structure_get_int(const_cast<Structure *>(this), name, &result) != 0; 
  assert( success );

  return result;
}


bool Structure::GetBoolField(char const * name) const
{
  gboolean result;

  bool success = gst_structure_get_boolean(const_cast<Structure *>(this), name, &result) != 0;
  assert( success );

  return result == TRUE;
}


unsigned long Structure::GetFourCCField(char const * name) const
{
  guint32 result;

  bool success = gst_structure_get_fourcc(const_cast<Structure *>(this), name, &result) != 0;
  assert( success );

  return result;
}


Fraction Structure::GetFractionField(char const * name) const
{
  const GValue *val = gst_structure_get_value (const_cast<Structure *>(this), name);
  assert( val );

  return Fraction( gst_value_get_fraction_numerator (val), gst_value_get_fraction_denominator (val) );
}



} } //namespace avs::gstreamer

#endif //AVS_HAS_GSTREAMER_SOURCE
