// Avisynth v3.0 alpha.  Copyright 2003-2005 David Pierre - Ben Rudiak-Gould et al.
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


//avisynth includes
#include "chunkheader.h"
#include "../../core/utility/round.h"

//stl include
#include <iostream>


namespace avs { namespace import { namespace avi {



ChunkHeader::ChunkHeader(std::istream& stream)
{
  stream.read(reinterpret_cast<char *>(this), sizeof(ChunkHeader));
}


namespace {

struct CharArrayDeleter 
{ 
  void operator()(char * array) const { delete[] array; }
};

}


boost::shared_ptr<void> ChunkHeader::ReadBlockFrom(std::istream& stream)
{
  boost::shared_ptr<char> result( new char[length], CharArrayDeleter() );  //allocates memory

  stream.read(result.get(), length);         //reads data
  if ( length % 2 != 0 )                     //if size of data is odd
    stream.ignore();                         //ignore a char to be at an even boundary again

  return result;
}


boost::optional<ChunkHeader> ChunkHeader::Locate(std::istream& stream, unsigned long fourCC, long& searchLength)
{
  while ( searchLength >= sizeof(ChunkHeader) )
  {
    ChunkHeader result(stream);                       //read chunk header from stream
    searchLength -= sizeof(ChunkHeader);              //update searchLength

    if ( result.fourCC == fourCC )                    //if found
      return result;                                  //success

    unsigned long skip = utility::RoundUp<2>(result.length);

    stream.ignore(skip);                              //else we skip chunk length (rounded up by 2)
    searchLength -= skip;
  }

  return boost::optional<ChunkHeader>();
}



} } } //namespace avs::import::avi
