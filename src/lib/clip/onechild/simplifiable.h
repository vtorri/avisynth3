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


#ifndef __AVS_CLIP_ONECHILD_SIMPLIFIABLE_H__
#define __AVS_CLIP_ONECHILD_SIMPLIFIABLE_H__

//avisynth includes
#include "../onechild.h"
#include "../refactorable.h"


namespace avs { namespace clip { namespace onechild {



//////////////////////////////////////////////////////////////////////////////////////////
//  Simplifiable<SubClip>
//
//  provides a Simplify implementation for OneChild subclasses
//  when the child has the Refactorable<SubClip> signature
//
template <class SubClip>
class AVS_NOVTABLE Simplifiable : public virtual OneChild
{

public:  //typedef

  typedef Simplifiable<SubClip> SimplifiableType;


public:  //Simplify method

  virtual PClip Simplify() const
  {
    boost::shared_ptr<Refactorable<SubClip> const> isRef
      = boost::dynamic_pointer_cast<Refactorable<SubClip> const>( GetChild() );

    return (! isRef ) ? this->shared_from_this()
                      : isRef->Refactor( dynamic_cast<SubClip const&>(*this) );
  }

};


//////////////////////////////////////////////////////////////////////////////////////////
//  FinalSimplifiable<SubClip>
//
//  provides both Simplify and FinalSimplify implementation for OneChild subclasses
//
template <class SubClip>
class AVS_NOVTABLE FinalSimplifiable : public Simplifiable<SubClip>
{

public:  //typedef

  typedef FinalSimplifiable<SubClip> FinalSimplifiableType;


public:  //FinalSimplify method

  virtual PClip FinalSimplify() const
  {
    this->FinalSimplifyChild();

    boost::shared_ptr<FinalRefactorable<SubClip> const> isRef
      = boost::dynamic_pointer_cast<FinalRefactorable<SubClip> const>( this->GetChild() );

    return (! isRef ) ? this->shared_from_this()
                      : isRef->FinalRefactor( dynamic_cast<SubClip const&>(*this) );
  }

};


} } } //namespace avs::clip::onechild

#endif //__AVS_CLIP_ONECHILD_SIMPLIFIABLE_H__
