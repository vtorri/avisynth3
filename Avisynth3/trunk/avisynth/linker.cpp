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



#include "linker.h"




const ArgumentMatcher::CastMap ArgumentMatcher::castMap = CreateCastMap();


ReorderPattern::ReorderInfo::ReorderInfo(int _position) : position(_position)
{
  if (position < 0)
    throw std::domain_error("a position must be positive");
}






PrototypeMatcher::PrototypeMatcher(const DescriptionPrototype& descript, const LinkagePrototype& link)
{




}







bool ReorderPattern::IsTrivial() const
{
  iterator it = begin();
  int index = 0;
  while (it != end() && it->position == index++) { ++it; }
  return it == end();
}


void Reorderer::DefaultFill(ArgVector& args) const
{
  for(DescriptionPrototype::iterator it = prototype.begin(); it != prototype.end(); )
    args.push_back(it->GetDefault());
}


Reorderer * Reorderer::MakeReorderer(const DescriptionPrototype& prototype, const ReorderPattern& pattern)
{
  return pattern.IsTrivial() ? (Reorderer *)new TrivialReorderer(prototype) : new PatternReorderer(prototype, pattern);
}


const ArgVector TrivialReorderer::operator()(const ArgVector& rawArgs) const
{
  ArgVector result;
  DefaultFill(result);
  copy(rawArgs.begin(), rawArgs.end(), result.begin());
  return result;
}

const ArgVector PatternReorderer::operator()(const ArgVector& rawArgs) const
{
  ArgVector result;
  DefaultFill(result);
  ReorderPattern::iterator p_it = pattern.begin();
  ArgVector::const_iterator a_it = rawArgs.begin();

  while( p_it != pattern.end() )
  {
    result[p_it->position] = *a_it;
    ++p_it;
    ++a_it;
  }

  return result;
}
