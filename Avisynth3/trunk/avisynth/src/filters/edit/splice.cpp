// Avisynth v3.0 alpha.  Copyright 2003 David Pierre - Ben Rudiak-Gould et al.
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
#include "splice.h"
#include "../../core/videoinfo.h"
#include "../../core/exception/novideo.h"
#include "../../core/exception/nosuchframe.h"


namespace avs { namespace filters {



Splice::Splice(CPVideoInfo const& vi, PEnvironment const& env)
  : env_( env )
{
  PVideoInfo vi_ = vi;

  if ( vi_->HasVideo() )
    vi_->SetFrameCount( 0 );
  if ( vi_->HasAudio() )
    vi_->SetSampleCount( 0LL );

  this->vi_ = vi_;
}


CPVideoFrame Splice::GetFrame(int n) const
{
  //search position where inserting respect the switchs ordering
  VideoSwitchOver::const_iterator it = std::upper_bound(videoSwitchs_.begin(), videoSwitchs_.end(), n);

  if ( it == videoSwitchs_.end() )      //insertion at end is suggested: ie beyond the frame range of the last clip
    if ( vi_->HasVideo() )              //if clip has video
      throw exception::NoSuchFrame(n);  //report frame don't exist
    else
      throw exception::NoVideo();       //else report no video (the video switchs vector was empty coz of that)

  //NB: it[-1] exists: the vector is not empty and we are not at begin()
  //NB2: the n < 0 case is taken care of by the first clip of the splice
  return childs_[ it - videoSwitchs_.begin() ]->GetFrame( n - (it == videoSwitchs_.begin() ? 0 : it[-1]) );
}


void Splice::GetAudio(void * buffer, long long start, int count) const
{

}


PClip Splice::Simplify() const
{
  if ( childs_.size() == 1 )
    return childs_.back();

  return shared_from_this();
}


void Splice::AddChild(PClip const& child)
{
  //check if child is a Splice itself
  boost::shared_ptr<Splice const> splice = boost::dynamic_pointer_cast<Splice const>(child);

  if ( splice )  //if it is
  {              //absorb it into self
    for ( ClipVector::const_iterator it = splice->childs_.begin(); it != splice->childs_.end(); ++it )
      AddChild( *it );
  }


}



} } //namespace avs::filters
