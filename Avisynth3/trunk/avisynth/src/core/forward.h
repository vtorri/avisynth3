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


#ifndef __AVS_FORWARD_H__
#define __AVS_FORWARD_H__

//boost include
#include <boost/shared_ptr.hpp>    //for shared_ptr


namespace avs {



//declarations
class Clip;
class Property;
struct Vecteur;
class Dimension;
class VideoInfo;
class ColorSpace;
class VideoFrame;
class BufferWindow;
class RuntimeEnvironment;
template <class T> class cow_shared_ptr;

//typedefs
typedef boost::shared_ptr<Clip const> PClip;
typedef boost::shared_ptr<Property const> CPProperty;
typedef boost::shared_ptr<VideoInfo const> CPVideoInfo;
typedef boost::shared_ptr<VideoFrame const> CPVideoFrame;
typedef boost::shared_ptr<RuntimeEnvironment> PEnvironment;

//cow typedefs
typedef cow_shared_ptr<Property> PProperty;
typedef cow_shared_ptr<VideoInfo> PVideoInfo;
typedef cow_shared_ptr<VideoFrame> PVideoFrame;

//weak_ptr typedefs
typedef boost::weak_ptr<Clip const> WeakPClip;
typedef boost::weak_ptr<RuntimeEnvironment> WeakPEnvironment;


} //namespace avs

#endif //__AVS_FORWARD_H__