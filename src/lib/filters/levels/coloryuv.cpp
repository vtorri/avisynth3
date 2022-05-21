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
#include "../../core/colorspace/yuv/depth8.h"
#include "../../core/videoframe/yuv/depth8.h"
#include "../../core/exception/colorspace/unsupported.h"


namespace avs { namespace filters {



ColorYUV::ColorYUV(PClip const& child)
  : clip::onechild::Concrete( child )
{
  PColorSpace space = child->GetVideoInfo()->GetColorSpace();

  if ( ! boost::dynamic_pointer_cast<colorspace::yuv::Depth8 const>(space) )
    throw exception::colorspace::Unsupported(space);
}


void ColorYUV::Count(VideoFrame const& frame, ByteCounter& y, ByteCounter& u, ByteCounter& v) const
{
  dynamic_cast<videoframe::yuv::Depth8 const&>(frame).Count(y, u, v);
}


void ColorYUV::Apply(VideoFrame& frame, coloryuv::LumaMap const& y, coloryuv::ChromaMap const& u, coloryuv::ChromaMap const& v) const
{
  dynamic_cast<videoframe::yuv::Depth8&>(frame).Apply(y, u, v);
}



namespace { int identity(int value) { return value; }  }


using namespace coloryuv;


int (* ColorYUV::AdjustY(Mode mode))(int)
{
  switch ( mode )
  {
  case PCtoTV:      return &LumaMap::PCtoTVAdjust<65536, 76309>;
  case TVtoPC:
  case TVtoPCOnlyY: return &LumaMap::TVtoPCAdjust<65536, 76309>;

  default:          return &identity;
  }
}


int (* ColorYUV::AdjustU(Mode mode))(int)
{
  switch( mode )
  {
  case PCtoTV: return &ChromaMap::PCtoTVAdjust<116129, 132201>;
  case TVtoPC: return &ChromaMap::TVtoPCAdjust<116129, 132201>;

  default:     return &identity;
  }
}


int (* ColorYUV::AdjustV(coloryuv::Mode mode))(int)
{
  switch( mode )
  {
  case PCtoTV: return &ChromaMap::PCtoTVAdjust<91881, 104597>;
  case TVtoPC: return &ChromaMap::TVtoPCAdjust<91881, 104597>;

  default:     return &identity;
  }
}




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
