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


#ifndef __AVS_FILTERS_EDITFILTER_H__
#define __AVS_FILTERS_EDITFILTER_H__


//avisynth includes
#include "childclip.h"
#include "../videoinfo.h"


namespace avs { namespace filters {



//////////////////////////////////////////////////////////////////////////////////////////
// EditFilter
//
// instanciable null filter forwarding to child, without a cache
// but with a modified videoinfo
//
class EditFilter : public ChildClip
{

  VideoInfo vi;


public:  //constructor

  EditFilter(PClip child)
    : ChildClip( child )
    , vi( GetChild()->GetVideoInfo() ) { }    


public:  //clip general interface

  virtual const VideoInfo& GetVideoInfo() const { return vi; }


protected:  //access

  VideoInfo& GetVideoInfo() { return vi; }

};



}; }; //namespace avs::filters

#endif //__AVS_FILTERS_EDITFILTER_H__