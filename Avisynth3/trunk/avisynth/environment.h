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


//STL includes
#include <map>
#include <string>
#include <vector>
#include <utility>    //for pair
using namespace std;

//avisynth includes
#include "refcounted.h"


//some class declarations
class Cache;
class Plugin;
class AVSValue;
class AVSFunction;
class MemoryPiece;
class LinkagePrototype;
class ScriptEnvironment;


//some typedefs
typedef smart_ref<Plugin> PPlugin;
typedef vector<AVSValue> ArgVector;
typedef smart_ref<AVSFunction> PAVSFunction;
typedef pair<PAVSFunction, bool> LinkResult;  //pair to describe a full link result, the bool says if last is expected by the AVSFunction 
typedef smart_ref<ScriptEnvironment> PEnvironment;



class ScriptEnvironment : public RefCounted {

  /*
   * Plugin stuff
   */
  typedef vector<PPlugin> PluginVector;

  PluginVector pluginVector;   //the vector of plugins

public:
  //load the plugin specified by pathName, exception if failure (which ?)  
  void LoadPlugin(const string& pathName);  


  /*
   * Memory management stuff
   */
private:
  int memMax;         //maximum set for memory usage
  int mallocUsed;     //currently allocated by this::malloc
  int totalMemUsed;   //total of memory used (counting the frame pipeline)

  void FireMemoryOverMax();

  void MemoryAllocated(int size) { totalMemUsed += size; if (totalMemUsed > memMax) FireMemoryOverMax(); }  
  void MemoryFreed(int size)  { totalMemUsed -= size; }

  friend class OwnedMemoryPiece;  //so it can use the two above
  
  typedef map<void *, MemoryPiece *> AllocationMap;

  AllocationMap allocMap;  //map who holds the memory blocks allocated by the below malloc

public:
  //memory allocation/deallocation methods
  //correctly update memory usage by the env
  //usage is expected for frames-like size, (relatively) small size can skip it
  void * malloc(int size);
  void free(void * ptr);

private:
  typedef vector<Cache *> CacheVector;

  CacheVector cacheVector;   //vector of all known Caches
  //needed in order to request caches to release frames (and so memory)

public:
  //methods used by clips to register/unregister their caches
  //generally you don't have to call them yourself
  //subclassing the right clip class handles it for you
  void RegisterCache(Cache * cache) { cacheVector.push_back(cache); }
  void UnRegisterCache(Cache * cache) { cacheVector.erase( find(cacheVector.begin(), cacheVector.end(), cache), cacheVector.end() ); }


  /*
   * General Stuff
   */
public:
  ScriptEnvironment();
  ScriptEnvironment(const ScriptEnvironment& other);

  virtual ~ScriptEnvironment();

  LinkResult Link(const string& functionName, const LinkagePrototype& link, bool ImplicitLastAllowed);

  AVSValue Invoke(const string& functionName, const ArgVector& args);
  AVSValue Parse(const string& script);

  static long __stdcall GetCPUFlags();

  bool operator==(const ScriptEnvironment& other) const { return this == &other; }
  bool operator!=(const ScriptEnvironment& other) const { return this != &other; }
};


#endif   //#define  __ENVIRONMENT_H__
