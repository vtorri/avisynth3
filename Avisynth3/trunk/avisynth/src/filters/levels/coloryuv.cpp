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
#include "coloryuv.h"
#include "coloryuv/lumamap.h"
#include "coloryuv/chromamap.h"
#include "../../core/videoinfo.h"
#include "../../core/colorspace.h"
#include "../../core/colorspace/yuv/depth8.h"
#include "../../core/exception/colorspace/unsupported.h"


namespace avs { namespace filters {

  

ColorYUV::ColorYUV(PClip const& child)
  : clip::onechild::Concrete( child )
{
  PColorSpace space = child->GetVideoInfo()->GetColorSpace();

  if ( ! boost::dynamic_pointer_cast<cspace::yuv::Depth8 const>(space) )
    throw exception::cspace::Unsupported(space);
}


void ColorYUV::Count(VideoFrame const& frame, ByteCounter& y, ByteCounter& u, ByteCounter& v) const
{
  boost::dynamic_pointer_cast<cspace::yuv::Depth8 const>(GetChildVideoInfo()->GetColorSpace())->Count(frame, y, u, v);
}


void ColorYUV::Apply(VideoFrame& frame, coloryuv::LumaMap const& y, coloryuv::ChromaMap const& u, coloryuv::ChromaMap const& v) const
{
  boost::dynamic_pointer_cast<cspace::yuv::Depth8 const>(GetChildVideoInfo()->GetColorSpace())->Apply(frame, y, u, v);
}


/*  FrameMaker * maker 
    = (autoWhite || autoGain) ? new coloryuv::framemaker::Auto(*this, Y, U, V, gamma, mode, coring, autoWhite, autoGain)
                              : static_cast<FrameMaker *>(new coloryuv::framemaker::Standard(*this, Y, U, V, gamma, mode, coring));

  SetFrameMaker( child->GetEnvironment(), maker );*/






/*
ColorYUV::ColorYUV(Clip const& child, InputParameters const& Y, InputParameters const& U, InputParameters const& V
                  , Levels levels, bool coring, bool analyze, bool autowhite, bool autogain)
  : clip::onechild::Concrete( child )
  , clip::caching::Concrete( child->GetEnvironment() )
  , yIParam_( Y )
  , uIParam_( U )
  , vIParam_( V )
  , levels_( levels )
  , coring_( coring )
{
    
    //Set boolean processing parameters
    analyze=_analyze;
    autowhite=_autowhite;
    autogain=_autogain;
    
    MakeGammaLUT();

    pixels = GetChildVideoInfo()->GetWidth()*
      GetChildVideoInfo()->GetHeight();
  }
  
  
  void ColorYUV::MakeGammaLUT() const
  {
    static const int scale = 256, shift = 2^10,
      coeff_y0 =  76309, coeff_y1 =  65536,
      coeff_u0 = 132201, coeff_u1 = 116129,
      coeff_v0 = 104597, coeff_v1 =  91881;
    int val;
    float g, b, c, gain;
    
    yPParam_.thresh1 = uPParam_.thresh1 = vPParam_.thresh1 = -1;
    yPParam_.thresh2 = uPParam_.thresh2 = vPParam_.thresh2 = 256;
    
    // Set luma
    gain = (boost::get<0>(yIParam_) + scale) / scale;
    c = (boost::get<1>(yIParam_) + scale) / scale;
    b = (boost::get<2>(yIParam_) + scale) / scale;
    g = (boost::get<3>(yIParam_) + scale) / scale;
    if ( g < 0.01 ) 
      g = 0.01;

    for ( int i = 0; i < 256; ++i )
    {
      val = i * shift;
      
      switch ( levels_ )
      {
      case PCtoTV:
        val = (int)((val - 16 * shift) * coeff_y0 / coeff_y1 + shift / 2);
        break;
      case TVtoPC:
      case TVtoPConlyY:
        val = (int)(val * coeff_y1 / coeff_y0 + 16 * shift + shift / 2);
        break;
      default:	//none
        break;
      }
      val = val / shift;
      
      float v = static_cast<float>(val) / 256.0f;
      v = (v * gain) + ((v-0.5) * c + 0.5) - v + (b - 1);
      
      if (boost::get<3>(yIParam_) != 0 && v > 0) v = pow( v, 1 / g);
      v = v * 256;
      
      v += 0.5;
      val = int(floor(v));
      
      val = saturate<BYTE>(val);
      
      if (val > 235)
      {
        if (yPParam_.thresh2 > 255) yPParam_.thresh2 = i;
        if (opt) val = 235;
      }
      else if (val < 16)
      {
        yPParam_.thresh1 = i;
        if (opt) val = 16;
      }
      yPParam_.LUT[i] = static_cast<unsigned char>(val);
    }

    gain = boost::get<0>(uIParam_) + scale;
    c = boost::get<1>(uIParam_) + scale;
    b = boost::get<2>(uIParam_);
    for (i = 0; i < 256; i++)
    {
      val = i * shift;
      switch (levels)
      {
      case 1:	// PC->TV Scale
        val = int((val - 128 * shift) * coeff_u0 / coeff_u1 + 128 * shift + shift / 2);
        break;
      case 2:	// TV->PC Scale
        val = int((val - 128 * shift) * coeff_u1 / coeff_u0 + 128 * shift + shift / 2);
        break;
      default:	//none
        break;
      }
      val = val / shift;
      
      v = float(val);
      v = (v * gain / scale) + ((v-128) * c / scale + 128) - v + b;
    
      v += 0.5;
      val = int(floor(v));
    
      if (val > 255) val = 255;
      else if (val < 0) val = 0;

      if (val > 240)
      {
        if (uPParam_.thresh2 > 255) uPParam_.thresh2 = i;
        if (opt) val = 240;
      }
      else if (val < 16)
      {
        uPParam_.thresh1 = i;
        if (opt) val = 16;
      }
      uPParam_.LUT[i] = static_cast<unsigned char>(val);
    }

    gain = boost::get<0>(vIParam_) + scale;
    c = boost::get<1>(vIParam_) + scale;
    b = boost::get<2>(vIParam_);
    for (i = 0; i < 256; i++)
    {
      val = i * shift;
      switch (levels)
      {
      case 1:	// PC->TV Scale
        val = int((val - 128 * shift) * coeff_v0 / coeff_v1 + 128 * shift + shift / 2);
        break;
      case 2:	// TV->PC Scale
        val = int((val - 128 * shift) * coeff_v1 / coeff_v0 + 128 * shift + shift / 2);
        break;
      default:	//none
        break;
      }
      val = val / shift;
      
      v = float(val);
      v = (v * gain / scale) + ((v-128) * c / scale + 128) - v + b;
      
      v += 0.5;
      val = int(floor(v));
      
      if (val > 255) val = 255;
      else if (val < 0) val = 0;
      
      if (val > 240)
      {
        if (vPParam_.thresh2 > 255) vPParam_.thresh2 = i;
        if (opt) val = 240;
      }
      else if (val < 16)
      {
        vPParam_.thresh1 = i;
        if (opt) val = 16;
      }
      vPParam_.LUT[i] = static_cast<unsigned char>(val);
    }
  }
*/
/*  bool ColorYUV::CheckParms(std::string const& _levels,
                            std::string const& _matrix,
                            std::string const& _opt)
  {
    int i;
    //Could use a vector here
    std::string
      LevelsTbl[] = { "", "TV->PC", "PC->TV", "PC->TV.Y" },
      MatrixTbl[] = { "", "rec.709" },
        OptTbl[] = { "", "coring" };
        
    levels = -1;
    if (!_levels.empty())
    {
      for (i=0; i<4 ; i++)
      {
        if (NoCaseEqual(_levels, LevelsTbl[i])) 
        {
          levels = i;
          break;
        }
      }
    }
    else levels = 0;
    
    matrix = -1;
    if (!_matrix.empty())
    {
      for (i=0; i<2 ; i++)
      {
        if (NoCaseEqual(_matrix, MatrixTbl[i])) 
        {
          matrix = i;
          break;
        }
      }
    }
    else matrix = 0;

    opt = -1;
    if (!_opt.empty())
    {
      for (i=0; i<2 ; i++)
      {
        if (NoCaseEqual(_opt, OptTbl[i])) 
        {
          opt = i;
          break;
        }
      }
    }
    else opt = 0;
    
    if ( levels < 0 || matrix < 0 || opt < 0 ) return false;
    return true;
  }
*/


} } //avs::filters
