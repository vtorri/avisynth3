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


#ifndef __AVSFUNCTION_H__
#define __AVSFUNCTION_H__

#include "linker.h"
#include "pclip.h"     //which includes colorspace.h
#include "environment.h"
#include <functional>
#include <vector>
#include <utility>     //for pair



//base class for all function in avisynth
//each function accessible in scripts has its own instance
//those are owned by plugins and so they don't need refcounting
class AVSFunction {

  const string name;

public:
  AVSFunction(const string& _name) : name(_name) { }

  virtual ~AVSFunction() { }

  const string& GetName() const { return name; }
  virtual const type_info& GetReturnType() const = 0;
  virtual const DescriptionPrototype& GetPrototype() const = 0;  

  //return plugin from which this function depends
  //core functions return the special core plugin
  //script functions return a NULL smart_ptr
  virtual PPlugin GetMotherPlugin() const = 0;

  //small useful test
  bool IsFilter() const { return GetReturnType() == typeid(PClip); }
  //type_info::operator== returns int in microsoft world....  :/

  //useful for determining priority in overload resolution
  enum FunctionType {
    CORE,
    PLUGIN,
    SCRIPT
  };
  virtual FunctionType GetFunctionType() const = 0;

  //method used to list accepted colorspaces
  virtual bool AcceptColorSpace(const ColorSpace& space) const = 0;


protected:
  //method who do the real work
  //args are passed in correct type and count according to prototype
  virtual AVSValue operator() (const ArgVector& args, PEnvironment env) const = 0;

  friend class CallBackFunction;  //so it can forward to the above

};




class ExternalFunction : public AVSFunction {

  const type_info& type;
  const DescriptionPrototype prototype;

public:
  ExternalFunction(const string& name, const type_info& _type, const DescriptionPrototype& _prototype)
    : AVSFunction(name), type(_type), prototype(_prototype) { }

  virtual const DescriptionPrototype& GetPrototype() const { return prototype; }
  virtual const type_info& GetReturnType() const { return type; }


};

//function defined by user
//how they work has yet to be determined
/*class UserFunction : public ExternalFunction {

public:

    
  virtual bool IsUserFunction() const { return true; }

  //user defined function have not the possibility to select colorspace
  //they accept everything (and eventually complain after)
  virtual bool AcceptColorSpace(const ColorSpace& space) const { return true; }

};*/


class NativePlugin; 

class PluginFunction : public ExternalFunction {

  NativePlugin& mother;

public:
  PluginFunction(const string& name, const type_info& type, const DescriptionPrototype& prototype, NativePlugin& _mother)
    : ExternalFunction(name, type, prototype), mother(_mother) { }
  PluginFunction(const PluginFunction& other)
    : ExternalFunction(other.GetName(), other.GetReturnType(), other.GetPrototype()), mother(other.mother) { }

  virtual PPlugin GetMotherPlugin() const; // { mother.AddRef(); return &mother; }
  virtual FunctionType GetFunctionType() const { return PLUGIN; }

};



//function of the core
//for each function of the core, there should be one instance of it
class CoreFunction : public AVSFunction {

public:
  CoreFunction(const string& name); //: AVSFunction(name) { CorePlugin::core.RegisterFunction(*this); }

  virtual FunctionType GetFunctionType() const { return CORE; }
};

//template class whihc take care of the GetReturnType method
//when writing core functions, you should subclass one of those
template <class T> class TypedCoreFunction : public CoreFunction {

public:
  TypedCoreFunction(const string& name) : CoreFunction(name) { }

  virtual const type_info& GetReturnType() const { return typeid(T); }

};

//convenience typedef of the above 
typedef TypedCoreFunction<PClip> CoreFilter;







/*************************************************************************************************/
/**************************** LinkedFunction and subclasses **************************************/
/*************************************************************************************************/





//those are the function who are passed by the environment when linking
class LinkedFunction : public AVSFunction, public RefCounted {

  PEnvironment env; 

public:
  LinkedFunction(const string& name, PEnvironment _env) : AVSFunction(name), env(_env) { }

  PEnvironment GetEnvironment() const { return env; }

  //function call with all the checks
  //invalid_argument exception if error
  AVSValue Call(const ArgVector& args) throw(std::invalid_argument);

};

typedef smart_ptr<LinkedFunction> PAVSFunction;


class MatchResult;  //defined in plugin.h
//it's that class Wrap method who creates CallBackFunction and ReorderingCallBackFUnction

//call back wrapper around an AVSFunction
//no argument reordering for this one
class CallBackFunction : public LinkedFunction {

  AVSFunction& callBack;

public:
  virtual const type_info& GetReturnType() const { return callBack.GetReturnType(); }
  virtual const DescriptionPrototype& GetPrototype() const { return callBack.GetPrototype(); }
  
  virtual FunctionType GetFunctionType() const { return callBack.GetFunctionType(); }
  virtual PPlugin GetMotherPlugin() const { return callBack.GetMotherPlugin(); }

  virtual bool AcceptColorSpace(const ColorSpace& space) const { return callBack.AcceptColorSpace(space); }

protected:
  //protected constructor
  CallBackFunction(AVSFunction& _callBack, PEnvironment _env)
    : LinkedFunction(_callBack.GetName(), _env), callBack(_callBack) { }

  friend class MatchResult;  //so it can use the constructor

  virtual AVSValue operator()(const ArgVector& args, PEnvironment env) const { return callBack(args, env); }

};

//callback wrapper with reordering
class ReorderingCallBackFunction : public CallBackFunction {

  ReorderPattern pattern;
  DescriptionPrototype prototype;

  //private constructor
  ReorderingCallBackFunction(AVSFunction& _callBack, PEnvironment _env, const ReorderPattern& _pattern)
    : CallBackFunction(_callBack, _env), prototype(_pattern.Adapt(_callBack.GetPrototype())), pattern(_pattern)  { }

  friend class MatchResult;  //so it can use the constructor

public:
  virtual const DescriptionPrototype& GetPrototype() const { return prototype; }

protected:
  virtual AVSValue operator()(const ArgVector& args, PEnvironment env) const;
};









/************************/







#endif // #define __AVSFUNCTION_H__