// Avisynth v3.0 alpha.  Copyright 2004 Ben Rudiak-Gould et al.
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


#ifndef __AVS_PROPERTY_H__
#define __AVS_PROPERTY_H__

//avisynth include
#include "forward.h"


namespace avs {



//////////////////////////////////////////////////////////////////////////////////
//  Property
//
//  base class for properties
//
class Property
{

public:  //structors

  Property() { }
  virtual ~Property() { }


public:  //clone method

  virtual CPProperty clone() const = 0;


public:  //Key inner class

  ///////////////////////////////////////////////////////////////////////////////
  //  Property::Key 
  //
  //  class used as a name/type for properties
  //
  class Key
  { 
  
  public:  //structors

    Key() { }
    virtual ~Key() { }

  public:  //Key interface

    //NB: == and hash must be consistent together (the defaults are)
    //ie  A == B  =>  A.hash() == B.hash()
    virtual bool operator==(Key const& other) const { return &other == this; }    
    virtual unsigned hash() const { return unsigned(this); }

  };//Property::Key

  typedef boost::shared_ptr<Key const> PKey;


public:  //Property interface

  virtual PKey GetKey() const  = 0;

  virtual bool IsStatic() const = 0;

};//Property





} //namespace avs

#endif //#define __AVS_PROPERTY_H__
