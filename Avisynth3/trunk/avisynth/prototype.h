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

class Argument {

  const string name;

protected:
  static const type_info & IsNotVoid(const type_info&  type)
  {
    if ( type == typeid(void) )
      throw std::invalid_argument("void is not a valid argument type");
    return type;
  }

public:
  Argument(const string& _name) : name(_name) { }

  const string& GetName() const { return name; }
  bool unNamed() const { return name.empty(); }

  bool operator==(const string& _name) const { return name == _name; }
};


class LinkageArgument : public Argument {

  const type_info& type;

public:
  LinkageArgument(const type_info& _type, const string& name = "") : Argument(name), type(IsNotVoid(_type)) { }
  LinkageArgument(const AVSValue& val) : Argument(""), type(IsNotVoid(val.Type())) { }

  const type_info& Type() const { return type; }
};


class DescriptionArgument : public Argument {

  const AVSValue defaut;

public:
  DescriptionArgument(const string& name, const AVSValue& _defaut)
    : Argument(name), defaut(_defaut)
  { 
    IsNotVoid(defaut.Type());
    if (name.empty())
      throw std::invalid_argument("the null chain is not a valid arg name");
  }

  const AVSValue& GetDefault() const { return defaut; }
  const type_info & Type() const { return defaut.Type(); }
  bool IsOptional() const { return ! defaut.IsDefined(); }

};



/***********************************************************************************************/
/*********************************** Prototype classes *****************************************/
/***********************************************************************************************/

template <typename ArgumentType> class prototype {

protected:
  typedef ArgumentType arg_type;
  typedef vector<ArgumentType> ArgList;
  
  ArgList argList;

  prototype() { }
  prototype(const prototype<ArgumentType>& other) : argList(other.argList) { }


  void add(const arg_type& arg)
  {
    if (! arg.GetName().empty() && find(arg.GetName()) != end() )
      throw std::invalid_argument(arg.GetName() + " is already a defined arg");
    argList.push_back(arg);
  }

public:
  int size() const { return argList.size(); }
  bool empty() const { return argList.empty(); }

  const arg_type& back() const { return argList.back(); }

  typedef typename ArgList::const_iterator iterator;

  iterator begin() const { return argList.begin(); }
  iterator end() const { return argList.end(); }
  iterator find(const string& name) const { return std::find(argList.begin(), argList.end(), name); }

};


class LinkagePrototype : public prototype<LinkageArgument> {

public:
  LinkagePrototype() { }
  LinkagePrototype(const LinkagePrototype& other) : prototype<LinkageArgument>(other) { }
  LinkagePrototype(const ArgVector& args)
  {
    for(ArgVector::const_iterator it = args.begin(); it != args.end(); )
      argList.push_back(LinkageArgument(*it));
  }

  void Add(const LinkageArgument& arg)
  {
    if( arg.unNamed() && ! empty() && ! back().unNamed() )
      throw std::invalid_argument("Only named arg can follow named one");
    add(arg);
  }
};

class DescriptionPrototype : public prototype<DescriptionArgument> {

public:
  DescriptionPrototype() { }
  DescriptionPrototype(const DescriptionPrototype& other) : prototype<DescriptionArgument>(other) { }
  
  void Add(const DescriptionArgument& arg)
  {
    if (! arg.IsOptional() && ! empty() && back().IsOptional())
      throw std::invalid_argument("can't have optional arg followed by plain one");
    add(arg);
  }

};








#endif //#define __PROTOTYPE_H__