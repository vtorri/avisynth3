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


#ifndef __AVS_CLIP_REFACTORABLE_H__
#define __AVS_CLIP_REFACTORABLE_H__


//avisynth include
#include "../core/clip.h"


namespace avs { namespace clip {


namespace onechild { 
  
//declarations
template <class T> class InitialSimplifiable;
template <class T> class Simplifiable; 

}



///////////////////////////////////////////////////////////////////////////////////////
//  InitialRefactorable<Parent>
//
//  Signature class of clips able of initial refactoring a parent of type Parent
//
template <class Parent> 
class NOVTABLE InitialRefactorable : public virtual Clip
{

private:  //Refactor method

  //Refactorisation method (associated with parent type)
  //parent is passed so info can be extracted from it
  virtual PClip InitialRefactor(Parent const& parent) const = 0;

  friend class onechild::InitialSimplifiable<Parent>;

};


///////////////////////////////////////////////////////////////////////////////////////
//  FinalRefactorable<Parent>
//
//  Signature class of clips able of initial refactoring a parent of type Parent
//
template <class Parent> 
class NOVTABLE FinalRefactorable : public virtual Clip
{

private:  //Refactor method

  //Refactorisation method (associated with parent type)
  //parent is passed so info can be extracted from it
  virtual PClip FinalRefactor(Parent const& parent) const = 0;

  friend class onechild::Simplifiable<Parent>;

};


} } //namespace avs::clip

#endif //__AVS_CLIP_REFACTORABLE_H__
