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


#ifndef __PROPERTY_H__
#define __PROPERTY_H__

#include "refcounted.h"
#include <vector>


#pragma warning( disable : 4290 )

//class used as a name/type for properties
class PropertyKey {

public:
  PropertyKey() { }
  virtual ~PropertyKey() { }

  virtual bool operator==(const PropertyKey& other) const { return &other == this; }
};



class Property : public RefCounted {

public:
  Property() { }

  virtual const PropertyKey& GetKey() const  = 0;
};

typedef smart_ptr<Property> PProperty;
typedef smart_ptr_to_cst<Property> CPProperty;  




class PropertySet : private std::vector<CPProperty> {  

public:
  PropertySet() { }
  PropertySet(const PropertySet& other) : vector<CPProperty>(other) { }

  //mutations methods
  void Set(const Property& prop)
  { 
    const PropertyKey& key = prop.GetKey();
    for( iterator it = begin(); it != end(); ++it )    
      if ( (*it)->GetKey() == key )
      {
        *it = prop;
        return;
      }
    push_back(prop);  //case where we reach end without finiding it
  }

  void Remove(const PropertyKey& key)
  { 
    for(iterator it = begin(); it != end(); ++it )
      if ( (*it)->GetKey() == key )
      {
        erase(it);
        return;
      }
  }

  CPProperty Get(const PropertyKey& key) const
  {
    for(const_iterator it = begin(); it != end(); ++it )
      if ( (*it)->GetKey() == key )
        return *it;
    return CPProperty();  //case when key not found
  }
};











#endif //#define __PROPERTY_H__