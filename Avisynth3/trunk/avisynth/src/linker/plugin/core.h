// Avisynth v3.0 alpha.  Copyright 2004 Ben Rudiak-Gould et al.
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


#ifndef __AVS_LINKER_PLUGIN_CORE_H__
#define __AVS_LINKER_PLUGIN_CORE_H__

//avisynth include
#include "../plugin.h"
#include "../function/core.h"


namespace avs { namespace linker { namespace plugin {



class Core : public Plugin
{

  std::vector<function::Core *> functions_;


private:  //structors

  Core()
    : Plugin( "Core" ) { }

  //generated destructor is fine


public:  //plugin interface

  virtual FunctionList GetFunctionList() const;


private:

  void Register(function::Core * funct) { functions_.push_back( funct ); }


private:  //sole instance

  static Core instance;

  friend function::Core;

};


} } } //namespace avs::linker::plugin

#endif //__AVS_LINKER_PLUGIN_CORE_H__