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


#ifndef __AVS_CLIP_FOLDED_H__
#define __AVS_CLIP_FOLDED_H__

//avisynth include
#include "../folder.h"
#include "../core/clip.h"
#include "../weakexpired.h"


namespace avs { namespace clip {



////////////////////////////////////////////////////////////////////////////////////////
//  Folded<Key, Expired>
//
//
//
template <class Key, class Expired = WeakExpired> class Folded : public virtual Clip
{

public:  //typedefs

  typedef Key KeyType;
  typedef Folded<Key> FoldedType;
  typedef folder<Key, WeakPClip, Expired> FolderType;


private:  //member

  static FolderType folder_;


public:  //structors

  Folded() { }
  virtual ~Folded() { folder_.TickToCleanUp(); }


public:  //Simplify method

  virtual PClip Simplify() const
  {
    return ( folder_[ GetKey() ] = shared_from_this() ).lock();
  }


private:  //GetKey method

  virtual Key GetKey() const = 0;

};

template <class Key, class Expired>
folder<Key, WeakPClip, Expired> Folded<Key, Expired>::folder_;


} } //namespace avs::clip

#endif //__AVS_CLIP_FOLDED_H__
