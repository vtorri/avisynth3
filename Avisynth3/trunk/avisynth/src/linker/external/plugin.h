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


#ifndef __AVS_LINKER_EXTERNAL_PLUGIN_H__
#define __AVS_LINKER_EXTERNAL_PLUGIN_H__

//avisynth includes
#include "../plugin.h"
#include "../../utility/folder.h"


namespace avs { namespace linker { namespace external {



//////////////////////////////////////////////////////////////////////////////////
//  external::Plugin
//
//
//
class Plugin : public linker::Plugin
{

  ////////////////////////////////////////////////////////////////////////////////
  //  Plugin::Expired
  //
  //  functor for the folder template to determine when plugins can be deleted
  //
  struct Expired
  {
    
    bool operator()(std::string const& str) const { return false; }

    bool operator()(boost::shared_ptr<external::Plugin const> const& plugin) const;

  };

  typedef boost::shared_ptr<external::Plugin const> PExtPlugin;
  typedef folder<std::string, PExtPlugin, Expired> PluginFolder;

  std::string fileName_;    //name of source file
  std::string pluginName_;  //plugin name

  static PluginFolder pluginFolder;


protected:  //structors

  Plugin(std::string const& fileName);


public:  //Plugin interface

  virtual char const * GetName() const { return pluginName_.c_str(); }


public:  //factory method

  static PPlugin Create(std::string const& fileName);


private:  //implementation helper

  static std::string CompleteName(std::string const& fileName);

};



} } } //namespace avs::linker::external

#endif //__AVS_LINKER_EXTERNAL_PLUGIN_H__
