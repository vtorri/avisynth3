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
#include "config.h"
#include "pattern.h"
#include "../core/exception/generic.h"


namespace {


// fontconfig initialisation
struct FontconfigInit
{
  FontconfigInit() { assert( FcInit() ); }
} initFontconfig;


struct ConfigDestructor
{
  void operator()( FcConfig *config ) const
  {
    FcConfigDestroy( config );
    FcFini();
  }
};

} // anonymous namespace


namespace avs { namespace fontconfig {



Config::Config()
{
  FcConfig  *config;

  config = FcInitLoadConfigAndFonts();
  assert ( config != NULL );

  fonts_ = FcConfigGetFonts( config, FcSetSystem );
  assert ( fonts_ != NULL );

  config_.reset( config, ConfigDestructor() );
}


boost::optional<std::string> Config::GetExactFontFilename( std::string const& fontName, bool bold, bool italic )
{
  boost::optional<std::string> filename;

  for ( int i = 0; i < fonts_->nfont; i++ ) {
    Pattern pattern( fonts_->fonts[i] );
    boost::optional<std::string> family = pattern.GetFamily();

    if ( family && ( (*family.get_ptr()).find( fontName ) != std::string::npos ) ) {
      boost::optional<std::string> style = pattern.GetStyle();
      char *requested_style = "Roman";
      int requested_spacing = FC_PROPORTIONAL;

      if (bold) {
        if (italic)
          requested_style = "Bold Oblique";
        else
          requested_style = "Bold";
      }
      else
        if (italic)
          requested_style = "Oblique";

//       if (italic)
//         requested_spacing = FC_CHARCELL;

      if ( style &&
           ( *style.get_ptr() == requested_style ) && 
           ( pattern.GetSpacing() == requested_spacing ) && 
           ( bold == ( pattern.GetWeight() == FC_WEIGHT_BOLD ) ) &&
           ( italic == ( pattern.GetSlant() == FC_SLANT_OBLIQUE ) ) ) {
        
        filename = pattern.GetFile();
        break;
      }
    }
  }

  return filename;
}


boost::optional<std::string> Config::GetApproximateFontFilename( bool bold, bool italic )
{
  boost::optional<std::string> filename;
  Pattern pattern;
  char *requested_style = "Roman";
  int requested_spacing = FC_PROPORTIONAL;

  if (bold) {
    if (italic)
      requested_style = "Bold Oblique";
    else
      requested_style = "Bold";
  }
  else
    if (italic)
      requested_style = "Oblique";

//   if (italic)
//     requested_spacing = FC_CHARCELL;

  pattern.SetSlant( italic ? FC_SLANT_OBLIQUE : FC_SLANT_ROMAN );
  pattern.SetWeight( bold ? FC_WEIGHT_BOLD : FC_WEIGHT_REGULAR );
  pattern.SetSpacing( requested_spacing );
  pattern.SetWidth( 100 );
  pattern.SetIndex( 0 );
  pattern.SetOutline( true );
  pattern.SetScalable( true );
  pattern.SetStyle( requested_style );

  FcDefaultSubstitute( pattern.GetPattern() );
  FcConfigSubstitute( config_.get(), pattern.GetPattern(), FcMatchPattern );

  FcPattern * result = FcFontMatch( config_.get(), pattern.GetPattern(), NULL );
  if ( result ) {
    Pattern p( result, true );
    filename = p.GetFile();
  }

  return filename;
}



} } // namespace avs::fontconfig
