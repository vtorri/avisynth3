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


#include <vector>
#include <string>
#include <algorithm>
using namespace std;


#include "avsvalue.h"


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
  LinkageArgument(const AVSValue& val) : name(""), type(val.type()) { }

  const string& GetName() const { return name; }
  const type_info& Type() const { return type; }

  bool unNamed() const { return name.empty(); }
  bool operator==(const string& _name) const { return name == _name; }
};



class DescriptionArgument : public LinkageArgument {

  const AVSValue defaut;

  static inline const string& CheckName(const string& name) { if (name.empty()) throw std::invalid_argument("the null chain is not a valid arg name"); }

public:
  DescriptionArgument(const string& name, const type_info& type, const AVSValue& _defaut = AVSValue())
    : LinkageArgument(type, CheckName(name)), defaut(_defaut) { }
  DescriptionArgument(const string& name, const AVSValue& _defaut)
    : LinkageArgument(_defaut.type(), CheckName(name)), defaut(_defaut) { }

  const AVSValue& GetDefault() const { return defaut; }
  bool IsOptional() const { return ! defaut.empty(); }

};



/***********************************************************************************************/
/*********************************** Prototype classes *****************************************/
/***********************************************************************************************/



class LinkagePrototype : public vector<LinkageArgument> {

  struct IsNameEmpty : unary_function<LinkageArgument, bool>
  {
    bool operator()(const LinkageArgument& arg) const { return arg.GetName().empty(); }
  };

public:
  LinkagePrototype() { }
  LinkagePrototype(const LinkagePrototype& other) : vector<LinkageArgument>(other) { }
  LinkagePrototype(const ArgVector& args) : vector<LinkageArgument>(args.begin(), args.end()) { }

  //just check that no empty named args follow named ones
  bool IsLegal() const
  {
    const_iterator it = find_if(begin(), end(), compose1(logical_not<bool>(), IsNameEmpty()) );
    return find_if(it, end(), IsNameEmpty()) != end();
  }

};

class DescriptionPrototype : public vector<DescriptionArgument> {

  struct IsNotOptional : unary_function<DescriptionArgument, bool>
  {
    bool operator()(const DescriptionArgument& arg) const { return ! arg.IsOptional(); }
  };

public:
  DescriptionPrototype() { }
  DescriptionPrototype(const DescriptionPrototype& other) : vector<DescriptionArgument>(other) { }
  
  //just check that no non-optional args follow optional ones
  bool IsLegal() const
  {
    const_iterator it = find_if(begin(), end(), compose1(logical_not<bool>(), IsNotOptional()) );
    return find_if(it, end(), IsNotOptional()) != end();
  }

};








#endif //#define __PROTOTYPE_H__