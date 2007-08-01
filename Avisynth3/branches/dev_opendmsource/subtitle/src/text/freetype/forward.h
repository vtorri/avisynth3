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


#ifndef __AVS_TEXT_FREETYPE_FORWARD_H__
#define __AVS_TEXT_FREETYPE_FORWARD_H__


//declaration of shared_ptr
namespace boost { template <typename T> class shared_ptr; }


namespace avs { 


//declarations
template <typename T> class box;
template <typename T> class vecteur;
template <typename T> class dimension;
template <int fractionPart> class fixed_point;  

  
namespace text { 
  

//another one
namespace rasterizer { class OutlineSplitter; }
  

namespace freetype {


//more declarations
class Face;
class Outline;
class TextWalker;


//typedefs
typedef fixed_point<6> FP6;
typedef box<FP6> BoxFP6;
typedef vecteur<FP6> VecteurFP6;
typedef dimension<FP6> DimensionFP6;
typedef boost::shared_ptr<Face> PFace;


} } } //namespace avs::text::freetype

#endif //__AVS_TEXT_FREETYPE_FORWARD_H__
