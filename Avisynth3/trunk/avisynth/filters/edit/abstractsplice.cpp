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


//avisynth include
#include "abstractsplice.h"

//stl includes
#include <iterator>   //for back_inserter
#include <algorithm>  //for transform



CPVideoFrame AbstractSplice::GetFrame(int n, const CachingClip& client) const
{
  //if no childs or neg frame, or more than frame count
  if ( ChildCount() == 0 || n < 0 || video_sw.back() <= n )  
    ThrowNoSuchFrameException( n );  //exception
  //search where n inserts itself in the ordered video switchs vector
  IntVector::const_iterator it = video_sw.begin();  
  while ( *it <= n ) { ++it; }       //search for > n 
  --it;        //it's the preceding value we use (valid, we know that 0 was skipped)
  return childs_[ it - video_sw.begin() ]->GetFrame( n - *it, client );
}



void AbstractSplice::GetAudio(BYTE * buf, __int64 start, __int64 count) const
{
  __int64 final_switch = audio_sw.back();  //switch after which there is no more audio
  __int64 end = start + count;             //end of requested samples (not included)

  //if no childs, or before clip, or after clip
  if ( ChildCount() == 0 || end < 0 || final_switch <= start )      
    FillWithBlank(buf, count);  //blank noise (may throw if no audio)
  else {
    int sample_size = GetVideoInfo().BytesPerAudioSample();  //bytes per audio samples 

    if ( final_switch < end )
    {
      __int64 local_count = end - final_switch;
      FillWithBlank(buf + (int)local_count * sample_size, local_count);
      count -= local_count;
    } //now we know we don't overlap after audio from self

    if ( start < 0 )
    {
      FillWithBlank(buf, -start);
      count += start;
      buf -= start * sample_size;
      start = 0;
    } //now we know we don't overlap before audio from self

    //search where start inserts itself in the ordered audio switchs vector
    Int64Vector::const_iterator it = audio_sw.begin(); 
    while( *it <= start ) { ++it; }       //search for > start

    for (
        ClipVector::const_iterator clip_it = childs_.begin() + ( it - audio_sw.begin() - 1 );
        *it < end;         //we know this condition ensure not falling from vectors
        ++it, ++clip_it
        )
    {
      //samples count for current clip to handle
      __int64 local_count = std::min( *it - start, count );     

      //ask child to do his part
      (*clip_it)->GetAudio(buf, start - *(it - 1), local_count);

      start += local_count;
      count -= local_count;
      buf += (int)local_count * sample_size;
    }//for

  }//else
}//GetAudio




void AbstractSplice::AddChild(PClip child)
{
  
  const VideoInfo& c_vi = child->GetVideoInfo();
  VideoInfo vi = GetVideoInfo();

  vi.CheckCompatible( c_vi );     //exceptions is incompatible

  if ( ! c_vi.IsNull() )          //if child is not null
  {                               //we splice (else not)

    int frame_count = 0;          //frames to splice
    __int64 sample_count = 0;     //samples to splice

    if ( vi.HasVideo() )    
    {
      vi.AddToFrameCount( frame_count = c_vi.GetFrameCount() );

      if ( vi.HasAudio() )
        vi.AddToSampleCount( sample_count = SamplesToSpliceFrom(c_vi) );
    }
    else                      //no video
      if ( vi.HasAudio() )
        vi.AddToSampleCount( sample_count = c_vi.GetSampleCount() );

    //vi is now updated

    //if child is absorbable
    if ( const AbstractSplice * other = IsAbsorbable( child.get() ) )
    {
      //we absorb it
      childs_.insert( childs_.end(), other->childs_.begin(), other->childs_.end() );

      std::transform( other->video_sw.begin() + 1, other->video_sw.end(), 
          std::back_inserter(video_sw), std::bind1st(std::plus<int>(), video_sw.back()) );
      
      std::transform( other->audio_sw.begin() + 1, other->audio_sw.end(),
          std::back_inserter(audio_sw), std::bind1st(std::plus<__int64>(), audio_sw.back()) );
    }
    else  //not absorbable
    {
      childs_.push_back( child );
      video_sw.push_back( frame_count + video_sw.back() );
      audio_sw.push_back( sample_count + audio_sw.back() );
    }

    SetVideoInfo( vi );  //update internal videoinfo
  }
}

