// Avisynth v3.0 alpha.  Copyright 2004 David Pierre - Ben Rudiak-Gould et al.
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


//avisynth includes
#include "plugin.h"
#include "../../functions/edit.h"
#include "../../functions/audio.h"
#include "../../functions/combine.h"
#include "../../functions/convert.h"
#include "../../functions/levels.h"
#include "../../functions/script.h"
#include "../../functions/source.h"


//stl include
#include <functional>   //for identity


namespace avs { namespace linker { namespace core {


Plugin::Plugin()
{
  functions::Edit::RegisterAll(*this);
  functions::Audio::RegisterAll(*this);
  functions::Combine::RegisterAll(*this);
  functions::Convert::RegisterAll(*this);
  functions::Levels::RegisterAll(*this);
  functions::Script::RegisterAll(*this);
  functions::Source::RegisterAll(*this);
}


void Plugin::FillFunctionList(FunctionList& addTo) const
{
  for( CoreFunctionList::const_iterator it = list_.begin(); it != list_.end(); ++it )
    addTo.push_back( PFunction(*it, std::identity<void *>()) );
}


Plugin::PCore Plugin::Get() 
{ 
  static Plugin instance;
  return PCore( &instance, std::identity<void *>() ); 
}


} } } //namespace avs::linker::core
