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
//
// Linking Avisynth statically or dynamically with other modules is making a
// combined work based on Avisynth.  Thus, the terms and conditions of the GNU
// General Public License cover the whole combination.


#ifndef __VARTABLE_H__
#define __VARTABLE_H__

#include "avsvalue.h"
#include <vector>




class ContextLock;
class DepthLock;

class VarTable {

  typedef std::vector<AVSValue> VarVector;
  VarVector vars;
  int hiddenSize;

  int PushContext() { int result = hiddenSize; hiddenSize = vars.size(); return result; }
  void PopContext(int oldHiddenSize) { vars.resize(hiddenSize); hiddenSize = oldHiddenSize; }

  friend class ContextLock;  //so it can use the two above
  friend class DepthLock;

public:
  VarTable() : hiddenSize(0) { }

  AVSValue& operator[](int i) { return vars[i - hiddenSize]; }

  void PushValue(const AVSValue& value) { vars.push_back(value); }
};


class ContextLock {

  VarTable & table;
  int oldHiddenSize;

public:
  ContextLock(VarTable& _table) : table(_table), oldHiddenSize(_table.PushContext()) { }

  ~ContextLock() { table.PopContext(oldHiddenSize); }
};

class DepthLock {

  VarTable & table;
  int size;

public:
  DepthLock(VarTable& _table) : table(_table), size(_table.vars.size()) { }

  ~DepthLock() { table.vars.resize(size); }
}

#endif //#ifndef __VARTABLE_H__