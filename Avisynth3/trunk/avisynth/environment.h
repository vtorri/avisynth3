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
#include "pclip.h"
#include <utility>    //for pair
#include <map>

//some typedefs
class Plugin;
typedef smart_ptr<Plugin> PPlugin;
class LinkedFunction;
typedef smart_ptr<LinkedFunction> PAVSFunction;

//pair to describe a full link result, the bool says if last is expected by the AVSFunction 
typedef pair<PAVSFunction, bool> LinkResult;  


class ScriptEnvironment : public RefCounted {

  typedef map<string, AVSValue> VarTable;
  typedef vector<PPlugin> PluginVector;

  VarTable varTable;
  PluginVector pluginVector;
  
  //to avoid refs circularities the env vartable does not take PClip
  //(PClip may hold a ref to the env)
  //made this test as a method ot ease further change 
  static bool AcceptType(const type_info& type) { return type != typeid(PClip) && type != typeid(CPVideoFrame); }

public:
  ScriptEnvironment();
  ScriptEnvironment(const ScriptEnvironment& other);

  //vars stuff
  bool GlobalVarExists(const string& name) const { return varTable.find(name) != varTable.end(); }
  const AVSValue& GetGlobarVar(const string& name) { return varTable[name]; }
  void SetGlobalVar(const string& name, const AVSValue& val) { if (AcceptType(val.type())) varTable[name] = val; }


  void LoadPlugin(const string& pathName);

  AVSValue Invoke(const string& functionName, const ArgVector& args);
  AVSValue Parse(const string& script);

  static long __stdcall GetCPUFlags();
//  static void __stdcall BitBlt(BYTE* dstp, int dst_pitch, const BYTE* srcp, int src_pitch, int row_size, int height);


  LinkResult Link(const string& name, const LinkagePrototype& link, bool ImplictLastAllowed);
  //simple linkage, implicit last is not allowed here
  PAVSFunction SimpleLink(const string& name, const LinkagePrototype& link) { return Link(name, link, false).first; }


  PAVSFunction Bind(PAVSFunction toBind, const ArgVector& bindedArgs);


};

typedef smart_ptr<ScriptEnvironment> PEnvironment;


#endif   //#define  __ENVIRONMENT_H__
