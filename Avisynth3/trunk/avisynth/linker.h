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
#include <utility> //for pair
#include <map>


class AVSTypeCaster {

public:
  virtual const AVSValue operator()(const AVSValue& arg) const = 0;
  virtual ~AVSTypeCaster() { }
};


class ArgumentMatcher {

  //static info
  typedef pair<const type_info *, const type_info *> CastPrototype;
  typedef auto_ptr<AVSTypeCaster> PTypeCaster;
  typedef map<CastPrototype, PTypeCaster> CastMap;

  static const CastMap castMap;

  static const CastMap CreateCastMap();

  //instance info
  bool success;
  AVSTypeCaster * caster;

public:
  ArgumentMatcher(const DescriptionArgument& descript, const LinkageArgument& link);

  bool Success() const { return success; }
  bool NeedCast() const { return caster != NULL; }

  AVSTypeCaster * GetTypeCaster() const { return caster; }

  operator bool() const { return Success(); }
};




//ReorderPattern keep track of reordering which must be performed
//to get args from the LinkagePrototype format to the DescriptionPrototype format
//now it only takes positions into account,
//but eventually it may take care of type cast too (int to float)
class ReorderPattern {

public:
  //structure holding the reorder info for one argument
  struct ReorderInfo {

    ReorderInfo(int _position);

    int position;

    bool operator==(const ReorderInfo& other) const { return position == other.position; }
  };

  typedef vector<ReorderInfo> InfoVector;
  typedef InfoVector::const_iterator iterator;


  //constructors
  ReorderPattern() { }
  ReorderPattern(const ReorderPattern& other) : infos(other.infos) { }

  void AddArgument(int position);

  int size() const { return infos.size(); }
  bool IsTrivial() const; //return true if no reorder to take
  
  iterator begin() const { return infos.begin(); }
  iterator end() const { return infos.end(); }

private:
  InfoVector infos;

};




class PrototypeMatcher {

  bool match, useImplicitLast;
  ReorderPattern pattern;

public:
  PrototypeMatcher(const DescriptionPrototype& descript, const LinkagePrototype& link);

  bool IsValid() const { return match; }
  bool UseImplicitLast() const { return useImplicitLast; }

  const ReorderPattern& GetReorderPattern() const { return pattern; }

  operator bool() const { return IsValid(); }  
};


//Reorder performs the actual reordering
//through its (virtual) operator ()
class Reorderer {

protected:
  const DescriptionPrototype& prototype;

  //fill args (supposedly empty) with the default arguments from prototype
  void DefaultFill(ArgVector& args) const;

public:
  Reorderer(const DescriptionPrototype& _prototype) : prototype(_prototype) { }

  virtual const ArgVector operator()(const ArgVector& rawArgs) const = 0;
  virtual ~Reorderer() { }  //virtual destructor

  static Reorderer * MakeReorderer(const DescriptionPrototype& prototype, const ReorderPattern& pattern);
};

//reorder who just forwards args without alteration
class TrivialReorderer : public Reorderer {

public:
  TrivialReorderer(const DescriptionPrototype& _prototype) : Reorderer(_prototype) { }

  virtual const ArgVector operator()(const ArgVector& rawArgs) const;
};

//reorder who performs reordering according to its pattern parameter
class PatternReorderer : public Reorderer {

  const ReorderPattern pattern;

public:
  PatternReorderer(const DescriptionPrototype& prototype, const ReorderPattern& _pattern)
    : Reorderer(prototype), pattern(_pattern) { }

  virtual const ArgVector operator()(const ArgVector& rawArgs) const;
};



class Linker {

  bool useImplicitLast;

public:
  Linker(const string& functionName, const LinkagePrototype&  lp, bool ImplicitLastAllowed = true);

  //test if last should be used, so it can be passed as first arg
  //this is necessary so this class won't have to fetch it herself
  //for Invoke calls, the vartable don't exist anymore
  //then it would have been a problem
  bool UseImplicitLast() const { return useImplicitLast; }

  AVSValue operator()(const ArgVector& args) const;


  static AVSValue Invoke(const string& functionName, const ArgVector& args)
  {
    LinkagePrototype prototype(args);
    Linker linker(functionName, prototype, false);
    return linker(args);
  }
};











#endif  //#define __LINKER_H__