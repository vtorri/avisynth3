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
#include "antialiaser.h"
#include "../core/colorspace.h"
#include "../core/videoframe.h"
#include "antialiaser/bitcount.h"
//#include "antialiaser/bitexpand.h"
#include "../core/exception/colorspace/unsupported.h"


namespace avs { namespace text {



Antialiaser::Antialiaser(Dimension const& dim, PEnvironment const& env, Font const& font)
  : dirty_( true )
  , alphaBuffer_( dim.Multiply<2, 1>(), env )
  , bitRenderer_( dim.Shift<4, 4>(), font ) { }





void Antialiaser::Apply(VideoFrame& frame, int textColor, int haloColor) 
{
  assert( (frame.GetDimension() == alphaBuffer_.GetDimension().Divide<2, 1>()) );

  if ( dirty_ )
    UpdateAlpha();

  switch( frame.GetColorSpace()->id() )
  {
  case ColorSpace::I_RGB24: ApplyRGB24( frame, textColor, haloColor ); break;
  case ColorSpace::I_RGB32: ApplyRGB32( frame, textColor, haloColor ); break;
  case ColorSpace::I_YUY2: ApplyYUY2( frame, textColor, haloColor ); break;
  case ColorSpace::I_YV12: ApplyYV12( frame, textColor, haloColor ); break;
  
  default: throw exception::cspace::Unsupported(frame.GetColorSpace());
  }
  
}



void Antialiaser::ApplyRGB24(VideoFrame& frame, int textColor, int haloColor) 
{
  int Rtext = ((textColor>>16)&255), Gtext = ((textColor>>8)&255), Btext = (textColor&255);
  int Rhalo = ((haloColor>>16)&255), Ghalo = ((haloColor>>8)&255), Bhalo = (haloColor&255);
  
  WindowPtr dst = frame.WriteTo(NOT_PLANAR);
  CWindowPtr alpha = alphaBuffer_.Read();

  for ( int y = alpha.height; y-- > 0; dst.pad(), alpha.pad() ) 
    for ( int x = alpha.width >> 1; x-- > 0; dst.to(3, 0), alpha.to(2, 0) ) 
      if ( *(short *)alpha.ptr )  //ie textAlpha or haloAlpha is not 0
      {
        int textAlpha = alpha[0];
        int haloAlpha = alpha[1];
        int imageAlpha = 64 - textAlpha - haloAlpha;

        dst[0] = ( dst[0] * imageAlpha + Btext * textAlpha + Bhalo * haloAlpha ) >> 6;
        dst[1] = ( dst[1] * imageAlpha + Gtext * textAlpha + Ghalo * haloAlpha ) >> 6;
        dst[2] = ( dst[2] * imageAlpha + Rtext * textAlpha + Rhalo * haloAlpha ) >> 6;
      }
}



void Antialiaser::ApplyRGB32(VideoFrame& frame, int textColor, int haloColor) 
{
  int Rtext = ((textColor>>16)&255), Gtext = ((textColor>>8)&255), Btext = (textColor&255);
  int Rhalo = ((haloColor>>16)&255), Ghalo = ((haloColor>>8)&255), Bhalo = (haloColor&255);

  WindowPtr dst = frame.WriteTo(NOT_PLANAR);
  CWindowPtr alpha = alphaBuffer_.Read();

  assert( (dst.height == alpha.height && dst.width == alpha.width * 2) );

  for ( int y = alpha.height; y-- > 0; dst.pad(), alpha.pad() ) 
    for ( int x = alpha.width >> 1; x-- > 0; dst.to(4, 0), alpha.to(2, 0) ) 
      if ( *(short *)alpha.ptr )  //ie textAlpha or haloAlpha is not 0
      {
        int textAlpha = alpha[0];
        int haloAlpha = alpha[1];
        int imageAlpha = 64 - textAlpha - haloAlpha;

        dst[0] = ( dst[0] * imageAlpha + Btext * textAlpha + Bhalo * haloAlpha ) >> 6;
        dst[1] = ( dst[1] * imageAlpha + Gtext * textAlpha + Ghalo * haloAlpha ) >> 6;
        dst[2] = ( dst[2] * imageAlpha + Rtext * textAlpha + Rhalo * haloAlpha ) >> 6;
      }
}




void Antialiaser::ApplyYUY2(VideoFrame& frame, int textColor, int haloColor)
{
  int Ytext = ((textColor>>16)&255), Utext = ((textColor>>8)&255), Vtext = (textColor&255);
  int Yhalo = ((haloColor>>16)&255), Uhalo = ((haloColor>>8)&255), Vhalo = (haloColor&255);

  WindowPtr dst = frame.WriteTo(NOT_PLANAR);
  CWindowPtr alpha = alphaBuffer_.Read();

  for ( int y = alpha.height; y-- > 0; dst.pad(), alpha.pad() ) 
    for ( int x = alpha.width >> 2; x-- > 0; dst.to(4, 0), alpha.to(4, 0) ) 
      if ( *(int *)alpha.ptr ) 
      {
        dst[0] = (dst[0] * (64-alpha[0]-alpha[1]) + Ytext * alpha[0] + Yhalo * alpha[1]) >> 6;
        dst[2] = (dst[2] * (64-alpha[2]-alpha[3]) + Ytext * alpha[2] + Yhalo * alpha[3]) >> 6;
        int auv1 = alpha[0]+alpha[2];
        int auv2 = alpha[1]+alpha[3];
        dst[1] = (dst[1] * (128-auv1-auv2) + Utext * auv1 + Uhalo * auv2) >> 7;
        dst[3] = (dst[3] * (128-auv1-auv2) + Vtext * auv1 + Vhalo * auv2) >> 7;
      }
}




void Antialiaser::ApplyYV12(VideoFrame& frame, int textColor, int haloColor)
{
  int Ytext = ((textColor>>16)&255), Utext = ((textColor>>8)&255), Vtext = (textColor&255);
  int Yhalo = ((haloColor>>16)&255), Uhalo = ((haloColor>>8)&255), Vhalo = (haloColor&255);

  WindowPtr Y = frame.WriteTo(PLANAR_Y);
  WindowPtr U = frame.WriteTo(PLANAR_U);
  WindowPtr V = frame.WriteTo(PLANAR_V);
  CWindowPtr alpha = alphaBuffer_.Read();

  for ( int y = V.height; y-- > 0; Y.skipPad(), U.pad(), V.pad(), alpha.skipPad() ) 
    for ( int x = V.width; x--> 0; Y.to(2, 0), U.to(1, 0), V.to(1, 0), alpha.to(4, 0) ) 
      if ( *(int *)alpha.at(0, 0) || *(int *)alpha.at(0, 1) )
      {
        Y(0, 0) = ( Y(0, 0) * (64 - alpha(0, 0) - alpha(1, 0)) + Ytext * alpha(0, 0) + Yhalo * alpha(1, 0) ) >> 6;
        Y(1, 0) = ( Y(1, 0) * (64 - alpha(2, 0) - alpha(3, 0)) + Ytext * alpha(2, 0) + Yhalo * alpha(3, 0) ) >> 6;

        Y(0, 1) = ( Y(0, 1) * (64 - alpha(0, 1) - alpha(1, 1)) + Ytext * alpha(0, 1) + Yhalo * alpha(1, 1) ) >> 6;
        Y(1, 1) = ( Y(1, 1) * (64 - alpha(2, 1) - alpha(3, 1)) + Ytext * alpha(2, 1) + Yhalo * alpha(3, 1) ) >> 6;

        int auv1 = ( alpha(0, 0) + alpha(2, 0) + alpha(0, 1) + alpha(2, 1) ) >> 1;
        int auv2 = ( alpha(1, 0) + alpha(3, 0) + alpha(1, 1) + alpha(3, 1) ) >> 1;

        U[0] = ( U[0] * (128 - auv1 - auv2) + Utext * auv1 + Uhalo * auv2 ) >> 7;
        V[0] = ( V[0] * (128 - auv1 - auv2) + Vtext * auv1 + Vhalo * auv2 ) >> 7;
      }
}




void Antialiaser::UpdateAlpha()
{

  dirty_ = false;

  ByteMap const& bitcnt = aliaser::BitCount::instance;
  //ByteMap const& bitexr = aliaser::BitExpandRight::instance;
  //ByteMap const& bitexl = aliaser::BitExpandLeft::instance;

  CWindowPtr antialias = bitRenderer_.ReadBits();
  WindowPtr alpha = alphaBuffer_.Write();

  antialias.to(2, 16);    //skip edges guards


  for ( int y = alpha.height; y-- > 0; antialias.to(- alpha.width >> 1, 8), alpha.pad() ) 
    for( int x = alpha.width >> 1; x-- > 0; antialias.to(1, 0), alpha.to(2, 0) )
    {

      int text = 0, halo = 0;    
      
      int tmp = 0;            
      BYTE const * ptr = antialias.at(-1, - 8); 
      
      // check if the whole area isn't just plain black      
      for ( int i = 24; i-- > 0; ptr += antialias.pitch ) 
        tmp |= *(int const *)ptr;

      if ( (tmp & 0x00FFFFFF) != 0 )    // quick exit if all black
      {
        for ( int i = 0; i < 8; ++i )
          text += bitcnt[ antialias(0, i) ];

        if ( text != 0 )
          halo = 64 - text;
        else
        {
          BYTE centerMask = 0; 

          for( int i = 0; i < 8; ++i ) 
          {        
            //no need to consider the center case, we know it empty
            centerMask |= /*bitexl[*/ antialias(-1, i); // ];
            centerMask |= /*bitexr[*/ antialias( 1, i); // ];
          }

          if ( centerMask == 255 )
            halo = 64;
          else
          {
          
            BYTE mask = centerMask;

            BYTE bottomMasks[8]; // = { 255, 255, 255, 255, 255, 255, 255, 255 };
            BYTE topMasks[8]; // = { 255, 255, 255, 255, 255, 255, 255, 255 };

            for( int i = 8; i-- > 0; ) 
            {
              mask |= antialias(0, i - 8);
              mask |= /*bitexl[*/ antialias(-1, i - 8) << (7 - i); // ];
              mask |= /*bitexr[*/ antialias( 1, i - 8) >> (7 - i); // ];

              topMasks[i] = mask;
            }

            mask = centerMask;

            for( int i = 0; i < 8; ++i ) 
            {
              mask |= antialias(0, i + 8);
              mask |= /*bitexl[*/ antialias(-1, i + 8) << i; // ];
              mask |= /*bitexr[*/ antialias( 1, i + 8) >> i; // ];

              bottomMasks[i] = mask;
            }

            for( int i = 0; i < 8; ++i )
              halo += bitcnt[ topMasks[i] | bottomMasks[i] ];            
          
          }//if ( centerMask == 255 )
        }//if ( text != 0 )
      }//if ( tmp & 0x00FFFFFF != 0 ) 

      alpha[0] = text;
      alpha[1] = halo;
    }
}


} } //namespace avs::text
