// Avisynth v3.0 alpha.  Copyright 2003-2006 David Pierre - Ben Rudiak-Gould et al.
// Copyleft 2006 Vincent Torri
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


#ifndef __AVS_FONTCONFIG_CONFIG_H__
#define __AVS_FONTCONFIG_CONFIG_H__

//boost include
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

// STL include
#include <string>

// fontconfig includes
#include <fontconfig/fontconfig.h>


namespace avs { namespace fontconfig {



/////////////////////////////////////////////////////////////////////////////////
//  fontconfig::Config
//
//  C++ wrapper around the FcConfig handle
//
class Config
{

  boost::shared_ptr<_FcConfig> config_;  //underlying handle
  FcFontSet *fonts_;

public:  //structors

  Config();

  //generated copy constructor and destructor are fine

private:

  boost::optional<std::string> GetExactFontFilename ( std::string const& fontName, bool bold = false, bool italic = false );
  boost::optional<std::string> GetApproximateFontFilename( bool bold = false, bool italic = false );

public:

  boost::optional<std::string> GetFontFilename ( std::string const& fontName, bool bold = false, bool italic = false )
    {
      boost::optional<std::string> name = GetExactFontFilename(fontName, bold, italic);
      if (!name)
        name = GetApproximateFontFilename(fontName, bold, italic);
      return name;
    }
};



} } // namespace avs::fontconfig


#endif //__AVS_FONTCONFIG_CONFIG_H__
