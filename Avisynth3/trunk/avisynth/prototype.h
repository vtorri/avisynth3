// Avisynth v3.0 alpha.  Copyright 2002 Ben Rudiak-Gould et al.
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


#ifndef __PROTOTYPE_H__
#define __PROTOTYPE_H__

//stl includes
#include <vector>
#include <string>
using namespace std;
//boost includes
#include <boost/any.hpp>   //which includes type_info

typedef boost::any AVSValue;
typedef vector<AVSValue> ArgVector;


/**********************************************************************************************/
/********************************* Arguments classes ******************************************/
/**********************************************************************************************/


//argument class used by the parser to describe what he has
//when linking functions
class LinkageArgument {

  const string name;
  const type_info& type;

public:
  LinkageArgument(const type_info& _type, const string& _name = "") : name(_name), type(_type) { }  
  //this constructor exist to simplify conversion of an ArgVector to a LinkagePrototype
  LinkageArgument(const AVSValue& value) : name(""), type(value.type()) { }

  const string& GetName() const { return name; }
  const type_info& Type() const { return type; }

  bool unNamed() const { return name.empty(); }    //the name sucks
  bool operator==(const string& _name) const { return name == _name; }
};



class DescriptionArgument : public LinkageArgument {

  const AVSValue defaut;

  static inline const string& CheckName(const string& name) { if (name.empty()) throw std::invalid_argument("the null chain is not a valid arg name"); }

public:
  DescriptionArgument(const string& name, const type_info& type) : LinkageArgument(type, CheckName(name)), defaut(AVSValue()) { }
  DescriptionArgument(const string& name, const AVSValue& _defaut)  : LinkageArgument(_defaut.type(), CheckName(name)), defaut(_defaut) { }

  const AVSValue& GetDefault() const { return defaut; }
  bool IsOptional() const { return ! defaut.empty(); }

};



/***********************************************************************************************/
/*********************************** Prototype classes *****************************************/
/***********************************************************************************************/



class LinkagePrototype : public vector<LinkageArgument> {

public:
  LinkagePrototype() { }
  LinkagePrototype(const LinkagePrototype& other) : vector<LinkageArgument>(other) { }
  LinkagePrototype(const ArgVector& args) : vector<LinkageArgument>(args.begin(), args.end()) { }

  //just check that no empty named args follow named ones
  bool IsLegal() const
  {
    const_iterator it = begin();
    while( it != end() && it->unNamed() ) { ++it; }  //skip unNamed args part
    if ( it == end() )
      return true;             //if no more, it's legal
    while( it != end() && ! it->unNamed() ) { ++it; }   //skip named args part
    return it == end();        //if at end, legal, otherwise unNamed after and illegal
  }

};

class DescriptionPrototype : public vector<DescriptionArgument> {

public:
  DescriptionPrototype() { }
  DescriptionPrototype(const DescriptionPrototype& other) : vector<DescriptionArgument>(other) { }
  DescriptionPrototype(const string& prototype);       //construction from a string

  //just check that no non-optional args follow optional ones
  bool IsLegal() const
  {
    const_iterator it = begin();
    while ( it != end() && ! it->IsOptional() ) { ++it; }   //skip non optional args part
    if ( it == end() )
      return true;                                          //if no more : legal
    while( it != end() && it->IsOptional() ) { ++it; }      //skip optional ones
    return it == end();                                     //we should be at end to be legal
  }

};








#endif //#define __PROTOTYPE_H__