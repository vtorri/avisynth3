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


#ifndef __AVS_FONTCONFIG_PATTERN_H__
#define __AVS_FONTCONFIG_PATTERN_H__

//boost include
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

// STL include
#include <string>

// fontconfig includes
#include <fontconfig/fontconfig.h>


namespace avs { namespace fontconfig {



/////////////////////////////////////////////////////////////////////////////////
//  fontconfig::Pattern
//
//  C++ wrapper around the FcPattern handle
//
class Pattern
{

  boost::shared_ptr<FcPattern> pattern_;  //underlying handle

public:  //structors

  Pattern();
  Pattern( FcPattern * pattern, bool withDestructor = false );

  //generated copy constructor and destructor are fine

public: //access

  FcPattern *GetPattern() { return pattern_.get(); }

private:

  int GetInteger( const char * property );
  boost::optional<std::string> GetString( const char * property );

public: //property read methods

  int GetWeight() { return GetInteger( FC_WEIGHT ); }
  int GetSlant() { return GetInteger( FC_SLANT ); }
  int GetSpacing() { return GetInteger( FC_SPACING ); }
  boost::optional<std::string> GetFamily() { return GetString( FC_FAMILY ); }
  boost::optional<std::string> GetStyle() { return GetString( FC_STYLE ); }
  boost::optional<std::string> GetFile() { return GetString( FC_FILE ); }

private:

  void SetPropertyInteger ( const char * property, int value );
  void SetPropertyBool ( const char * property, bool value );
  void SetPropertyString ( const char * property, std::string& value );

public: //property write methods

  void SetSlant( int slant ) { SetPropertyInteger( FC_SLANT, slant); }
  void SetWeight( int weight ) { SetPropertyInteger( FC_WEIGHT, weight); }
  void SetSpacing( int spacing ) { SetPropertyInteger( FC_SPACING, spacing); }
  void SetWidth( int width ) { SetPropertyInteger( FC_WIDTH, width); }
  void SetIndex( int index ) { SetPropertyInteger( FC_INDEX, index); }
  void SetOutline( bool outline ) { SetPropertyBool( FC_OUTLINE, outline); }
  void SetScalable( bool scalable ) { SetPropertyBool( FC_SCALABLE, scalable); }
  void SetStyle( std::string style ) { SetPropertyString( FC_STYLE, style); }

};



} } // namespace avs::fontconfig


#endif //__AVS_FONTCONFIG_PATTERN_H__
