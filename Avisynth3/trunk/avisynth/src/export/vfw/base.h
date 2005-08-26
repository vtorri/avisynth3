// Avisynth v3.0 alpha.  Copyright 2003-2005 David Pierre - Ben Rudiak-Gould et al.
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


#ifndef __AVS_EXPORT_VFW_BASE_H__
#define __AVS_EXPORT_VFW_BASE_H__

//avisynth include
#include "../../core/utility/instancecounter.h"

//windows includes
#ifndef NOMINMAX
#define NOMINMAX          //prevents generation of min and max macros
#endif //NOMINMAX
#include <initguid.h>
#include <objbase.h>


namespace avs { namespace export { namespace vfw {


//declaration and typedef
struct Avs3Tag { };
typedef instance_counted<Avs3Tag> InstanceCounter;



//////////////////////////////////////////////////////////////////////////////
//  Base
//
//  a base class for implementations of COM interfaces
//
class Base
{

  unsigned long refCount_;
  InstanceCounter counter_;


public:  //structors

  Base() : refCount_( 1 ) { }
  Base(Base const&) : refCount_( 1 ) { }

  virtual ~Base() { }


protected:  //IUnknown like

  unsigned long AddRef_() 
  { 
    return ++refCount_; 
  }
	
  unsigned long Release_()
  {
    unsigned long ref = --refCount_;
    if ( ref == 0 )
      delete this;
    return ref;
  }
};



} } } //namespace avs::export::vfw

#endif //__AVS_EXPORT_VFW_BASE_H__
