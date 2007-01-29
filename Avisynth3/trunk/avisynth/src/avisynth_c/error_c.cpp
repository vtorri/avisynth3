/* Avisynth 3.0 C Interface
 * Copyright 2005-2007 Vincent Torri <vtorri at univ-evry dot fr>
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

// STLport include
#include <string>

// C API include
#include "define_c.h"
#include "enum_c.h"
#include "private/error_c_private.h"


AVS_Error *
avs_error_new (AVS_Error_Code code, const char *description)
{
  AVS_Error *error = new AVS_Error;
  if (!error)
    return NULL;

  error->code = code;
  error->description = std::string (description);

  return error;
}

EXTERN_C int
avs_error_code_get (AVS_Error *error)
{
  /* should never occur */
  if (!error)
    return -1;

  return error->code;
}

EXTERN_C const char *
avs_error_message_get (AVS_Error *error)
{
  /* should never occur */
  if (!error)
    return NULL;

  switch (error->code) {
  case AVS_ERROR_BAD_DIMENSION:
    return "bad dimension";
  case AVS_ERROR_BROKEN_FILE:
    return "broken file";
  case AVS_ERROR_COLLISION_COLORSPACE:
    return "collision of colorspace";
  case AVS_ERROR_COLLISION_DIMENSION:
    return "collision of dimension";
  case AVS_ERROR_COLLISION_FPS:
    return "collision of fps";
  case AVS_ERROR_COLLISION_FRAMECOUNT:
    return "collision of frame count";
  case AVS_ERROR_COLORPSACE_INVALID_HEIGHT:
    return "colorspace: invalid height";
  case AVS_ERROR_COLORPSACE_INVALID_WIDTH:
    return "colorspace: invalid height";
  case AVS_ERROR_COLORPSACE_UNKNOWN:
    return "unkown colorspace";
  case AVS_ERROR_COLORPSACE_UNSUPPORTED:
    return "unsupported colorspace";
  case AVS_ERROR_FATAL:
    return "fatal error";
  case AVS_ERROR_GENERIC:
    return "generic error";
  case AVS_ERROR_NO_AUDIO:
    return "no audio";
  case AVS_ERROR_NO_SUCH_FILE:
    return "no such file";
  case AVS_ERROR_NO_SUCH_FRAME:
    return "no such frame";
  case AVS_ERROR_NO_SUCH_PLANE:
    return "no such plane";
  case AVS_ERROR_NO_VIDEO:
    return "no video";
    /* should never occur */
  default:
    return NULL;
  }
}

EXTERN_C const char *
avs_error_description_get (AVS_Error *error)
{
  /* should never occur */
  if (!error)
    return NULL;

  return error->description.c_str ();
}

EXTERN_C void
avs_error_delete (AVS_Error *error)
{
  if (!error)
    return;

  delete error;
}
