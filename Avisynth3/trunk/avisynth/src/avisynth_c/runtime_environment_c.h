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

#ifndef __RUNTIME_ENVIRONMENT_C_H__
#define __RUNTIME_ENVIRONMENT_C_H__


/* C API include */
#include "define_c.h"


/** \file runtime_environment_c.h
 * \brief C interface for environments
 *
 * C interface for environment.
 *
 * The environment sets the quantity of memory that avisynth will
 * use. You set it with the function avs_environment_new().
 *
 * Once you have finished with an AVS_VideoInfo (usually when avisynth
 * is not used anymore), you delete it with avs_environment_delete().
 */


/**
 * opaque declaration of an environment.
 */
typedef struct AVS_Environment_ AVS_Environment;


/** \brief Create an environment.
 *
 * \param size The string that contains the script.
 * \return A newly allocated environment.
 *
 * Create an environment of size \p size (in bytes). If an error
 *occurred, this function returns \c NULL.
 */
AVS_C_API AVS_Environment *avs_environment_new    (int size);


/** \brief Delete an AVS_Environment.
 *
 * \param p_env The environment to delete.
 *
 * Delete the environment \p p_env.
 */
AVS_C_API void             avs_environment_delete (AVS_Environment *p_env);


#endif /* __RUNTIME_ENVIRONMENT_C_H__ */
