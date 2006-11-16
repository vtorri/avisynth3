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


#ifndef __AVS_VIDEOFRAME_CONCRETE_BASE_H__
#define __AVS_VIDEOFRAME_BASE_H__

//avisynth includes
#include "../../key.h"
#include "../../videoframe.h"
#include "../../geometry/dimension.h"

//stl include
#include <hash_map>  //for hash_map


namespace avs { namespace videoframe { namespace concrete {



/////////////////////////////////////////////////////////////////////////////////////////
//  Base
//
//  provides common features for all VideoFrame subclasses
//
class AVS_NOVTABLE Base : public virtual VideoFrame
{

private:  //typedefs

  typedef std::hash_map<PKey, CPProperty, Key::Hasher, Key::Comparator> PropertyMap;
  typedef std::hash_multimap<PKey, CPProperty, Key::Hasher, Key::Comparator> PropertyMultiMap;


private:  //members

  Dimension dim_;                //dimension of the frame
  FrameType type_;               //type of the frame

  PropertyMultiMap flows_;       //holds flow properties
  mutable PropertyMap statics_;  //holds static properties


public: //structors

  //normal constructor
  Base(PColorSpace const& space, Dimension const& dim, FrameType type);

  //generated copy constructor and destructor are fine


public:  //general frame info

  //queries
  virtual FrameType GetType() const { return type_; }
  virtual Dimension const& GetDimension() const { return dim_; }

  //write access
  virtual void SetType(FrameType type);


protected:  //write access

  void SetDimension(Dimension const& dim) { dim_ = dim; }
  void ProtectedSetType(FrameType type) { type_ = type; }


public:  //properties methods

  virtual void SetProperty(CPProperty const& prop);
  virtual void SetStaticProperty(CPProperty const& prop) const;

  virtual CPProperty GetStaticProperty(PKey const& key) const;
  virtual void GetFlowProperties(PKey const& key, PropertyVector& propVector) const;

  virtual void RemoveFlowProperties(PKey const& key);


protected:  //helper method

  void ClearStaticProperties() { statics_.clear(); }

};




} } } //namespace avs::videoframe::concrete

#endif //__AVS_VIDEOFRAME_CONCRETE_BASE_H__
