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


//avisynth includes
#include "staticimage.h"
#include "../../core/videoinfo.h"
#include "../../core/colorspace.h"
#include "../../core/videoframe.h"
#include "../../text/antialiaser.h"
#include "../../core/bufferwindow.h"
#include "../../core/cow_shared_ptr.h"
#include "../../core/exception/noaudio.h"
#include "../../core/runtime_environment.h"


namespace avs { namespace filters {



StaticImage::StaticImage(CPVideoFrame const& frame)
  : frame_( frame )
{
  PVideoInfo vi = VideoInfo::Create();

  vi->AddVideo(frame->GetColorSpace(), frame->GetDimension(), 25);

  vi_ = vi;
}


PEnvironment const& StaticImage::GetEnvironment() const { return frame_->GetEnvironment(); }


void StaticImage::GetAudio(void * /*buffer*/, long long /*start*/, int /*count*/) const
{
  throw exception::NoAudio();
}


PClip StaticImage::CreateBlankClip(ColorSpace& space, Dimension const& dim, PEnvironment const& env)
{
  return Create( env->CreateFrame(space, dim, PROGRESSIVE) );
}


PClip StaticImage::CreateMessageClip(std::string const& msg, PEnvironment const& env)
{
  text::Font font("Arial", 28, false, false);
  Dimension dim = font.GetTextBoundingBox(msg).Shift<4, 4>();

  PVideoFrame frame = env->CreateFrame(ColorSpace::rgb32(), dim, PROGRESSIVE);

  text::Antialiaser aliaser(dim, env, font);
  aliaser.SetText(msg, Vecteur(dim.GetWidth() >> 1, 0), text::TOP_CENTER);
  aliaser.Apply(*frame, 0xF0F0F0, 0);

  return Create(frame); 
}


} } //namespace avs::filters
