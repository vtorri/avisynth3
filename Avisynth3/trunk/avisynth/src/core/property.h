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


#ifndef __AVS_PROPERTY_H__
#define __AVS_PROPERTY_H__


//boost include
#include <boost/shared_ptr.hpp>


namespace avs {


//class declaration
class Property;

//typedef
typedef boost::shared_ptr<Property const> CPProperty;



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
  class Key { };


public:  //GetKey method

  virtual Key & GetKey() const  = 0;


};//Property









} //namespace avs

#endif //#define __AVS_PROPERTY_H__