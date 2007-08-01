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


#ifndef __AVS_FILTERS_MESSAGECLIP_H__
#define __AVS_FILTERS_MESSAGECLIP_H__

//avisynth include
#include "staticimage.h"

//stl include
#include <string>



namespace avs { 
  
  
//declaration
namespace text { namespace freetype { class Font; } }
  
  

namespace filters {



class MessageClip : public StaticImage
{

  std::string msg_;


private:  //structors

  MessageClip(std::string const& msg, PEnvironment const& env);

  MessageClip(std::string const& msg, CPVideoInfo const& vi, PEnvironment const& env);

  //generated destructor is fine
    

public:  //various clip factory methods

  static PClip Create(std::string const& msg, PEnvironment const& env) { return PClip( static_cast<Clip *>(new MessageClip(msg, env)) ); }

  static PClip CreateErrorClip(PClip const& source, std::string const& msg) { return PClip( static_cast<Clip *>(new MessageClip(msg, source->GetVideoInfo(), source->GetEnvironment())) ); }

  static PClip CreateVersionClip(PEnvironment const& env) { return Create("Avisynth 3.0 alpha", env); }


private:  

  static CPVideoFrame CreateFrame(std::string const& msg, text::freetype::Font const& font, Dimension const& dim, PColorSpace const& space, PEnvironment const& env);

  static CPVideoFrame CreateFrame(std::string const& msg, PEnvironment const& env);

};


} } //namespace avs::filters

#endif //__AVS_FILTERS_MESSAGECLIP_H__
