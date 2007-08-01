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

#ifndef __COLORSPACE_C_H__
#define __COLORSPACE_C_H__


/* C API include */
#include "define_c.h"


/**
 * @file colorspace_c.h
 * @brief C interface for colorspace of the video of a clip.
 *
 * C interface for colorspaces. You can get the colorspace of a clip
 * with the function avs_videoinfo_colorspace_get().
 *
 * Once you got it, you can check it with avs_colorspace_id_get() and
 * #ColorSpace_Id.
 *
 * Once you have finished with an #AVS_ColorSpace, you delete it with
 * avs_colorspace_delete().
 */

/**
 * opaque declaration of a colorspace.
 */
typedef struct AVS_ColorSpace_ AVS_ColorSpace;


/** @brief ColorSpace ids, used to switch on color spaces */
typedef enum
{
  I_EXTERNAL, /**< reports an external colorspace (defined by a plugin) */
  I_RGB24,    /**< 24 bits RGB colorspace */
  I_RGB32,    /**< 32 bits RGB colorspace */
  I_YUY2,     /**< YUY2 interlaced colorspace */
  I_YV12,     /**< YV12 planar colorspace */
  I_YV24      /**< YV24 planar colorspace */
}ColorSpace_Id;


/** @brief Delete an AVS_ColorSpace.
 *
 * @param p_cs The colorspace to delete.
 *
 * Delete the colorspace @p p_cs.
 */
AVS_C_API void avs_colorspace_delete (AVS_ColorSpace *p_cs);


/** @brief Retrieve the type of the colorspace.
 *
 * @param p_cs The colorspace.
 * @return The id of the colorspace.
 *
 * Retrieve the type of the colorspace @p p_cs.
 */
AVS_C_API ColorSpace_Id avs_colorspace_id_get (const AVS_ColorSpace *p_cs);


#endif /* __COLORSPACE_C_H__ */
