// Avisynth v2.5.  Copyright 2002 Ben Rudiak-Gould et al.
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
//
// As a special exception, the copyright holders of Avisynth give you
// permission to link Avisynth with independent modules that communicate with
// Avisynth solely through the interfaces defined in avisynth.h, regardless of the license
// terms of these independent modules, and to copy and distribute the
// resulting combined work under terms of your choice, provided that
// every copy of the combined work is accompanied by a complete copy of
// the source code of Avisynth (the version of Avisynth used to produce the
// combined work), being distributed under the terms of the GNU General
// Public License plus this exception.  An independent module is a module
// which is not derived from or based on Avisynth, such as 3rd-party filters,
// import and export plugins, or graphical user interfaces.


#include "cache.h"
#include "pclip.h"


CPVideoFrame Cache::GetCachedFrame(int n, Clip& client, Clip& source)
{
  CPVideoFrame result;
  //first operation : we search all frame caches for frame n
  for(CacheVector::iterator i = cache.begin(); i != cache.end(); ++i )    
  {
    CPVideoFrame result = (*i)->fetch(n);
    if ( result )       //if found  (not NULL)
      return result;    //return it
  }
  //second operation : we check that client is a registered client
  //search its entry in the client map
  ClientToCacheMap::iterator it = clientMap.find(&client);       
  //if found : use it, else create register client (which creates the entry)
  FrameCache * targetCache = (it != clientMap.end()) ? it->second :
                                                       RegisterClient(client);
  //use the source to make the frame, store it in cache and return it
  return targetCache->store(n, source.MakeFrame(n) );
}





