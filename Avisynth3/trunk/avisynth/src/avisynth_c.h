/* Avisynth 3.0 C Interface 
 * Copyright 2005 Vincent Torri <vtorri at univ-evry dot fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA, or visit
 * http://www.gnu.org/copyleft/gpl.html .
 *
 * As a special exception, I give you permission to link to the
 * Avisynth C interface with independent modules that communicate with
 * the Avisynth C interface solely through the interfaces defined in
 * avisynth_c.h, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting combined work
 * under terms of your choice, provided that every copy of the
 * combined work is accompanied by a complete copy of the source code
 * of the Avisynth C interface and Avisynth itself (with the version
 * used to produce the combined work), being distributed under the
 * terms of the GNU General Public License plus this exception.  An
 * independent module is a module which is not derived from or based
 * on Avisynth C Interface, such as 3rd-party filters, import and
 * export plugins, or graphical user interfaces.
 */

#ifndef __AVISYNTH_3_0_C_H__
#define __AVISYNTH_3_0_C_H__

/******************************************************************
 * extern stuff
 ******************************************************************/


/* Always use C linkage */
#ifdef __cplusplus
extern "C" 
{
#endif

/******************************************************************
 * Constants
 ******************************************************************/

#ifndef __AVISYNTH_H__
enum { AVISYNTH_INTERFACE_VERSION = 2 };
#endif

#if !defined(__ANSI__) && !defined(__cplusplus)
#  define HAVE_BOOL
typedef enum { false=0, true=1 } bool;
#endif


/* #include "avisynth_c/blitter_c.h" */
#include "avisynth_c/colorspace_c.h"
#include "avisynth_c/clip_c.h"
#include "avisynth_c/runtime_environment_c.h"
#include "avisynth_c/videoframe_c.h"
#include "avisynth_c/videoinfo_c.h"


#ifdef __cplusplus
}
#endif


#endif /* __AVISYNTH_3_0_C_H__ */
