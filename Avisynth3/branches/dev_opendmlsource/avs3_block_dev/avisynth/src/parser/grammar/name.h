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


#ifndef __AVS_PARSER_GRAMMAR_NAME_H__
#define __AVS_PARSER_GRAMMAR_NAME_H__

//avisynth include
#include "keywords.h"

//boost includes
#include <boost/spirit/core.hpp>


namespace spirit = boost::spirit;


namespace avs { namespace parser { namespace grammar {



/////////////////////////////////////////////////////////////////////////////////
//  Name
//
//  grammar to parse valid names (for both variables and functions)
//
struct Name : public spirit::grammar<Name>
{

public:  //definition nested class

  template <typename ScannerT>
  struct definition
  {

    definition(Name const& /*self*/)
    {

      using namespace spirit;

      name
          =   lexeme_d
              [
                ( (alpha_p | '_') >> *(alnum_p | '_') )
              ]
              - KeyWords::instance
          ;

    }

    spirit::rule<ScannerT> const & start() const { return name; }


  private:
  
    spirit::rule<ScannerT> name;

  };

};


} } } //namepace avs::parser::grammar

#endif //__AVS_PARSER_GRAMMAR_NAME_H__
