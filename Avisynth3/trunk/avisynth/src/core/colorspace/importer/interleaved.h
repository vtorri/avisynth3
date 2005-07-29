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


#ifndef __AVS_COLORSPACE_IMPORTER_INTERLEAVED_H__
#define __AVS_COLORSPACE_IMPORTER_INTERLEAVED_H__

//avisynth includes
#include "../importer.h"
#include "../interleaved.h"
#include "../../ownedblock.h"
#include "../../bufferwindow.h"


namespace avs { namespace colorspace { namespace importer {



////////////////////////////////////////////////////////////////////////////////////
//  importer::interleaved<align>
//
//
//
template <int align> class interleaved : public Importer
{

  boost::shared_ptr<colorspace::Interleaved const> space_;


public:  //structors

  interleaved(PColorSpace const& space)
    : space_( boost::dynamic_pointer_cast<colorspace::Interleaved const>(space) ) { }

  //generated destructor is fine


public:  //Importer interface

  virtual PColorSpace GetColorSpace() const { return space_; }

  virtual CPVideoFrame CreateFrame(owned_block<1> const& block, Dimension const& dim, FrameType type) const
  {
    //create an align aligned frame buffer of the expected size by promoting the block
    //should not blit since the block is supposed to have the correct alignment
    buffer_window<align> main( space_->ToPlaneDim(dim, '~'), block, 0 );

    //use space_ to transform the frame buffer into a frame
    //if the size was favorable the conversion from buffer_window<align>
    //to buffer_window<block::Align> (ie BufferWindow) will not blit
    return space_->CreateFrame(dim, type, main);
  }

};



} } } //namespace avs::colorspace::importer

#endif //__AVS_COLORSPACE_IMPORTER_INTERLEAVED_H__
