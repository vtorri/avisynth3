// Avisynth v3.0 alpha.  Copyright 2005 David Pierre - Ben Rudiak-Gould et al.
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
#include "linespanvector.h"

//stl include
#include <algorithm>            //for std::sort

//assert include
#include <assert.h>


namespace avs { namespace text { namespace rasterizer {





LineSpanVector::LineSpanVector(FlipVectorPair const& flips)
{
  assert( ! flips.enter.empty() && flips.enter.size() == flips.exit.size() );

  //sorts transitions into ascending order
  std::sort(flips.enter.begin(), flips.enter.end());
  std::sort(flips.exit.begin(), flips.exit.end());

  FlipVector::const_iterator enterIt = flips.enter.begin();
  FlipVector::const_iterator exitIt = flips.exit.begin();
    
  FlipVector::const_iterator enterEnd = flips.enter.end();

  while ( true )
  {
    //here we are in a position where there is spans to make
    //and the last span (if there is any) doesn't overlap with the next one
    while ( *enterIt == *exitIt )                 //while we keep getting empty spans
      if ( ++enterIt != enterEnd )                //but there is more spans
        ++exitIt;                                 //we keep advancing          
      else                                        //no more spans to make
        return;                                   //we are done

    //since we now have a non-empty next span, which doesn't overlap with previous (if there is any)
    spanVector_.push_back( LineSpan(*enterIt++, *exitIt++) );      //we take it
    
    if ( enterIt == enterEnd )                    //if no more
      return;                                     //we are done

    LineSpan& span = spanVector_.back();
    while ( *enterIt <= span.end )                //while the next span is mergeable into last one
    {
      span.end = std::max(span.end, *exitIt);     //we merge it (sort guarantee nothing to do with begin)
      if ( ++enterIt != enterEnd )                //if there is more
        ++exitIt;                                 //advance to next
      else 
        return;                                   //else we are done
    }

    //here there is a next span which doesn't overlap last one, so it can go back for a loop
  }
}


void LineSpanVector::MergeThickened(LineSpanVector const& other, long radius)
{
  SpanVector result;                                //we don't mutate this directly, but pack a new span vector
  
  iterator itThis  = begin();
  iterator itOther = other.begin();
   
  while ( itThis != end() && itOther != other.end() ) 
  {
    //besides while condition, we know that neither this nor other overlaps the last span (if there is any)

    LineSpan span;                                  //future packed span

    if ( itThis->begin < itOther->begin - radius )  //this' span starts before
      span = *itThis++;                             //we take it
    else
    {
      span = itOther->Thicken(radius);              //we take a thickened span from other
      ++itOther;

      span.RightMerge(itThis, end());               //merge with spans from this      
      //even if it merged nothing, we still go in the merge loop after
      //since other's thickened spans can overlap with themselves
    }
    
    //merge as much overlapping spans that possible, alternating from other and this
    while ( span.RightMergeThickened(itOther, other.end(), radius) && span.RightMerge(itThis, end()) ) { }
 
    result.push_back(span);                         //flush our merged span            
  }

  //now we handle left over spans in either this or other (but not both)
  if ( itThis != end() )                            //if more in self
    result.insert( result.end(), itThis, end() );   //add all of them at result's end
  else                                              //else consider other
    while ( itOther != other.end() )                //while more in other
    {
      LineSpan span = itOther->Thicken(radius);     //take the 1st one
      span.RightMergeThickened(++itOther, other.end(), radius);  //merge thickened spans within
      result.push_back(span);                       //flush it
    }

  swap(result, spanVector_);                        //commit result to self
}



void LineSpanVector::Remove(LineSpanVector const& other)
{
  SpanVector result;

  iterator itThis  = begin();
  iterator itOther = other.begin();

  while ( itThis != end() ) 
  {
    //while other has spans which are before span
    while ( itOther != other.end() && *itOther < *itThis )
      ++itOther;                                    //we skip them

    if ( itOther == other.end() )
      break;

    LineSpan span = *itThis;                        //take span from this
    ++itThis;                                       //and move on

    if ( span < *itOther )                          //if *itOther is after span
    {
      result.push_back(span);                       //span is good 
      continue;
    }
    
    //it's neither before, nor after: ie it overlaps
    if ( itOther->begin <= span.begin )             //if other's span starts before span
    {
      span.begin = itOther->end;                    //remove it from span (which may become illegally empty)
      ++itOther;                                    //and move on
    }

    //while there is spans in other ending before span (which implies that span was not made illegal before)
    while ( itOther != other.end() && itOther->end < span.end )
    {
      //push the part before *ItOther
      result.push_back( LineSpan(span.begin, itOther->begin) );
      span.begin = itOther->end;                    //update span to be the part after
    }
    
    result.push_back(span);                         //push what we have left of span
  }

  result.insert(result.end(), itThis, end());

  swap(result, spanVector_);                        //commit result to self
}



void LineSpanVector::Realize(BYTE * ptr, LineSpan const& span, int step) const
{
  iterator it = begin();

  while ( it != end() && *it < span )         //while packed span are before
    ++it;                                     //we skip them

  if ( it == end() )                          //if no more
    return;                                   //done

  if ( *it <= span.begin )                    //if *it starts before span.begin, it overlaps (since we know it's not before)
  {
    LineSpan truncated(span.begin, it->end);  //truncated part
    truncated.Realize(ptr, step);             //realize it
    ++it;                                     //move to next
  }

  while ( it != end() && *it < span.end )     //while there more which are inside span
  {
    it->Realize(ptr, step);                   //we realize them
    ++it;                                     //next one
  }

  if ( it != end() && *it <= span.end )       //there is an extra span, which starts inside span: then overlap
  {
    LineSpan truncated(it->begin, span.end);  //get truncated part
    truncated.Realize(ptr, step);             //and realize it
  }

}



} } } //namespace avs::text::rasterizer
