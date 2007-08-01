// Avisynth v3.0 alpha.  Copyright 2003-2007 David Pierre - Ben Rudiak-Gould et al.
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


//macro include
#include "../config.h"

#ifdef AVS_HAS_GSTREAMER_SOURCE

//avisynth includes
#include "message.h"


namespace avs { namespace gstreamer {



MessageType Message::GetType()
{ 
  return static_cast<MessageType>( GST_MESSAGE_TYPE( this ) );
}


boost::optional<std::string> Message::GetError()
{
  GError *error;
  gchar *msg;
  boost::optional<std::string> res;

  gst_message_parse_error( this, &error, &msg );

  if (msg)
    {
      res = msg;
      free(msg);
    }

  if (error)
    g_error_free( error );

  return res;
}


} } //namespace avs::gstreamer

#endif //AVS_HAS_GSTREAMER_SOURCE
