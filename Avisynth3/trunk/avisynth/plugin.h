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


#ifndef __PLUGIN_H__
#define __PLUGIN_H__


#include "avsfunction.h"


class MatchResult : public PrototypeMatcher {

  AVSFunction& function;

public:
  MatchResult(AVSFunction& _function, const LinkagePrototype& link)
    : PrototypeMatcher(_function.GetPrototype(), link), function(_function) { }


  bool operator< (const MatchResult& other) const ;  //used to determine overload priority

  PAVSFunction Wrap(PEnvironment env) const { return IsTrivial() ? new CallBackFunction(function, env) : new ReorderingCallBackFunction(function, env, *this); }

};


typedef vector<MatchResult> SearchResult;








class NativePlugin : public Plugin {

  mutable Counter counter;

  string pluginName;

  typedef vector<PluginFunction> FunctionVector;

  FunctionVector functions;

  //private constructor, use factory method instead
  //do all the work, link to get filled with plugin function and ....
  NativePlugin(const string& filename); 

public:
  virtual void AddRef() const { counter.AddRef(); }
  virtual void Release() const { if (counter.Release() == 0) delete this; }

  virtual const string& GetName() const { return pluginName; }
 
  //factory method, used in place of a constructor
  //so it may implement instances folding
  static PPlugin GetPlugin(const string& filename); 
};




class PluginFunction : public AVSFunction {

  const type_info& type;
  const DescriptionPrototype prototype;
  NativePlugin& mother;

public:
  PluginFunction(const string& name, const type_info& _type, const DescriptionPrototype& _prototype, NativePlugin& _mother)
    : AVSFunction(name), type(_type), prototype(_prototype), mother(_mother) { }
  //copy constructor
  PluginFunction(const PluginFunction& other) : AVSFunction(other.GetName()), type(other.type), prototype(other.prototype), mother(other.mother) { }

  virtual const DescriptionPrototype& GetPrototype() const { return prototype; }
  virtual const type_info& GetReturnType() const { return type; }

  virtual PPlugin GetMotherPlugin() const; // { return mother; }
  virtual FunctionType GetFunctionType() const { return PLUGIN; }
};





#endif //#define __PLUGIN_H__