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


#ifndef __AVS_FORWARD_H__
#define __AVS_FORWARD_H__

//avisynth includes
#include "block/align.h"           //for block::Align
//#include "cow_shared_ptr.h"        //includes <boost/shared_ptr.hpp>

//boost include
#include <boost/rational.hpp>      //for rational



namespace boost {


template <typename T> class weak_ptr;
template <typename T> class shared_ptr;


}


namespace avs {


//declarations
class Key;
class Clip;
enum Plane;
class Cache;
class Blitter;
class Property;
enum FrameType;
class VideoInfo;
class FrameMaker;
class ColorSpace;
class VideoFrame;
class RuntimeEnvironment;
template <int align> class block_;
template <int align> class owned_block;
template <typename T> class box;
template <typename T> class vecteur;
template <typename T> class dimension;
template <class Data> struct window_ptr;
template <typename T> class cow_shared_ptr;
template <int align, int guard = block::Align, class Buffer = owned_block<1> > class buffer_window;

//typedefs
typedef unsigned char BYTE;
typedef box<long> Box;
typedef vecteur<long> Vecteur;
typedef dimension<long> Dimension;
typedef window_ptr<BYTE> WindowPtr;
typedef window_ptr<BYTE const> CWindowPtr;
typedef block_<block::Align> Block;
typedef owned_block<block::Align> OwnedBlock;
typedef buffer_window<block::Align> BufferWindow;
typedef boost::rational<long> Fraction;

//ptr typedefs
typedef boost::shared_ptr<Key const> PKey;
typedef boost::shared_ptr<Clip const> PClip;
typedef boost::shared_ptr<Property const> CPProperty;
typedef boost::shared_ptr<VideoInfo const> CPVideoInfo;
typedef boost::shared_ptr<ColorSpace const> PColorSpace;
typedef boost::shared_ptr<VideoFrame const> CPVideoFrame;
typedef boost::shared_ptr<RuntimeEnvironment> PEnvironment;

//cow ptr typedefs
typedef cow_shared_ptr<Property> PProperty;
typedef cow_shared_ptr<VideoInfo> PVideoInfo;
typedef cow_shared_ptr<VideoFrame> PVideoFrame;

//weak_ptr typedefs
typedef boost::weak_ptr<Clip const> WeakPClip;
typedef boost::weak_ptr<RuntimeEnvironment> WeakPEnvironment;


} //namespace avs

#endif //__AVS_FORWARD_H__
