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


#ifndef __AVS_VIDEOFRAME_BASE_H__
#define __AVS_VIDEOFRAME_BASE_H__


//avisynth includes
#include "../dimension.h"
#include "../videoframe.h"

//stl include
#include <vector>


namespace avs {


/////////////////////////////////////////////////////////////////////////////////////////
//  VideoFrame::Base
//
//  
//
class VideoFrame::Base : public VideoFrame
{

private:  //typedef

  typedef std::vector<CPProperty> PropertyVector;


private:  //members

  Dimension dim_;
  Type type_;

  PropertyVector propVector;


public: //constructors

  //normal constructor
  Base(ColorSpace& space, Dimension const& dim, Type type);

  //colorspace conversion constructor
  Base(ColorSpace& space, Base const& other);

  //copy constructor
  Base(Base const& other);


public:  //general frame info

  virtual Dimension const& GetDimension() const { return dim_; }

  virtual Type GetType() const { return type_; }

  virtual void SetType(Type type);


public:  //properties methods

  virtual void SetProperty(CPProperty prop);
  virtual void RemoveProperty(Property::Key const& key);

  virtual CPProperty GetProperty(Property::Key const& key) const;


protected:  //helper methods

  static Dimension ToPlaneDimension(ColorSpace& space, Dimension const& dim, Plane plane);

};






} //namespace avs

#endif //__AVS_VIDEOFRAME_BASE_H__