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


#ifndef __AVS_FILTERS_AVIFILESOURCE_H__
#define __AVS_FILTERS_AVIFILESOURCE_H__

//avisynth include
#include "avisource.h"


struct IAVIFile;
struct IAVIStream;


namespace avs { namespace filters {



///////////////////////////////////////////////////////////////////////////////////////////
//  AviFileSource
//
//  AviSource subclass opening avis using vfw
//
class AviFileSource : public AviSource
{

  typedef boost::shared_ptr<IAVIFile> PAVIFile;
  typedef boost::shared_ptr<IAVIStream> PAVIStream;

  struct Initializer
  {
    Initializer();
    ~Initializer(); 
  };


  Initializer initializer_;           //must be 1st, so destroyed after avifile and avistreams

  PAVIFile aviFile_;
  PAVIStream video_;
  CPVideoInfo vi_;


public:  //structors

  AviFileSource(std::string const& fileName, PEnvironment const& env);

  //generated destructor is fine


public:  //clip general interface

  virtual CPVideoInfo GetVideoInfo() const { return vi_; }

  virtual BYTE * GetAudio(BYTE * buffer, __int64 start, int count) const;


private:  //AviSource private interface

  virtual int PreviousKeyFrame(int n) const;

  virtual long ReadVideo(int n, BYTE * buffer, long bufferSize) const;


private:  //implementation helpers

  //get stream of asked type from an avifile, returns NULL if don't exist
  static IAVIStream * GetStream(PAVIFile const& aviFile, unsigned long fccType);

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
