// Avisynth v3.0 alpha.  Copyright 2003 Ben Rudiak-Gould et al.
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


#include "property.h"
#include "algorithm"

typedef smart_ptr<PropertySet> PPropertySet;

void PropertySet::Set(CPProperty prop) 
{
  PropertyVector::iterator it = find(propVector.begin(), propVector.end(), prop->key);
  if (it != propVector.end())
    it->value = prop;
  else
    propVector.push_back(prop);
}

void PropertySet::Remove(PPropertyKey key) 
{
  PropertyVector::iterator it = find(propVector.begin(), propVector.end(), key);
  if (it != propVector.end())
    propVector.erase(it);
}


CPProperty PropertySet::Get(PPropertyKey key) const
{
  PropertyVector::const_iterator it = find(propVector.begin(), propVector.end(), key);
  return it == propVector.end() ? CPProperty() : it->value;
}

void PropertySet::IntegrityCheck() const 
{
  PropertyVector::const_iterator it = propVector.begin();
  while( it != propVector.end() )
  {
    it->value->IntegrityCheck(*this);
    ++it;
  }
}
