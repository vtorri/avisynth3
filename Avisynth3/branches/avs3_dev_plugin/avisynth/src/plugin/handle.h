// Avisynth v3.0 alpha.  Copyright 2003-2005 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_PLUGIN_HANDLE_H__
#define __AVS_PLUGIN_HANDLE_H__

//avisynth include
#include "../../define.h"       //for AVS_NOVTABLE, AVS_NORETURN


namespace avs { namespace plugin {



///////////////////////////////////////////////////////////////////////////////////////////////
//  plugin::Handle
//
//
//
class AVS_NOVTABLE Handle
{

public:  //Handle interface

  //fetchs a pointer to a symbol from within the plugin
  //returns NULL if it doesn't exist
  virtual void * GetSymbol(char const * symbolName) const = 0;

  virtual void AVS_NORETURN ThrowBadPlugin() const = 0;


public:  //convenience helpers

  void * CheckingGetSymbol(char const * symbolName) const
  {
    void * symbol = GetSymbol(symbolName);    
    if ( symbol == NULL )
      ThrowBadPlugin();
    return symbol;
  }

  template <typename Function> Function * GetSymbolAs(char const * symbolName) const
  {
    return reinterpret_cast<Function *>(GetSymbol(symbolName));
  }

  template <typename Function> Function * CheckingGetSymbolAs(char const * symbolName) const
  {
    return reinterpret_cast<Function *>(CheckingGetSymbol(symbolName));
  }

};



} } //namespace avs::plugin

#endif //__AVS_PLUGIN_HANDLE_H__
