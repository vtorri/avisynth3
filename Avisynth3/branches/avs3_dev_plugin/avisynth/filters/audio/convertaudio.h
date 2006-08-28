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


#ifndef __AVS_FILTERS_CONVERTAUDIO_H__
#define __AVS_FILTERS_CONVERTAUDIO_H__


//avisynth include
#include "audioprocess.h"
#include "audiobuffered.h"
#include "../../sampletype.h"


#pragma warning ( push )           //push warning state
#pragma warning (disable : 4250)   //get rid of MI dominance decisions


namespace avs { namespace filters {


class ConvertAudio : public AudioProcess 
                   , private AudioBuffered
{

  SampleType type_;


public:  //constructor

  ConvertAudio(PClip child, SampleType type)
    : AudioProcess( child )
    , type_( type )
  {
    GetVideoInfo().SetSampleType( type );
  }


public:  //clip general interface

  //resolving MI ambiguity
  virtual void GetAudio(BYTE * buffer, __int64 start, int count) const
  {
    AudioBuffered::GetAudio(buffer, start, count);
  }


public:  //child changing clone

  virtual PClip clone(PClip child) const { return Create(child, type_); }


private:  //AudioBuffered interface

  virtual void Process(BYTE * buffer, BYTE * temp, __int64 start, int count, int channels) const
  {
    GetChild()->GetAudio(temp, start, count);
    Convert(buffer, temp, count * channels);
  }


private:  //ConvertAudio interface

  virtual void Convert(BYTE * output, BYTE * input, int sampleCount) const = 0;


public:  //factory method

  static PClip Create(PClip child, SampleType type);


public:  //target sample type specific inner classes

  class ToInt8;
  class ToInt16;
  class ToInt32;
  class ToFloat;


public:  //From inner subclasses

  template <class Type> class From;

  typedef From<char>  FromInt8;
  typedef From<short> FromInt16;
  typedef From<int>   FromInt32;
  typedef From<float> FromFloat;

};//ConvertAudio


/////////////////////////////////////////////////////////////////////////////////
//  ConvertAudio::From<Type>
//
//
//
template <class Type> class ConvertAudio::From : public ConvertAudio
{

public:  //constructor

  From(PClip child, SampleType type)
    : ConvertAudio( child, type ) { }

private:  //AudioBuffered interface

  virtual int BytesPerSampleToBuffer() const { return sizeof(Type); }

};



/////////////////////////////////////////////////////////////////////////////////////
//  ConvertAudio::Int8
//
//
//
class ConvertAudio::ToInt8
{

public:  //factory method

  static PClip Create(PClip child);


private:  //source sample type specific inner classes

  class FromInt16 : public ConvertAudio::From<short>
  {

  public:  //constructor

    FromInt16(PClip child)
      : From<short>( child, SAMPLE_INT8 ) { }


  private:  //ConvertAudio interface

    virtual void Convert(BYTE * output, BYTE * intput, int count) const;

  };


  class FromInt32 : public ConvertAudio::From<int>
  {

  public:  //constructor

    FromInt32(PClip child)
      : From<int>( child, SAMPLE_INT8 ) { }


  private:  //ConvertAudio interface

    virtual void Convert(BYTE * output, BYTE * intput, int count) const;

  };


  class FromFloat : public ConvertAudio::From<float>
  {

  public:  //constructor

    FromFloat(PClip child)
      : From<float>( child, SAMPLE_INT8 ) { }


  private:  //ConvertAudio interface

    virtual void Convert(BYTE * output, BYTE * intput, int count) const;

  };

};//ConvertAudio::ToInt8






////////////////////////////////////////////////////////////////////////////////////////
//  ConvertAudio::ToInt16
//
//
//
class ConvertAudio::ToInt16
{

public:  //factory method

  static PClip Create(PClip child);


private:  //source sample type specific inner classes

  class FromInt8 : public ConvertAudio::From<BYTE>
  {

  public:  //constructor

    FromInt8(PClip child)
      : From<BYTE>( child, SAMPLE_INT8 ) { }


  private:  //ConvertAudio interface

    virtual void Convert(BYTE * output, BYTE * intput, int count) const;

  };

  class FromInt32 : public ConvertAudio::From<int>
  {

  public:  //constructor

    FromInt32(PClip child)
      : From<int>( child, SAMPLE_INT8 ) { }


  private:  //ConvertAudio interface

    virtual void Convert(BYTE * output, BYTE * intput, int count) const;

  };
  
  class FromFloat : public ConvertAudio::From<float>
  {

  public:  //constructor

    FromFloat(PClip child)
      : From<float>( child, SAMPLE_INT8 ) { }


  private:  //ConvertAudio interface

    virtual void Convert(BYTE * output, BYTE * intput, int count) const;

  };  
    
};//ConvertAudio::ToInt16



////////////////////////////////////////////////////////////////////////////////////////
//  ConvertAudio::ToInt32
//
//
//
class ConvertAudio::ToInt32
{

public:  //factory method

  static PClip Create(PClip child);


private:  //source sample type specific inner classes

  class FromInt8 : public ConvertAudio::From<BYTE>
  {

  public:  //constructor

    FromInt8(PClip child)
      : From<BYTE>( child, SAMPLE_INT8 ) { }


  private:  //ConvertAudio interface

    virtual void Convert(BYTE * output, BYTE * intput, int count) const;

  };

  class FromInt16 : public ConvertAudio::From<short>
  {

  public:  //constructor

    FromInt16(PClip child)
      : From<short>( child, SAMPLE_INT8 ) { }


  private:  //ConvertAudio interface

    virtual void Convert(BYTE * output, BYTE * intput, int count) const;

  };
  
  class FromFloat : public ConvertAudio::From<float>
  {

  public:  //constructor

    FromFloat(PClip child)
      : From<float>( child, SAMPLE_INT8 ) { }


  private:  //ConvertAudio interface

    virtual void Convert(BYTE * output, BYTE * intput, int count) const;

  };  
    
};//ConvertAudio::ToInt32



////////////////////////////////////////////////////////////////////////////////////////
//  ConvertAudio::ToFloat
//
//
//
class ConvertAudio::ToFloat
{

public:  //factory method

  static PClip Create(PClip child);


private:  //source sample type specific inner classes

  class FromInt8 : public ConvertAudio::From<BYTE>
  {

  public:  //constructor

    FromInt8(PClip child)
      : From<BYTE>( child, SAMPLE_INT8 ) { }


  private:  //ConvertAudio interface

    virtual void Convert(BYTE * output, BYTE * intput, int count) const;

  };

  class FromInt16 : public ConvertAudio::From<short>
  {

  public:  //constructor

    FromInt16(PClip child)
      : From<short>( child, SAMPLE_INT8 ) { }


  private:  //ConvertAudio interface

    virtual void Convert(BYTE * output, BYTE * intput, int count) const;

  };
  
  class FromInt32 : public ConvertAudio::From<int>
  {

  public:  //constructor

    FromInt32(PClip child)
      : From<int>( child, SAMPLE_INT8 ) { }


  private:  //ConvertAudio interface

    virtual void Convert(BYTE * output, BYTE * intput, int count) const;

  };  
    
};//ConvertAudio::ToFloat



} } //namespace avs::filters

#pragma warning ( pop )

#endif //__AVS_FILTERS_CONVERTAUDIO_H__