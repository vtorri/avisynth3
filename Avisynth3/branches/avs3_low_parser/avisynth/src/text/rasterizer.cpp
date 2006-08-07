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


//avisynth includes
#include "rasterizer.h"
#include "../core/colorspace.h"
#include "../core/videoframe.h"
#include "freetype/textwalker.h"
#include "rasterizer/spanmap.h"
#include "rasterizer/outlinesplitter.h"
#include "../core/exception/colorspace/unsupported.h"


namespace avs { namespace text { namespace freetype { namespace {



VecteurFP6 GetOrigin(VecteurFP6 pen, BoxFP6 const& box, Align align)
{
  pen -= box.GetPosition();

  switch( align )
  {
  //case BOTTOM_LEFT:   return pen;
  case BOTTOM_CENTER: return pen + VecteurFP6( -box.GetDimension().GetWidth() >> 1, 0 );
  case BOTTOM_RIGHT:  return pen + VecteurFP6( -box.GetDimension().GetWidth(), 0 );
  case CENTER_LEFT:   return pen + VecteurFP6( 0, -box.GetDimension().GetHeight() >> 1 );
  case CENTER:        return pen + VecteurFP6( -box.GetDimension().GetWidth() >> 1, -box.GetDimension().GetHeight() >> 1 );
  case CENTER_RIGHT:  return pen + VecteurFP6( -box.GetDimension().GetWidth(), -box.GetDimension().GetHeight() >> 1 );
  case TOP_LEFT:      return pen + VecteurFP6( 0, -box.GetDimension().GetHeight() );
  case TOP_CENTER:    return pen + VecteurFP6( -box.GetDimension().GetWidth(), -box.GetDimension().GetHeight() );
  case TOP_RIGHT:     return pen + VecteurFP6( -box.GetDimension().GetWidth(), -box.GetDimension().GetHeight() );

  default:  assert( align == BOTTOM_LEFT );
            return pen;
  }
}


} } //namespace freetype::anonymous



void Rasterizer::operator()(std::string const& text, WindowPtr const& masks, freetype::VecteurFP6 const& pen, Align align) const
{
  rasterizer::OutlineSplitter splitter;
  freetype::TextWalker walk( font_.GetFace(), freetype::GetOrigin(pen, font_.GetTextBoundingBox(text), align) );

  walk( text.begin(), text.end(), splitter );

  rasterizer::SpanMap textMap( splitter.GetSpanMaker() );
  rasterizer::SpanMap haloMap = textMap.Thicken( radius_ );
  haloMap.Remove(textMap);

  textMap.Realize( masks, 2 );
  haloMap.Realize( WindowPtr(masks) += 1, 2 );
}




void Rasterizer::ApplyTo(CWindowPtr const& masks, int textColor, int haloColor, VideoFrame& frame)
{
  PColorSpace space = frame.GetColorSpace();

  switch( space->id() )
  {
  case ColorSpace::I_RGB24: ApplyRGB24(masks, textColor, haloColor, frame); break;
  case ColorSpace::I_RGB32: ApplyRGB32(masks, textColor, haloColor, frame); break;
  case ColorSpace::I_YUY2: ApplyYUY2(masks, textColor, haloColor, frame); break;
  case ColorSpace::I_YV12: ApplyYV12(masks, textColor, haloColor, frame); break;
  
  default: throw exception::colorspace::Unsupported(space);
  }
}



void Rasterizer::ApplyRGB24(CWindowPtr masks, int textColor, int haloColor, VideoFrame& frame) 
{
  int Rtext = ((textColor>>16)&255), Gtext = ((textColor>>8)&255), Btext = (textColor&255);
  int Rhalo = ((haloColor>>16)&255), Ghalo = ((haloColor>>8)&255), Bhalo = (haloColor&255);
  
  WindowPtr dst = frame.WriteTo('~');

  for ( int y = masks.height; y-- > 0; dst.pad(), masks.pad() ) 
    for ( int x = masks.width >> 1; x-- > 0; dst.to(3, 0), masks.to(2, 0) ) 
      if ( *(short *)masks.ptr != 0 )     //ie textAlpha or haloAlpha is not 0
      {
        int textAlpha = masks[0];
        int haloAlpha = masks[1];
        int imageAlpha = 64 - textAlpha - haloAlpha;

        dst[0] = ( dst[0] * imageAlpha + Btext * textAlpha + Bhalo * haloAlpha ) >> 6;
        dst[1] = ( dst[1] * imageAlpha + Gtext * textAlpha + Ghalo * haloAlpha ) >> 6;
        dst[2] = ( dst[2] * imageAlpha + Rtext * textAlpha + Rhalo * haloAlpha ) >> 6;
      }
}



void Rasterizer::ApplyRGB32(CWindowPtr masks, int textColor, int haloColor, VideoFrame& frame) 
{
  int Rtext = ((textColor>>16)&255), Gtext = ((textColor>>8)&255), Btext = (textColor&255);
  int Rhalo = ((haloColor>>16)&255), Ghalo = ((haloColor>>8)&255), Bhalo = (haloColor&255);

  WindowPtr dst = frame.WriteTo('~');

  for ( int y = masks.height; y-- > 0; dst.pad(), masks.pad() ) 
    for ( int x = masks.width >> 1; x-- > 0; dst.to(4, 0), masks.to(2, 0) ) 
      if ( *(short *)masks.ptr != 0 )  //ie textAlpha or haloAlpha is not 0
      {
        int textAlpha = masks[0];
        int haloAlpha = masks[1];
        int imageAlpha = 64 - textAlpha - haloAlpha;

        dst[0] = ( dst[0] * imageAlpha + Btext * textAlpha + Bhalo * haloAlpha ) >> 6;
        dst[1] = ( dst[1] * imageAlpha + Gtext * textAlpha + Ghalo * haloAlpha ) >> 6;
        dst[2] = ( dst[2] * imageAlpha + Rtext * textAlpha + Rhalo * haloAlpha ) >> 6;
      }
}




void Rasterizer::ApplyYUY2(CWindowPtr masks, int textColor, int haloColor, VideoFrame& frame)
{
  int Ytext = ((textColor>>16)&255), Utext = ((textColor>>8)&255), Vtext = (textColor&255);
  int Yhalo = ((haloColor>>16)&255), Uhalo = ((haloColor>>8)&255), Vhalo = (haloColor&255);

  WindowPtr dst = frame.WriteTo('~');

  for ( int y = masks.height; y-- > 0; dst.pad(), masks.pad() ) 
    for ( int x = masks.width >> 2; x-- > 0; dst.to(4, 0), masks.to(4, 0) ) 
      if ( *(int *)masks.ptr ) 
      {
        dst[0] = (dst[0] * (64-masks[0]-masks[1]) + Ytext * masks[0] + Yhalo * masks[1]) >> 6;
        dst[2] = (dst[2] * (64-masks[2]-masks[3]) + Ytext * masks[2] + Yhalo * masks[3]) >> 6;
        int auv1 = masks[0]+masks[2];
        int auv2 = masks[1]+masks[3];
        dst[1] = (dst[1] * (128-auv1-auv2) + Utext * auv1 + Uhalo * auv2) >> 7;
        dst[3] = (dst[3] * (128-auv1-auv2) + Vtext * auv1 + Vhalo * auv2) >> 7;
      }
}




void Rasterizer::ApplyYV12(CWindowPtr masks, int textColor, int haloColor, VideoFrame& frame)
{
  int Ytext = ((textColor>>16)&255), Utext = ((textColor>>8)&255), Vtext = (textColor&255);
  int Yhalo = ((haloColor>>16)&255), Uhalo = ((haloColor>>8)&255), Vhalo = (haloColor&255);

  WindowPtr Y = frame.WriteTo('Y');
  WindowPtr U = frame.WriteTo('U');
  WindowPtr V = frame.WriteTo('V');

  for ( int y = V.height; y-- > 0; Y.skipPad(), U.pad(), V.pad(), masks.skipPad() ) 
    for ( int x = V.width; x--> 0; Y.to(2, 0), U.to(1, 0), V.to(1, 0), masks.to(4, 0) ) 
      if ( *(int *)masks.at(0, 0) || *(int *)masks.at(0, 1) )
      {
        Y(0, 0) = ( Y(0, 0) * (64 - masks(0, 0) - masks(1, 0)) + Ytext * masks(0, 0) + Yhalo * masks(1, 0) ) >> 6;
        Y(1, 0) = ( Y(1, 0) * (64 - masks(2, 0) - masks(3, 0)) + Ytext * masks(2, 0) + Yhalo * masks(3, 0) ) >> 6;

        Y(0, 1) = ( Y(0, 1) * (64 - masks(0, 1) - masks(1, 1)) + Ytext * masks(0, 1) + Yhalo * masks(1, 1) ) >> 6;
        Y(1, 1) = ( Y(1, 1) * (64 - masks(2, 1) - masks(3, 1)) + Ytext * masks(2, 1) + Yhalo * masks(3, 1) ) >> 6;

        int auv1 = ( masks(0, 0) + masks(2, 0) + masks(0, 1) + masks(2, 1) ) >> 1;
        int auv2 = ( masks(1, 0) + masks(3, 0) + masks(1, 1) + masks(3, 1) ) >> 1;

        U[0] = ( U[0] * (128 - auv1 - auv2) + Utext * auv1 + Uhalo * auv2 ) >> 7;
        V[0] = ( V[0] * (128 - auv1 - auv2) + Vtext * auv1 + Vhalo * auv2 ) >> 7;
      }
}



} } //namespace avs::text
