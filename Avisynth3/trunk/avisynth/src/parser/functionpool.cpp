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


//avisynth include
#include "functionpool.h"
#include "../linker/function.h"
#include "../core/exception/generic.h"


namespace avs { namespace parser {



void FunctionPool::Resolve(std::string const& prototype, TypedCode& target) const
{
  
  for(FunctionVector::const_iterator it = functionList_.begin(); it != functionList_.end(); ++it)
    if ( prototype == (*it)->GetPrototype() )
    {
      (*it)->AppendOperation(target.first);
      target.second = (*it)->GetReturnType();
      return;
    }

  throw exception::Generic("Unable to resolve function");

}


} } //namespace avs::parser