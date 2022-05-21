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


#ifndef __AVS_FILTERS_RESIZE_SUBRANGE_H__
#define __AVS_FILTERS_RESIZE_SUBRANGE_H__


namespace avs { namespace filters { namespace resize {



////////////////////////////////////////////////////////////////////////////////////////////
//  SubRange
//
//  helper class for Resize
//   
//  The SubRange class carries info about the range of the source which should be resized
//
class SubRange
{

  double lower_, upper_;
  int whole_;


public:  //structors

  explicit SubRange(int whole)
    : lower_( 0.0 )
    , upper_( whole )
    , whole_( whole ) { }

  SubRange(double lower, double upper, int whole);

  //rescaling constructor
  SubRange(SubRange const& other, int whole);
 
  //generated copy constructor and destructor are fine


public:  //access

  double lower() const { return lower_; }
  double upper() const { return upper_; }
  double length() const { return upper_ - lower_; }
  int whole() const { return whole_; }

};



} } } //namespace avs::filters::resize

#endif //__AVS_FILTERS_RESIZE_SUBRANGE_H__
