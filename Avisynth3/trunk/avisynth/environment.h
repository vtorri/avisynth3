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


#ifndef  __ENVIRONMENT_H__
#define  __ENVIRONMENT_H__



#include "prototype.h"  //includes linker.h, avsvalue.h...

#include <utility>    //for pair
#include <map>

//some typedefs
class Plugin;
typedef smart_ptr<Plugin> PPlugin;
class LinkedFunction;
typedef smart_ptr<LinkedFunction> PAVSFunction;

//pair to describe a full link result, the bool says if last is expected by the AVSFunction 
typedef pair<PAVSFunction, bool> LinkResult;  

class MemoryBuffer;

class ScriptEnvironment : public RefCounted {

  typedef vector<PPlugin> PluginVector;

  PluginVector pluginVector;
  

  int MemoryMax;
  mutable int MemoryUsed;

  void RegisterMemory(int size) const;
  void FreeMemory(int size) const;

  friend class MemoryBuffer;  //so it can use the two above

public:
  ScriptEnvironment();
  ScriptEnvironment(const ScriptEnvironment& other);


  void LoadPlugin(const string& pathName);

  AVSValue Invoke(const string& functionName, const ArgVector& args);
  AVSValue Parse(const string& script);

  static long __stdcall GetCPUFlags();


  LinkResult Link(const string& name, const LinkagePrototype& link, bool ImplictLastAllowed);
  //simple linkage, implicit last is not allowed here
//  PAVSFunction SimpleLink(const string& name, const LinkagePrototype& link) { return Link(name, link, false).first; }


  PAVSFunction Bind(PAVSFunction toBind, const ArgVector& bindedArgs);



};

typedef smart_ptr<ScriptEnvironment> PEnvironment;


#endif   //#define  __ENVIRONMENT_H__
