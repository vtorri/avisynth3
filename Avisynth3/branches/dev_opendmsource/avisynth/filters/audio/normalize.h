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


#ifndef __AVS_FILTERS_NORMALIZE_H__
#define __AVS_FILTERS_NORMALIZE_H__


//avisynth include
#include "audioprocess.h"


namespace avs { namespace filters {


/////////////////////////////////////////////////////////////////////////////////
//  Normalize
//
//  Normalize a child audio track
//
class Normalize : public AudioProcess
{

  float level_;
  float factor;


public:  //constructor

  Normalize(PClip child, float level);


public:  //child changing clone

  virtual Clip * clone(PClip child) const;


public:  //factory method

  static Create(PClip child);


private:  //sample type specific inner subclasses

  class Int16;
  class Float;


protected:  //implementation helpers

  float GetFactor(BYTE * buf, int count) const;

  virtual float GetPeak(BYTE * buf, int count) const = 0;

};


class Normalize::Int16 : public Normalize
{

public:  //constructor

  Int16(PClip child, float level)
    : Normalize( child, level ) { }


public:  //clip public interface

  virtual void GetAudio(BYTE * buf, __int64 start, int count) const;


protected:

  virtual float GetPeak(BYTE * buf, int count) const;

};


class Normalize::Float : public Normalize
{

public:  //constructor

  Float(PClip child, float level)
    : Normalize( child, level ) { }


public:  //clip public interface

  virtual void GetAudio(BYTE * buf, __int64 start, int count) const;


protected:

  virtual float GetPeak(BYTE * buf, int count) const;

};


} } //namespace avs::filters

#endif //__AVS_FILTERS_NORMALIZE_H__