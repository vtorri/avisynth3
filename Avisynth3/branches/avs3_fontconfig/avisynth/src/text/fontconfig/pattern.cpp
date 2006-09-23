// Avisynth v3.0 alpha.  Copyright 2006 David Pierre - Ben Rudiak-Gould et al.
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


//avisynth includes
#include "pattern.h"


namespace {


struct PatternDestructor
{
  void operator()( FcPattern *pattern ) const { }
};


} // anonymous namespace


namespace avs { namespace fontconfig {



Pattern::Pattern()
{
  FcPattern *pattern = FcPatternCreate();
  assert( pattern != NULL );
  pattern_.reset( pattern, &FcPatternDestroy );
}


Pattern::Pattern( FcPattern * pattern, bool withDestructor )
{
  assert( pattern != NULL );
  if ( withDestructor )
    pattern_.reset( pattern, &FcPatternDestroy );
  else
    pattern_.reset( pattern, PatternDestructor() );
}


int Pattern::GetInteger( const char * property )
{
  int i = 0;

  if ( FcPatternGetInteger( pattern_.get(), property, 0, &i ) == FcResultTypeMismatch )
    i = -1;
  return i;
}


boost::optional<std::string> Pattern::GetString( const char * property )
{
  FcChar8 *str;
  boost::optional<std::string> result;

  if ( FcPatternGetString( pattern_.get(), property, 0, &str ) != FcResultTypeMismatch )
    result = std::string( (char *)str );

  return result;
}


void Pattern::SetPropertyInteger( const char * property, int value )
{
  FcPatternAddInteger( pattern_.get(), property, value );
}


void Pattern::SetPropertyBool( const char * property, bool value )
{
  FcPatternAddBool( pattern_.get(), property, value );
}


void Pattern::SetPropertyString( const char * property, std::string& value )
{
  FcPatternAddString( pattern_.get(), property, (FcChar8 *)value.c_str() );
}



} } // namespace avs::fontconfig
