// Avisynth v3.0 alpha.  Copyright 2002 Ben Rudiak-Gould et al.
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


#include "avsfunction.h"





CorePlugin * CorePlugin::GetCore()
{
  static CorePlugin * core = new CorePlugin();
  //we don't have to worry about deleting it here, the env will do it.
  return core;
}


const string& CorePlugin::GetName() const
{
  static const string name = "Avisynth Core";
  return name;
}

const string& CorePlugin::GetAuthor() const
{
  static const string author = "Ben Rubiak Gould et al.";
  return author;
}

const string& CorePlugin::GetDescription() const
{
  static const string description = "Set of all avisynth internals";
  return description;
}
