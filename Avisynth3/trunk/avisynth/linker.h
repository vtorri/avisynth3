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


#ifndef  __LINKER_H__
#define  __LINKER_H__

#include "prototype.h"
#include <map>



//holds a caster
class CasterHolder {
  
protected:
  //polymorphic struct used for avs type casting
  struct Caster {

    virtual AVSValue operator()(const AVSValue& arg) const = 0;
  };

  struct IntToFloatCaster : Caster {

    virtual AVSValue operator()(const AVSValue& arg) const { return AVSValue( (float)boost::any_cast<int>(arg) ); }
  };

  struct IdentityCaster : Caster {

    virtual AVSValue operator()(const AVSValue& arg) const { return arg; }
  };

  static const IntToFloatCaster itf;
  static const IdentityCaster id;

  const Caster * caster;

public:
  CasterHolder() : caster(&id) { }
  CasterHolder(const CasterHolder& other) : caster(other.caster) { }

  AVSValue operator()(const AVSValue& arg) const { return (*caster)(arg); }

  DescriptionArgument operator()(const DescriptionArgument& arg) const { if ( IsIdentity() ) ret

  bool IsIdentity() const { return caster == &id; }
};

//
class ArgumentMatcher : public CasterHolder {

  //static info
  struct  CastPrototype {
    const type_info& src;
    const type_info& dst;

    CastPrototype(const type_info& _src, const type_info& _dst) : src(_src), dst(_dst) { }

    bool operator==(const CastPrototype& other) const { return src == other.src && dst == other.dst; }
    bool operator<(const CastPrototype& other) const { return src.before(other.src) || ( src == other.src && dst.before(other.dst) ); }
  };
  typedef map<CastPrototype, const Caster *> CastMap;

  static const CastMap castMap;
  //method to initialize the cast map
  static CastMap CreateCastMap();

  //instance info
  bool success;


public:
  ArgumentMatcher(const DescriptionArgument& descript, const LinkageArgument& link);

  bool Success() const { return success; }
};


class ReorderInfo : public CasterHolder {

  int position;

public:
  ReorderInfo(const ArgumentMatcher& arg, int _position) : CasterHolder(arg), position(_position) { }

  int GetPosition() const { return position; }
  operator int() const { return position; }

  //used by Reorderpattern to test its legality
  bool operator==(const ReorderInfo& other) const { return position == other.position; }
};

//ReorderPattern keep track of reordering which must be performed
//to get args from the LinkagePrototype format to the DescriptionPrototype format
class ReorderPattern : public vector<ReorderInfo> {

public:
  ReorderPattern() { }
  ReorderPattern(const ReorderPattern& other) : vector<ReorderInfo>(other) { }

  bool IsTrivial() const; //return true if no reorder to take
 
  DescriptionPrototype Adapt(const DescriptionPrototype& old) const;

};




class PrototypeMatcher : public ReorderPattern {

  bool match, useImplicitLast;  

  void TryToMatch(const DescriptionPrototype& descript, const LinkagePrototype& link);

  bool AddToPattern(const ArgumentMatcher& arg, int position)
  {
    bool result = arg.Success();
    if ( result )
      push_back(ReorderInfo(arg, position));
    return result;
  }

public:
  PrototypeMatcher(const DescriptionPrototype& descript, const LinkagePrototype& link);

  bool Success() const { return match; }
  bool UseImplicitLast() const { return useImplicitLast; }
};








#endif  //#define __LINKER_H__