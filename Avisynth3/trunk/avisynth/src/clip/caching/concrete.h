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


#ifndef __AVS_CLIP_CACHING_CONCRETE_H__
#define __AVS_CLIP_CACHING_CONCRETE_H__

//avisynth includes
#include "../caching.h"
#include "../../core/runtime_environment.h"

//boost include
#include <boost/scoped_ptr.hpp>


namespace avs { namespace clip { namespace caching {



/////////////////////////////////////////////////////////////////////////////////
//  Concrete
//
//  implementation of the Caching interface
//
class Concrete : public virtual Caching
{

  boost::scoped_ptr<Cache> cache_;


protected:  //SetFrameMaker method

  void SetFrameMaker(FrameMaker const& maker, PEnvironment const& env)
  { 
    cache_.reset( env->CreateCache(maker) );
  }


private:  //Caching requirement

  virtual Cache& GetCache() const { return *cache_; }

};



} } } //namespace avs::clip::caching

#endif //__AVS_CLIP_CACHING_CONCRETE_H__
