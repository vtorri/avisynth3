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


#include "linker.h" 
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

typedef vector<AVSFunction *> FunctionList;

//base class for plugins
class Plugin {

  FunctionList list;

protected:
  //before adding to list, check that eventual overload is legal
  void AddFunction(AVSFunction& funct) throw(std::invalid_argument);

public:
  Plugin() { }

  virtual ~Plugin() { }

  virtual void AddRef() const = 0;
  virtual void Release() const = 0;

  virtual const string& GetName() const = 0;

  //returns the list of known functions
  const FunctionList& GetFunctionList() const { return list; }
  //return NULL when not found
  //it supposes you cannot change return type when overloading (common requirement)
  const type_info * ReturnTypeOf(const string& name) const; 
  //search for function of given name and matching the given prototype
  void FunctionSearch(SearchResult& result, const string& name, const LinkagePrototype& link);
};

typedef smart_ptr<Plugin> PPlugin;



class CorePlugin : public Plugin {

  //private contructor 
  CorePlugin() { }

  //method called by CoreFunction to register themselves
  void RegisterFunction(CoreFunction& funct){ AddFunction(funct); }

  friend class CoreFunction; //so it can call the above

public:
  virtual void AddRef() const { }
  virtual void Release() const { }

  virtual const string& GetName() const { static const string name = "Avisynth Core"; return name; }
  
  //the sole instance of the core
  static CorePlugin core;   
};



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



#endif //#define __PLUGIN_H__