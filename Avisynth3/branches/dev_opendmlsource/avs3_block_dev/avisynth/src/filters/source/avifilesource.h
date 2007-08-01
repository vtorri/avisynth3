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

#ifdef _WIN32

#ifndef __AVS_FILTERS_AVIFILESOURCE_H__
#define __AVS_FILTERS_AVIFILESOURCE_H__

//avisynth includes
#include "avifile/rawaudio.h"
#include "avifile/rawvideo.h"
#include "../../clip/framemaker/concrete.h"


namespace avs { namespace filters {



///////////////////////////////////////////////////////////////////////////////////////////
//  AviFileSource
//
//
//
class AviFileSource : public source::avifile::RawAudio
                    , public source::avifile::RawVideo
                    , public clip::framemaker::Concrete
{

  PAVIFile aviFile_;
  CPVideoInfo vi_;


public:  //structors

  AviFileSource(std::string const& fileName, PEnvironment const& env);

  //generated destructor is fine


public:  //clip general interface

  virtual CPVideoInfo GetVideoInfo() const { return vi_; }


public:  //implementation helpers

  //get stream of asked type from an avifile, returns empty if don't exist
  static PAVIStream GetStream(PAVIFile const& aviFile, unsigned long fccType);

  //wraps result from IAVIStream::ReadFormat into a shared_ptr
  static boost::shared_ptr<void> ReadFormat(PAVIStream const& aviStream);


public:  //factory method and functor

  static PClip Create(std::string const& fileName, PEnvironment const& env);

  struct Creator
  {
    PClip operator()(std::string const& fileName, PEnvironment const& env) const { return Create(fileName, env); }
  };

};



} } //namespace avs::filters

#endif //__AVS_FILTERS_AVIFILESOURCE_H__

#endif //_WIN32
