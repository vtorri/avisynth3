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
using namespace std;

#pragma warning( disable : 4290 )

//class used as a name/type for properties
class PropertyKey {

public:
  PropertyKey() { }
  virtual ~PropertyKey() { }

  virtual void AddRef() const = 0;
  virtual void Release() const = 0;  

  virtual bool equals(const PropertyKey * other) const = 0;

  bool operator==(const PropertyKey& other) const { return equals(&other); }
};


typedef smart_ptr_to_cst<PropertyKey> PPropertyKey;

//keys used by the internals
//always defined static, so no need to refcount
//needs to find some way to correctly unwrap them from COM
class InternalPropertyKey : public PropertyKey {

public:
  InternalPropertyKey() { }

  virtual void AddRef() const { }
  virtual void Release() const { }

  virtual bool equals(const PropertyKey * other) const { return other == this; }

};

class PropertySet;

//exception class
class ConstraintViolation { };

class Property : public RefCounted {

protected:
  //check that self goes along well with the other properties of the set
  virtual void IntegrityCheck(const PropertySet& set) const throw(ConstraintViolation) { } 

  friend class PropertySet; //so it can call the above

public:
  Property() { }

  //protected properties are not supposed to be altered directly
  //but through special methods
  virtual bool IsProtected() const = 0;

  virtual PPropertyKey GetKey() const  = 0;

};

class ProtectedProperty : public Property {

public:
  ProtectedProperty() { }

  virtual bool IsProtected() const { return true; }
};




typedef smart_ptr_to_cst<Property> CPProperty;  //smart const Property *
typedef smart_ptr<Property> PProperty;
typedef smart_ptr_to_cst<PropertySet> CPPropertySet;
typedef smart_ptr<PropertySet> PPropertySet;

class PropertySet : public RefCounted {

  struct Entry {
    
    PPropertyKey key;
    CPProperty value;

    Entry(CPProperty _value) : key(_value->GetKey()), value(_value) { }
    Entry(const Entry& other) : key(other.key), value(other.value) { }

    bool operator== (const Entry& other) const { return key == other.key; }
    bool operator== (PPropertyKey _key) const { return key == _key; }
  };

  typedef vector<Entry> PropertyVector;

  PropertyVector propVector;

protected:
  //ask each property to check integrity
  void IntegrityCheck() const;

public:
  PropertySet() { }
  PropertySet(const PropertySet& other) : propVector(other.propVector) { }

  virtual RefCounted * clone() const { return new PropertySet(*this); }

  //mutations methods
  void Set(CPProperty prop);
  void Remove(PPropertyKey key);

  //same as above but with no effect on protected properties
  //throw an exception ?
  void ProtectedSet(CPProperty prop);
  void ProtectedRemove(PPropertyKey key);

  CPProperty Get(PPropertyKey key) const;
};












#endif //#define __PROPERTY_H__