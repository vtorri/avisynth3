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


#ifndef __ERROR_H__
#define __ERROR_H__


#include <string>
using namespace std;



//internal exception class
//those are not supposed to reach top level
class InternalError {
public: 
  const string err_msg;
  InternalError(const string& _err_msg) : err_msg(_err_msg) { }
};


//exception class (who may reach top level)
class AvisynthError  {
public:
  const string err_msg;
  AvisynthError(const string& _err_msg) : err_msg(_err_msg) { }
};



//exception class for errors in parsed scripts
class ScriptError : public AvisynthError {
public: 
  ScriptError(const string& err_msg) : AvisynthError("Script Error: " + err_msg) { }
};



// the code for this is in AVIReadHandler.cpp

AvisynthError MyWin32Error(const char *format, DWORD err, ...);


#endif  //#ifndef __ERROR_H__


