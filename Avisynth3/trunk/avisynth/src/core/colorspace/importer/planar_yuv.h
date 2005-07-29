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


#ifndef __AVS_COLORSPACE_IMPORTER_PLANAR_YUV_H__
#define __AVS_COLORSPACE_IMPORTER_PLANAR_YUV_H__

//avisynth includes
#include "../importer.h"
#include "../yuv/planar.h"
#include "../../videoframe.h"
#include "../../ownedblock.h"
#include "../../bufferwindow.h"
#include "../../cow_shared_ptr.h"   //so PVideoFrame is defined


namespace avs { namespace colorspace { namespace importer {



////////////////////////////////////////////////////////////////////////////////////
//  importer::planar_yuv<alignY, alignUV>
//
//
//
template <long alignY, long alignUV> class planar_yuv : public Importer
{

  boost::shared_ptr<colorspace::yuv::Planar const> space_;


public:  //structors

  planar_yuv(PColorSpace const& space)
    : space_( boost::dynamic_pointer_cast<colorspace::yuv::Planar const>(space) ) { }

  //generated destructor is fine


public:  //Importer interface

  virtual PColorSpace GetColorSpace() const { return space_; }

  virtual CPVideoFrame CreateFrame(owned_block<1> const& block, Dimension const& dim, FrameType type) const
  {
    Dimension dimY  = space_->ToPlaneDim(dim, 'Y');
    Dimension dimUV = space_->ToPlaneDim(dim, 'U');

    //split into independant blocks each containing a plane
    owned_block<1> blockY = block;
    owned_block<1> blockU = blockY.Split( dimY.BMPSize<alignY>() );
    owned_block<1> blockV = blockU.Split( dimUV.BMPSize<alignUV>() );

    //promote the blocks into frame buffers with the proper alignment
    buffer_window<alignY>  y( dimY, blockY, 0 );
    buffer_window<alignUV> u( dimUV, blockU, 0 );
    buffer_window<alignUV> v( dimUV, blockV, 0 );
    
    return space_->CreateFrame(dim, type, y, u, v);
  }


public:  //SwapUV inner class

  class SwapUV : public planar_yuv<alignY, alignUV>
  {

  public:  //structors

    SwapUV(PColorSpace const& space) : planar_yuv<alignY, alignUV>( space ) { }
    //generated destructor is fine


  public:  //Importer interface

    virtual CPVideoFrame CreateFrame(owned_block<1> const& block, Dimension const& dim, FrameType type) const
    {
      PVideoFrame result = planar_yuv<alignY, alignUV>::CreateFrame(block, dim, type);

      swap((*result)['U'], (*result)['V']);

      return result;
    }

  };

};



} } } //namespace avs::colorspace::importer

#endif //__AVS_COLORSPACE_IMPORTER_PLANAR_YUV_H__
