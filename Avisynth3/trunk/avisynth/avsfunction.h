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


#ifndef __AVSFUNCTION_H__
#define __AVSFUNCTION_H__

//stl includes
#include <string>
#include <vector>
using namespace std;
//boost includes
#include <boost/any.hpp>   //for any
//avisynth includes
#include "refcounted.h"

//class declarations
class Clip;
class VarTable;
class ColorSpace;
class MatchResult;
class LinkagePrototype;
class ScriptEnvironment;
class DescriptionPrototype;

//typedefs
typedef boost::any AVSValue;
typedef vector<AVSValue> ArgVector;
typedef smart_ptr<Clip> PClip;


//base class for plugins
class Plugin : public RefCounted {

  const string name;

public:
  Plugin(const string& _name) : name(_name) { }

  const string& GetName() const { return name; }

  virtual MatchResult FunctionSearch(const string& name, const LinkagePrototype& prototype, bool implicitLastAllowed = false) = 0;
};

typedef smart_ptr<Plugin> PPlugin;


//base class for all function in avisynth
//each function accessible in scripts has its own instance
//those are owned by plugins and so they don't need refcounting
class AVSFunction {

  const string name;

public:
  AVSFunction(const string& _name) : name(_name) { }

  virtual ~AVSFunction() { } //virtual destructor

  virtual void AddRef() = 0;
  virtual void Release() = 0;

  const string& GetName() const { return name; }
  virtual const type_info& GetReturnType() const = 0;
  virtual const DescriptionPrototype& GetPrototype() const = 0;  

  //return plugin from which this function depends
  //core functions return the special core plugin
  //script functions return a NULL smart_ptr (or ?)
  virtual PPlugin GetMotherPlugin() const = 0;

  //useful for determining priority in overload resolution
  enum FunctionType {
    CORE,
    PLUGIN,
    SCRIPT
  };
  virtual FunctionType GetFunctionType() const = 0;

  //function call with all the checks
  //invalid_argument exception if error
  AVSValue Call(const ArgVector& args, VarTable& table, ScriptEnvironment& env);

protected:
  //method who do the real work
  //args are passed in correct type and count according to prototype
  virtual AVSValue operator() (const ArgVector& args, VarTable& table, ScriptEnvironment& env) = 0;

  friend class CallBackFunction;  //so it can forward to the above

};

typedef smart_ptr<AVSFunction> PAVSFunction;



/*************************************************************************************************/
/********************************** CorePlugin and CoreFunction **********************************/
/*************************************************************************************************/



class CoreFunction;

class CorePlugin : public Plugin {

  typedef vector<CoreFunction *> FunctionList;

  FunctionList list;

  friend class CoreFunction; //so it can push itself on the above

  //private contructor 
  CorePlugin() : Plugin("Avisynth Core") { }

public:  
  virtual MatchResult FunctionSearch(const string& name, const LinkagePrototype& prototype, bool implicitLastAllowed = false) = 0;

  //the sole instance of the core
  static CorePlugin core;   
};



//function of the core
//for each function of them, there should be one instance of it (declared static)
class CoreFunction : public AVSFunction {

public:
  CoreFunction(const string& name) : AVSFunction(name) { CorePlugin::core.list.push_back(this); }

  //core functions are static, no refcounting needed
  virtual void AddRef() { }
  virtual void Release() { }

  virtual PPlugin GetMotherPlugin() const { return PPlugin(CorePlugin::core); }

  virtual FunctionType GetFunctionType() const { return CORE; }
};

//template class which take care of the GetReturnType method
template <class T> class TypedCoreFunction : public CoreFunction {

public:
  TypedCoreFunction(const string& name) : CoreFunction(name) { }

  virtual const type_info& GetReturnType() const { return typeid(T); }
};

//template who take care of GetPrototype too, provided you supply a string prototype at construction
//it invalidates DescriptionPrototype sharing, but it's unlikely to happen much for filters
template <class T> class PrototypedCoreFunction : public TypedCoreFunction<T> {

  DescriptionPrototype prototype;

public:
  PrototypedCoreFunction(const string& name, const string& _prototype)
    : TypedCoreFunction(name), prototype(_prototype) { }

  virtual const DescriptionPrototype& GetPrototype() const { return prototype; }
};

//convenience typedef of the above
typedef PrototypedCoreFunction<PClip> CoreFilter;







//general callback avsfunction wrapper
class CallBackFunction : public AVSFunction {

  PAVSFunction callBack;
  Counter counter;

public:
  CallBackFunction(AVSFunction& _callBack) : AVSFunction(_callBack.GetName()), callBack(_callBack) { }

  virtual void AddRef() { counter.AddRef(); }
  virtual void Release() { if (counter.Release() == 0) delete this; }

  virtual const type_info& GetReturnType() const { return callBack->GetReturnType(); }
  virtual const DescriptionPrototype& GetPrototype() const { return callBack->GetPrototype(); }
  
  virtual FunctionType GetFunctionType() const { return callBack->GetFunctionType(); }
  virtual PPlugin GetMotherPlugin() const { return callBack->GetMotherPlugin(); }

protected:
  virtual AVSValue operator() (const ArgVector& args, VarTable& table, ScriptEnvironment& env) const { return (*callBack)(args, table, env); }
};













#endif // #define __AVSFUNCTION_H__