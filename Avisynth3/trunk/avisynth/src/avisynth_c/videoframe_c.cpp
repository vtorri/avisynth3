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


// C API include
#include "define_c.h"
#include "private/videoframe_c_private.h"

// avisynth includes
#include "../core/blitter.h"
#include "../core/runtime_environment.h"

// Destructor

EXTERN_C void
avs_videoframe_delete (AVS_VideoFrame *p_vf)
{
  if (!p_vf) return;

  delete p_vf;
}

EXTERN_C void avs_videoframe_plane_blit (AVS_VideoFrame *p_vf, char plane, unsigned char *ptr, int pitch)
{
  avs::PEnvironment env = p_vf->p_vf_->GetEnvironment ();
  avs::Blitter const& blit = env->GetBlitter();

  if (!p_vf)
    return;

  blit.Blit(p_vf->p_vf_->ReadFrom(plane), ptr, pitch);
}