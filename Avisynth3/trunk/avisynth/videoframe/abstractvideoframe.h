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


#ifndef __AVS_ABSTRACTVIDEOFRAME_H__
#define __AVS_ABSTRACTVIDEOFRAME_H__


//avisynth includes
#include "../dimension.h"
#include "../videoframe.h"

//stl include
#include <vector>


namespace avs {


/////////////////////////////////////////////////////////////////////////////////////////
//  AbstractVideoFrame
//
//  
//
class AbstractVideoFrame : public VideoFrame
{

private:  //typedef

  typedef std::vector<CPProperty> PropertyVector;


private:  //members

  Dimension dim_;
  FrameType type_;

  PropertyVector props;


public: //constructors

  //normal constructor
  AbstractVideoFrame(ColorSpace& space, const Dimension& dim, FrameType type);

  //conversion constructor
  AbstractVideoFrame(ColorSpace& space, const AbstractVideoFrame& other);

  //copy constructor
  AbstractVideoFrame(const AbstractVideoFrame& other);


public:  //general frame info

  virtual const Dimension& GetDimension() const { return dim_; }

  virtual FrameType GetFrameType() const { return type_; }



public:  //properties methods

  virtual void SetProperty(CPProperty prop);
  virtual void RemoveProperty(const PropertyKey& key);

  virtual boost::shared_ptr<const Property> GetProperty(const PropertyKey& key);


protected:  //helper method

  void ThrowInvalidPlaneException(ColorSpace& space, Plane plane);

};






}; //namespace avs

#endif //__AVS_ABSTRACTVIDEOFRAME_H__