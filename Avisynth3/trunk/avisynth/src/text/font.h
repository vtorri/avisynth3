// Avisynth v3.0 alpha.  Copyright 2004 Ben Rudiak-Gould et al.
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


#ifndef __AVS_TEXT_FONT_H__
#define __AVS_TEXT_FONT_H__

//avisynth include
#include "../core/dimension.h"

//boost include
#include <boost/shared_ptr.hpp>

//stl include
#include <string>

//windows includes
#include <windows.h>
#include <wingdi.h>               //for HFONT, CreateFont


namespace avs { namespace text {


class Font
{

  boost::shared_ptr<HFONT> pFont_;


public:  //structors

  Font(std::string const& name, int size, bool bold, bool italic);

  //generated copy constructor and destructor are fine


public:

  Dimension GetTextBoundingBox(std::string const& text);

  HFONT operator*() const { return *pFont_; }


private:  //HFONTDeleter struct

  struct HFONTDeleter
  {
    void operator()(void * pFont) const { DeleteObject( *(HFONT *)pFont ); }
  };

};


} } //namespace avs::text

#endif //__AVS_TEXT_FONT_H__