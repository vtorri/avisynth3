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
#include "face.h"
#include "library.h"
#include "../core/exception/generic.h"

// freetype includes
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

//assert include
#include <assert.h>


namespace avs { namespace freetype {



Face::Face(std::string const& fileName, int index)
{
  FT_Face face = NULL;

  if ( FT_New_Face(Library::instance, fileName.c_str(), index, &face) != 0 )
    throw avs::exception::Generic ("Error while creating a Face.");

  face_.reset(face, &FT_Done_Face);  
}
  
unsigned Face::GetCharIndex(unsigned charCode)
{
  return FT_Get_Char_Index(face_.get(), charCode);
}


bool Face::HasKerning() const
{
  return FT_HAS_KERNING(face_.get()) != 0;
}


Vecteur Face::GetKerning(unsigned leftGlyph, unsigned rightGlyph) const
{
  FT_Vector result;
  FT_Error error = FT_Get_Kerning(face_.get(), leftGlyph, rightGlyph, FT_KERNING_DEFAULT, &result);

  assert( error == 0 );

  return Vecteur( result.x >> 6, result.y >>6);
}

  
void Face::SetCharSize (Dimension const& size, Dimension const& resolution)  
{   
  FT_Error error = FT_Set_Char_Size(face_.get(), size.GetWidth(), size.GetHeight()
                                               , resolution.GetWidth(), resolution.GetHeight());
  assert( error == 0 );    //assuming it normally always suceeds
}


Glyph Face::GetGlyph(unsigned glyphIndex)
{
  FT_Glyph glyph;
  if ( FT_Load_Glyph(face_.get(), glyphIndex, FT_LOAD_DEFAULT) != 0 )
    throw avs::exception::Generic("Error while loading a glyphslot in a Face.");
  
  if ( FT_Get_Glyph(face_.get()->glyph, &glyph) != 0 )
    throw avs::exception::Generic("Error while retrieving a glyph from a glyphslot.");
  
  return Glyph(glyph);
}

/*
  void Face::set_pixel_sizes (FT_UInt pixel_width,
			      FT_UInt pixel_height)
  {
    FT_Error error;
    
    error = FT_Set_Pixel_Sizes (face, pixel_width, pixel_height);
    
    if (error)
      throw avs::exception::Generic ("Error while setting the character dimensions of a Face.");
  }
    
  void Face::load_glyph (FT_UInt  glyph_index, FT_Int32 load_flags)
  {
    FT_Error error;
    
    error = FT_Load_Glyph (face, glyph_index, load_flags);
			   
    if (error)
      throw avs::exception::Generic ("Error while loading a glyph from a Face.");
  }

  void Face::load_char (FT_ULong char_code, FT_Int32 load_flags)
  {
    FT_Error error;
    
    error = FT_Load_Char (face, char_code, load_flags);
			   
    if (error)
      throw avs::exception::Generic ("Error while loading a glyph from a Face.");
  }
    
  void Face::set_transform (FT_Matrix *matrix, FT_Vector* delta)
  {
    FT_Set_Transform (face, matrix, delta);
  }
    
  void Face::get_kerning (FT_UInt    left_glyph,
			  FT_UInt    right_glyph,
			  FT_UInt    kern_mode,
			  FT_Vector *kerning)
  {
    FT_Error error;
    
    error = FT_Get_Kerning (face,
			    left_glyph, right_glyph,
			    kern_mode, kerning);
			   
    if (error)
      throw avs::exception::Generic ("Error while returning kerning vector from a Face.");
  }

  void Face::get_glyph_name (FT_UInt    glyph_index,
			     FT_Pointer buffer,
			     FT_UInt    buffer_max)
  {
    FT_Error error;
    
    error = FT_Get_Glyph_Name (face,
			       glyph_index, buffer, buffer_max);
			   
    if (error)
      throw avs::exception::Generic ("Error while retrieving ASCII name of a glyph in a Face.");
  }

  const std::string Face::get_postscript_name ()
  {
    std::string S(FT_Get_Postscript_Name(face));

    return S;
  }
    
  void Face::select_charmap (FT_Encoding encoding)
  {
    FT_Error error;
    
    error = FT_Select_Charmap (face, encoding);
			   
    if (error)
      throw avs::exception::Generic ("Error while selecting a charmap in a Face.");
  }
    
  void Face::set_charmap (FT_CharMap charmap)
  {
    FT_Error error;
    
    error = FT_Set_Charmap (face, charmap);
			   
    if (error)
      throw avs::exception::Generic ("Error while selecting a charmap for character code in a Face.");
  }



  FT_ULong Face::get_first_char (FT_UInt *gindex)
  {
    FT_ULong index;

    index = FT_Get_First_Char (face, gindex);

    return index;
  }

  FT_ULong Face::get_next_char (FT_ULong char_code,
				FT_UInt *gindex)
  {
    FT_ULong index;

    index = FT_Get_Next_Char (face, char_code, gindex);

    return index;
  }

  FT_UInt Face::get_name_index (FT_String *glyph_name)
  {
    FT_UInt index;

    index = FT_Get_Name_Index (face, glyph_name);
			   
    if (!index)
      throw avs::exception::Generic ("Error while returning the glyph index a glyph in a Face.");

    return index;
  }

  // Multiple Master Fonts

  void Face::get_mm (FT_Multi_Master *master)
  {
    FT_Error error;
    
    error = FT_Get_Multi_Master (face, master);
			   
    if (error)
      throw avs::exception::Generic ("Error while retrieving Multiple Master descriptor.");
  }

  void Face::set_mm_design_coord (FT_UInt  num_coords,
				  FT_Long *coords)
  {
    FT_Error error;
    
    error = FT_Set_MM_Design_Coordinates (face, num_coords, coords);
			   
    if (error)
      throw avs::exception::Generic ("Error while choosing interpolated font.");
  }
    
  void Face::set_mm_blend_coord (FT_UInt  num_coords,
				 FT_Long *coords)
  {
    FT_Error error;
    
    error = FT_Set_MM_Blend_Coordinates (face, num_coords, coords);
			   
    if (error)
      throw avs::exception::Generic ("Error while choosing interpolated font.");
  }
    
  // True Type Tables

  void *Face::get_sfnt_table (FT_Sfnt_Tag tag)
  {
    return FT_Get_Sfnt_Table (face, tag);
  }

  void Face::load_sfnt_table (FT_ULong  tag,
			      FT_Long   offset,
			      FT_Byte*  buffer,
			      FT_ULong* length)
  {
    FT_Error error;
    
    error = FT_Load_Sfnt_Table (face, 
				tag, offset, buffer, length);
			   
    if (error)
      throw avs::exception::Generic ("Error while font table in memory.");
  }
    
  // Type 1 Tables

  FT_Int Face::has_ps_glyph_names (void)
  {
    FT_Int test;

    test = FT_Has_PS_Glyph_Names(face);

    return test;
  }
  
  void Face::get_ps_font_info (PS_FontInfoRec *font_info)
  {
    FT_Get_PS_Font_Info(face,
			font_info);
  }

  // SFNT Names
  
  FT_UInt Face::get_sfnt_name_count (void)
  {
    FT_UInt nbr_str;
    
    nbr_str = FT_Get_Sfnt_Name_Count (face);

    return nbr_str;
  }
  
  void Face::get_sfnt_name (FT_UInt      idx,
			    FT_SfntName *name)
  {
    FT_Error error;
    
    error = FT_Get_Sfnt_Name (face, idx, name);
			   
    if (error)
      throw avs::exception::Generic ("Error while retrieving the SFNT 'name' table.");
  }
    
  // BDF Fonts

  BDFCharsetID Face::get_bdf_charset_id()
  {
    FT_Error error;
    const char *encoding, *registry;
    
    error = FT_Get_BDF_Charset_ID (face, &encoding, &registry);

    if (error)
      throw avs::exception::Generic ("Error while retrieving the BDF font character set identity.");
    
    return BDFCharsetID(encoding, registry);
  }
  
  void Face::get_bdf_property (const std::string prop_name,
			       BDF_PropertyRec  *property)
  {
    FT_Error error;
    
    error = FT_Get_BDF_Property (face, prop_name.c_str(), property);
			   
    if (error)
      throw avs::exception::Generic ("Error while retrieving the BDF property from BDF or PCF Face.");
  }

  // PFR Fonts

  void Face::get_pfr_metrics (FT_UInt   *outline_resolution,
			      FT_UInt   *metrics_resolution,
			      FT_Fixed  *metrics_x_scale,
			      FT_Fixed  *metrics_y_scale)
  {
    FT_Error error;
    
    error = FT_Get_PFR_Metrics (face,
				outline_resolution, metrics_resolution,
				metrics_x_scale, metrics_y_scale);
			   
    if (error)
      throw avs::exception::Generic ("Error while retrieving the outline and metrics of a PFR Face.");
  }
    
  void Face::get_pfr_kerning (FT_UInt    left,
			      FT_UInt    right,
			      FT_Vector *avector)
  {
    FT_Error error;
    
    error = FT_Get_PFR_Kerning (face, left, right, avector);
			   
    if (error)
      throw avs::exception::Generic ("Error while retrieving the kerning pair of two glyph in a PFR Face.");
  }

  void Face::get_pfr_advance (FT_UInt    gindex,
			      FT_Pos    *advance)
  {
    FT_Error error;
    
    error = FT_Get_PFR_Advance (face, gindex, advance);
			   
    if (error)
      throw avs::exception::Generic ("Error while retrieving the glyph advance from a PFR Face.");
  }

  // Windows FNT Fonts

  void Face::get_winfnt_header (FT_WinFNT_HeaderRec *header)
  {
    FT_Error error;
    
    error = FT_Get_WinFNT_Header (face, header);
			   
    if (error)
      throw avs::exception::Generic ("Error while retrieving Windows font info header.");
  }*/

} } // namespace avs::freetype
