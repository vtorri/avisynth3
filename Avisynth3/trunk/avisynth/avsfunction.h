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

#include "prototype.h"
#include "colorspace.h"
#include "pclip.h" 
#include <functional>


class Plugin;

//base class for all function in avisynth
//each function accessible in scripts has its own instance
//(except some special functions who are done by the parser)
class AVSFunction {

  const string name;

public:
  AVSFunction(const string& _name) : name(_name) { }
  
  const string& GetName() const { return name; }
  virtual const type_info& GetReturnType() const = 0;
  virtual const DescriptionPrototype& GetPrototype() const = 0;

  //return the plugin this function depends
  //(for internals, there is a special plugin representing the core)
  virtual const Plugin * GetMotherPlugin() const = 0;
  virtual Plugin * GetMotherPlugin() = 0;  
  
  //small useful test
  bool IsFilter() const { return GetReturnType() == typeid(PClip); }
  //type_info::operator== returns int in microsoft world....  :/

  //useful for determining priority in overload resolution
  virtual bool IsUserFunction() const { return false; }
  virtual bool IsCoreFunction() const { return false; }

  //method used to list accepted colorspaces
  virtual bool AcceptColorSpace(const ColorSpace& space) const = 0;


protected:
  virtual AVSValue operator() (const vector<AVSValue>& args) const = 0;

  friend class Linker;

};

template <typename ReturnType> class TypedAVSFunction : public AVSFunction {

public:
  TypedAVSFunction(const string& name) : AVSFunction(name) { }

  virtual const type_info& GetReturnType() const { return typeid(ReturnType); }

};

//base class for plugins
class Plugin {

protected:
  typedef vector<auto_ptr<AVSFunction> > FunctionVector;

  FunctionVector functionVector;

public:
  virtual const string& GetName() const = 0;
  virtual const string& GetAuthor() const = 0;
  virtual const string& GetDescription() const = 0;

  virtual bool IsLoaded() const = 0;

  virtual void Load() = 0;
  virtual void UnLoad() = 0;

  //iterator used to remove one level of indirection from FunctionVector
  //(works like an AVSFunction * when it's in fact AVSFunction **)
  class iterator {
    
    FunctionVector::const_iterator it;

  public:
    iterator() { }
    iterator(FunctionVector::const_iterator _it) : it(_it) { }
    iterator(const iterator& other) : it(other.it) { }

    const iterator& operator ++() { ++it; return *this; }
    iterator operator++(int) { return iterator(it++); }

    bool operator==(const iterator& other) const { return it == other.it; }

    const AVSFunction&  operator*() const  { return *(it->get()); }
    const AVSFunction * operator->() const { return it->get(); }
  };

  iterator begin() const { return iterator(functionVector.begin()); }
  iterator end()  const  { return iterator(functionVector.end()); }
};




/*******************************************************************************************/
/***************************** CorePlugin and CoreFunction *********************************/
/*******************************************************************************************/

template <typename ReturnType> class InternalFunction;

class CorePlugin : public Plugin {

  //private contructor 
  CorePlugin() { }

public:
  //returns the sole instance of the core
  static CorePlugin * GetCore();
    
  //method called by InternalFunction to register themselves as a part of the core
  void RegisterFunction(AVSFunction * funct);

  virtual const string& GetName() const;
  virtual const string& GetAuthor() const;
  virtual const string& GetDescription() const;

  //the core is always loaded
  virtual bool IsLoaded() const { return true; }
  //so these have no effect
  virtual void Load() { }
  virtual void UnLoad() { }

};

//common superclass for user-defined function and core function
template <typename ReturnType> class InternalFunction : public TypedAVSFunction<ReturnType> {  
  
public:
  //construct and register itself at the core
  InternalFunction(const string& name)
    : TypedAVSFunction<ReturnType>(name) { CorePlugin::GetCore()->RegisterFunction(this); }

  virtual Plugin * GetMotherPlugin() { return CorePlugin::GetCore(); }
 
};

//function defined by user
//how they work has yet to be determined
template <typename ReturnType> class UserFunction : public InternalFunction<ReturnType> {

public:
  UserFunction(const string& name) : InternalFunction<ReturnType>(name) { }
    
  virtual bool IsUserFunction() const { return true; }

  //user defined function have not the possibility to select colorspace
  //they accept everything (and eventually complain after)
  virtual bool AcceptColorSpace(const ColorSpace& space) const { return true; }

};

//function of the core
//for each function of the core, there should be one instance of it
template <typename ReturnType> class CoreFunction : public InternalFunction<ReturnType> {

public:
  CoreFunction(const string& name) : InternalFunction<ReturnType>(name) { }

  virtual bool IsCoreFunction() const { return true; }

};

//convenience subclass of the above (economize writing CLIP_T all the time)
class CoreFilter : public CoreFunction<PClip> {

public:
  CoreFilter(const string& name) : CoreFunction<PClip>(name) { }

};



/*********************************************************************************************/
/************************** NativePlugin and PluginFunction **********************************/
/*********************************************************************************************/

template <typename ReturnType> class PluginFunction;

class NativePlugin : public Plugin {

  string pluginName;
  string pluginAuthor;
  string pluginDescription;

  HMODULE plugin;
  bool loaded;

  void RegisterFunction(PluginFunction * funct);

public:
  //do all the work, link to get filled with plugin function and ....
  NativePlugin(const string& filename); 

  virtual const string& GetName() const { return pluginName; }
  virtual const string& GetAuthor() const { return pluginAuthor; }
  virtual const string& GetDescription() const { return pluginDescription; }

  virtual bool IsLoaded() const { return loaded; }

  virtual void Load();
  virtual void UnLoad();
};


typedef AVSValue (* ProcessFunction)(const ArgVector& args);

template <typename ReturnType> class PluginFunction : public TypedAVSFunction<ReturnType> {

  NativePlugin * mother;
  const DescriptionPrototype prototype;

  ProcessFunction pf;

  //method should be called by mother plugin on all its filters when load is done
  void SetProcessFunction(ProcessFunction _pf) { pf = _pf; }
  friend NativePlugin;

public:
  //register the self as a function of mother
  PluginFunction(NativePlugin * _mother, const string& _name, const DescriptionPrototype& _prototype);

  virtual const DescriptionPrototype& GetPrototype() const { return prototype; }

  virtual const Plugin * GetMotherPlugin() const { return mother; }
  virtual Plugin * GetMotherPlugin() { return mother; }


};














#endif // #define __AVSFUNCTION_H__