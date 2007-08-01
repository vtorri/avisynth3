// Avisynth v3.0 alpha.  Copyright 2005 David Pierre - Ben Rudiak-Gould et al.
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
#include "maker.h"
#include "packer.h"
#include "interleaved2x2.h"
#include "../../../core/utility/round.h"


namespace avs { namespace filters { namespace resize { namespace pattern {



interleaved<2, 2>::interleaved(PEnvironment const& env, Filter const& filter, SubRange const& subrange, int size)
  : Base( env )
{
  Maker make(filter, subrange, size);

  size = utility::RoundUp<2>(size);       //rounds up size to a multiple of 2           
  
  int realCount = make.count();           //true number of coeffs
  int count = utility::RoundUp<2>(realCount + 1);  //must be even with at least one zero for compensation (so we can ensure even offset)
                              
  Init( count, size * (1 + count / 2) );

  packer<1, 2> zero( 0, Get() );          //packer for even pixels
  packer<1, 2> one( 1, Get() + 1 );       //packer for odd pixels
  

  for ( int i = 0; i < size; ++i )
  {
    packer<1, 2>& pack = (i & 1) ? one : zero;  //select the appropriate packer

    int pads = realCount - count;     //number of zeros of padding (which have to be made)

    int offset = make.offset();       //fetch offset for this pixel

    switch( offset & 3 )
    {

    case 2:                           //offset is even, but...
      if ( pads == 2 )                //if pads is 2
      {
        pack.offset( offset - 2 );    //we can make it 0 mod 4 (better alignment opportunities)
        pack.coeff( 0 );              //make necessary padding
        pack.coeff( 0 );
        pads = 0;                     //update pads
        break;                        //break the switch
      }

    case 0:                           //offset is even (we may fall through from case 2)
      pack.offset( offset );          //we pack it
      break;                          //and done

    default:  //ie case 1 and 3
      pack.offset( offset - 1 );      //we remove 1 to make it even
      pack.coeff( 0 );                //necessary padding
      --pads;                         //update pads

    }

    for( int k = count; k-- > 0; )    //pack all true coeffs
      pack.coeff( make.coeff() );

    for( ; pads-- > 0; )              //final pads with zeros
      pack.coeff( 0 );
  }

}



} } } } //namespace avs::filters::resize::pattern
