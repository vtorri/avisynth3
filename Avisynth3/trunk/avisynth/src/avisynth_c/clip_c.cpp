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
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <sys/stat.h>

// C API include
#include "define_c.h"
#include "private/clip_c_private.h"
#include "private/error_c_private.h"
#include "private/videoinfo_c_private.h"
#include "private/videoframe_c_private.h"
#include "private/runtime_environment_c_private.h"

// Avisynth include
#include "../core/clip.h"
#include "../parser/parser.h"
#include "../core/exception.h"
#include "../core/exception/colorspace/unsupported.h"
#include "../core/videoinfo.h"
#include "../filters/convert/toyv12.h"
#include "../filters/convert/torgb32.h"
#include "../filters/source/messageclip.h"


using namespace std;

// FIXME: link problem if I use the ifstream type. I don't understand...
static string
get_script (const char *filename)
{
  FILE *file;
  struct stat buf;
  char *content;
  int filesize;
  int res;
  string script = "";

  stat (filename, &buf);
  filesize = buf.st_size;

  file = fopen (filename, "r");
  if (!file)
    return script;

  content = (char *)malloc (sizeof (char) * (filesize + 1));
  if (!content) {
    fclose (file);
    return script;
  }
  res = fread (content, 1, filesize, file);
  content[filesize] = '\0';
  fclose (file);

  if (res != filesize) {
    printf ("res != filesize\n");
    free (content);
    return script;
  }
  script = content;
  free (content);

  cout << "script : " << script << endl;

//   while (getline (file, line))
//     {
//       if (!line.empty())
//         script += line + "\n";
//     }

  return script;
}

// static string
// get_script2 (char *filename)
// {
//   ifstream file;//(filename, ios::in);
//   string script = "";
//   string line;

//   while (getline (file, line))
//     {
//       if (!line.empty())
//         script += line + "\n";
//     }

//   return script;
// }

// Constructor

EXTERN_C AVS_Clip *
avs_clip_new_from_script (const char *script, const AVS_Environment *p_env)
{
  AVS_Clip *p_clip;

  if (!script || !p_env || !p_env->p_env_)
    return NULL;

  p_clip = new AVS_Clip;
  if (!p_clip)
    return NULL;

  p_clip->p_clip_ = avs::parser::Parser()(script, p_env->p_env_);

  return p_clip;
}

EXTERN_C AVS_Clip *avs_clip_new_from_file (const char *filename, const AVS_Environment *p_env)
{
  AVS_Clip *p_clip;

  if (! filename || !p_env || !p_env->p_env_)
    return NULL;

  string script = get_script (filename);

  p_clip = new AVS_Clip;
  if (!p_clip)
    return NULL;

  p_clip->p_clip_ = avs::parser::Parser()(script, p_env->p_env_);

  return p_clip;
}

EXTERN_C AVS_Clip *avs_clip_new_from_message (const char *msg, const AVS_Environment *p_env)
{
  AVS_Clip *p_clip;

  if (! msg || !p_env || !p_env->p_env_)
    return NULL;

  p_clip = new AVS_Clip;
  if (!p_clip)
    return NULL;

  p_clip->p_clip_ = avs::filters::MessageClip::Create(msg, p_env->p_env_);

  return p_clip;
}

EXTERN_C AVS_Clip *
avs_clip_new_to_rgb32 (const AVS_Clip *p_clip, AVS_Error **error)
{
  if (error)
    *error = NULL;

  if (!p_clip || p_clip->p_clip_->GetVideoInfo ()->IsRGB32 ())
    return NULL;

  avs::PClip clip;
  try {
     clip = avs::filters::convert::ToRGB32::Create(p_clip->p_clip_);
  }
  catch (avs::exception::colorspace::Unsupported e) {
    if (error) {
      *error = avs_error_new (AVS_ERROR_COLORPSACE_UNSUPPORTED,
                              e.msg ().c_str ());
      return NULL;
    }
  }

  AVS_Clip *p_clip_rgb32 = new AVS_Clip;
  if (!p_clip_rgb32)
    return NULL;

  p_clip_rgb32->p_clip_ = clip;
  return p_clip_rgb32;
}

EXTERN_C AVS_Clip *
avs_clip_new_to_yv12 (const AVS_Clip *p_clip)
{
  AVS_Clip *p_clip_yv12;

  if (!p_clip || p_clip->p_clip_->GetVideoInfo ()->IsYV12 ())
    return NULL;

  p_clip_yv12 = new AVS_Clip;
  if (!p_clip_yv12)
    return NULL;

  p_clip_yv12->p_clip_ = avs::filters::convert::ToYV12::Create(p_clip->p_clip_);

  return p_clip_yv12;
}

// Destructor

EXTERN_C void
avs_clip_delete (AVS_Clip *p_clip)
{
  if (!p_clip) return;

  delete p_clip;
}

// General interface

EXTERN_C AVS_VideoInfo *
avs_clip_videoinfo_get (const AVS_Clip *p_clip)
{
  AVS_VideoInfo *p_vi;

  if (!p_clip)
    return NULL;

  p_vi = new AVS_VideoInfo;
  if (!p_vi)
    return NULL;

  p_vi->p_vi_ = p_clip->p_clip_->GetVideoInfo ();

  return p_vi;
}

EXTERN_C AVS_VideoFrame *
avs_clip_videoframe_get (const AVS_Clip *p_clip, long int n)
{
  AVS_VideoFrame *p_vf;

  if (!p_clip)
    return NULL;

  p_vf = new AVS_VideoFrame;
  if (!p_vf)
    return NULL;

  p_vf->p_vf_ = p_clip->p_clip_->GetFrame (n);

  return p_vf;
}

EXTERN_C unsigned char *
avs_clip_audio_get (const AVS_Clip *p_clip, unsigned char *buffer, long long start, long count)
{
  unsigned char *position;

  if (!p_clip || !buffer || (count < 0))
    return NULL;

  printf (" ** audio taille : %d\n", sizeof (*buffer));
  memset (buffer, 0, sizeof (*buffer));

  position = p_clip->p_clip_->GetAudio (buffer, start, count);

  return position;
}
