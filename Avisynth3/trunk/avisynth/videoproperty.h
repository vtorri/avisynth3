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


#ifndef __VIDEOPROPERTY_H__
#define __VIDEOPROPERTY_H__

#include "property.h"
#include "colorspace.h"


class ColorSpaceProperty : public ProtectedProperty {

  //private to force use of the GetColorSpaceProperty method
  ColorSpaceProperty(const ColorSpace& _space) : space(_space) { }

protected:
  //in this case, check that height and width property are defined
  //and fit in with colorspace constraints
  virtual void IntegrityCheck(const PropertySet& set) const throw(ConstraintViolation);

public:
  const ColorSpace& space;

  //the key associated at this property
  static const InternalPropertyKey KEY;

  virtual PPropertyKey GetKey() const { return &KEY; }
  
  static CPProperty GetInstance(const ColorSpace& space);
};

typedef smart_ptr_to_cst<ColorSpaceProperty> CPColorSpaceProperty;



class DimensionProperty : public ProtectedProperty {

protected:
  //don't test the set, just ensure values are > 0  
  virtual void IntegrityCheck(const PropertySet& set) const throw(ConstraintViolation);

public:
  DimensionProperty(int _width, int _height) : width(_width), height(_height) { }

  const int width, height;

  static const InternalPropertyKey KEY;

  virtual PPropertyKey GetKey() const { return &KEY; }
};


class FieldOrFrameClipProperty : public ProtectedProperty {

protected:  
  virtual void IntegrityCheck(const PropertySet& set) const throw(ConstraintViolation);

public:
  FieldOrFrameClipProperty(bool _IsFrameClip) : IsFrameClip(_IsFrameClip) { }

  const bool IsFrameClip;

  static const InternalPropertyKey KEY;

  virtual PPropertyKey GetKey() const { return &KEY; }
};

class FrameTypeProperty : public ProtectedProperty {

protected:
  //just check that a ColorSpaceProperty is defined
  virtual void IntegrityCheck(const PropertySet& set) const throw(ConstraintViolation);

public:
  enum FrameType {  //don't change order
    FIELD_TOP,
    FIELD_BOTTOM,
    PROGRESSIVE,
    UNKNOWN,
    INTERLACED_TFF,
    INTERLACED_BFF
  };

  FrameType type;

  //constructors
  FrameTypeProperty(FrameType _type) : type(_type) { }
  FrameTypeProperty(const FrameTypeProperty& other) : type(other.type) { }
  FrameTypeProperty(const ColorSpace& space, bool IsField);


  bool IsField() const { return type <= FIELD_BOTTOM; }
  bool IsFrame() const { return type >= PROGRESSIVE; }

  bool IsInterlaced() const { return type >= INTERLACED_TFF; }
  bool MayBeInterlaced() const { return type >= UNKNOWN; }


  virtual RefCounted * clone() const throw() { return new FrameTypeProperty(*this); }

  static const InternalPropertyKey KEY;

  virtual PPropertyKey GetKey() const { return &KEY; }
};




#endif   // __VIDEOPROPERTY_H__