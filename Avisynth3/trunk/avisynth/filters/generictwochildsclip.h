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


#ifndef __AVS_GENERICTWOCHILDSCLIP_H__
#define __AVS_GENERICTWOCHILDSCLIP_H__


//avisynth includes
#include "cachingclip.h"
#include "twochildsclip.h"


#pragma warning ( push )           //push warning state
#pragma warning (disable : 4250)   //get rid of MI dominance decisions




///////////////////////////////////////////////////////////////////////////////
// GenericTwoChildsClip
//
// clip with two childs, a cache, and its own videoinfo
//
class GenericTwoChildsClip : public TwoChildsClip, public CachingClip
{

protected:  //structors

  GenericTwoChildsClip(PClip left, PClip right, const CachePolicy& policy = DefaultCachePolicy())
    : TwoChildsClip( left, right )
    , CachingClip( left->GetEnvironment(), policy )
  {
    DispatchTo( GetLeftChild() );
    DispatchTo( GetRightChild() );
  }

  virtual ~GenericTwoChildsClip()
  {
    WithdrawFrom( GetLeftChild() );
    WithdrawFrom( GetRightChild() );
  }
  

public:  //cache hints methods

  //resolving MI ambiguities
  virtual void Dispatch(const CacheRequest& request) const { CachingClip::Dispatch(request); }
  virtual void Withdraw(const CacheRequest& request) const { CachingClip::Withdraw(request); }


protected:  //write access

  void SetLeftChild(PClip left)
  {
    WithdrawFrom( GetLeftChild() );
    TwoChildsClip::SetLeftChild( left );
    DispatchTo( GetLeftChild() );
  }

  void SetRightChild(PClip right)
  {
    WithdrawFrom( GetRightChild() );
    TwoChildsClip::SetRightChild( right );
    DispatchTo( GetRightChild() );
  }

};


#pragma warning ( pop ) 

#endif //__AVS_GENERICTWOCHILDSCLIP_H__