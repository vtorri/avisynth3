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


#ifndef __AVS_CLIP_FOLDED_MAKE_H__
#define __AVS_CLIP_FOLDED_MAKE_H__

//avisynth include
#include "../folded.h"


namespace avs { namespace clip { namespace folded {



/////////////////////////////////////////////////////////////////////////////////
//  folded::Make<SuperClass, Key, Expired>
//
//  helper template to create a folding clip subclass from a clip class
//
template <class SuperClass, class Key, class Expired = WeakExpired> 
class Make : public Folded<Key, Expired>
           , public SuperClass        
{

public:  //structors

  template <typename T> 
  Make(T const& t)
    : SuperClass( t ) { }

  template <typename T, typename U>
  Make(T const& t, U const& u)
    : SuperClass( t, u ) { }

  //generated destructor is fine


public:  //Simplify method

  virtual PClip Simplify() const
  {
    PClip sp = SuperClass::Simplify();
    
    return sp != shared_from_this() ? sp : FoldedType::Simplify();
  }


private:  //GetKey method

  virtual Key GetKey() const { return SuperClass::GetKey(); }

};


} } } //namespace avs::clip::folded

#endif //__AVS_CLIP_FOLDED_MAKE_H__