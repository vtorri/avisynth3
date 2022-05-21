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
#include "trim.h"
#include "splice.h"
#include "splice/noaudio.h"
#include "splice/novideo.h"
#include "splice/aligned.h"
#include "splice/unaligned.h"
#include "../../core/videoinfo.h"


namespace avs { namespace filters {


Splice::Splice(CPVideoInfo const& vi, PEnvironment const& env)
  : vi_( vi )
  , env_( env )
{
  if ( vi_->HasVideo() )
    vi_->SetFrameCount( 0 );
  if ( vi_->HasAudio() )
    vi_->SetSampleCount( 0LL );
}


PClip Splice::Simplify() const
{
  return childs_.size() == 1 ? operator[]( 0 )
                             : shared_from_this();
}


PClip Splice::FinalSimplify() const
{
  for ( std::vector<PClip>::iterator it = childs_.begin(); it != childs_.end(); ++it )
    *it = (*it)->FinalSimplify();

  return shared_from_this();
}


void Splice::CheckCompatible(PClip const& clip)
{
  CPVideoInfo self = GetVideoInfo();
  CPVideoInfo other = clip->GetVideoInfo();

  if ( self->HasVideo() )              //if video
    self->CheckVideoMatch(*other);     //check other matchs
  else
    if ( other->HasVideo() )           //other has video, but not self
      self->CheckHasVideo();           //trigger exception

  if ( self->HasAudio() )              //same as above
    self->CheckAudioMatch(*other);
  else
    if ( other->HasAudio() )
      self->CheckHasAudio();
}


void Splice::Merge(Splice const& source)
{
  for ( std::vector<PClip>::const_iterator it = source.childs_.begin(); it != source.childs_.end(); ++it )
    PushChild( *it );
}


void Splice::SetFrameCount(int count)
{
  vi_->SetFrameCount(count);
}

void Splice::SetSampleCount(long long count)
{
  vi_->SetSampleCount(count);
}


bool Splice::MergingPush(PClip const& child)
{
  if ( ! childs_.empty() )
  {
    boost::shared_ptr<Trim const> left = boost::dynamic_pointer_cast<Trim const>(childs_.back());
    boost::shared_ptr<Trim const> right = boost::dynamic_pointer_cast<Trim const>(child);

    if (  left                                    //if left is a Trim
       && right                                   //if right is a Trim
       && left->GetChild() == right->GetChild()   //if they have same child
       && left->GetEnd() == right->GetBegin()     //if they are contiguous
       )
    {
      //update back with the appropriate Trim
      childs_.back() = Trim::Create(left->GetChild(), left->GetBegin(), right->GetEnd());
      return true;                                //report success
    }
  }

  childs_.push_back(child);                       //push child
  return false;                                   //report failure
}



PClip Splice::CreateAligned(PClip const& left, PClip const& right)
{
  CPVideoInfo vi = left->GetVideoInfo();
  PEnvironment const& env = left->GetEnvironment();

  boost::shared_ptr<Splice> splice;

  if ( vi->HasVideo() )
    splice.reset
        ( vi->HasAudio() ? static_cast<Splice *>(new splice::Aligned(vi, env))
                         : static_cast<Splice *>(new splice::NoAudio(vi, env))
        );
  else
    if ( ! vi->HasAudio() )
      vi->CheckHasVideo();        //force an exception in the no video no audio case
    else
      splice.reset( static_cast<Splice *>(new splice::NoVideo(vi, env)) );

  splice->AddChild(left);
  splice->AddChild(right);

  return splice->Simplify();
}


PClip Splice::CreateUnaligned(PClip const& left, PClip const& right)
{
  CPVideoInfo vi = left->GetVideoInfo();
  PEnvironment const& env = left->GetEnvironment();

  boost::shared_ptr<Splice> splice;

  if ( vi->HasVideo() )
    splice.reset
        ( vi->HasAudio() ? static_cast<Splice *>(new splice::Unaligned(vi, env))
                         : static_cast<Splice *>(new splice::NoAudio(vi, env))
        );
  else
    if ( ! vi->HasAudio() )
      vi->CheckHasVideo();        //force an exception in the no video no audio case
    else
      splice.reset( static_cast<Splice *>(new splice::NoVideo(vi, env)) );

  splice->AddChild(left);
  splice->AddChild(right);

  return splice->Simplify();
}


} } //namespace avs::filters
