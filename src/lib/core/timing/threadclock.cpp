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


//avisynth include
#include "threadclock.h"

#ifdef _WIN32
//windows includes
# include <windows.h>
# include <winbase.h>
#else
//posix include
# include <pthread.h>
#endif


namespace avs { namespace timing {



long long ThreadClock::operator ()() const
{
#ifdef _WIN32

  union
  {
    FILETIME ft;
    long long i64;
  } ct, et, kt, ut;

  GetThreadTimes(GetCurrentThread(), &ct.ft, &et.ft, &kt.ft, &ut.ft);

  return kt.i64 + ut.i64;

#else

  struct timespec tp;
  if (clock_gettime(CLOCK_REALTIME, &tp) == -1) return 0LL;
  else return (1000000000LL*tp.tv_sec + tp.tv_nsec);

#endif  //_WIN32
}


} } //namespace avs::timing
