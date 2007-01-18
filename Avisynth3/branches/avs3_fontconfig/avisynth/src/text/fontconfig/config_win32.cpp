// Avisynth v3.0 alpha.  Copyright 2003-2006 David Pierre - Ben Rudiak-Gould et al.
// Copyleft 2006 Vincent Torri
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

#ifdef WIN32

//avisynth include
#include "config_win32.h"

//boost include
#include <boost/optional.hpp>

//windows include
#include <windows.h>


enum WindowsVersion
{
  WINDOWS_VERSION_W9X_FIRST = 1;
  WINDOWS_VERSION_W95       = 1,
  WINDOWS_VERSION_W95SP1,
  WINDOWS_VERSION_W95OSR2,
  WINDOWS_VERSION_W98,
  WINDOWS_VERSION_W98SP1,
  WINDOWS_VERSION_W98SE,
  WINDOWS_VERSION_WME,
  WINDOWS_VERSION_W9X_LAST  = 99;

  WINDOWS_VERSION_WNT_FIRST = 101,
  WINDOWS_VERSION_WNT351    = 101,
  WINDOWS_VERSION_WNT4,
  WINDOWS_VERSION_W2K,
  WINDOWS_VERSION_WXP,
  WINDOWS_VERSION_WNT_LAST  = 199,

  WINDOWS_VERSION_WCE_FIRST = 201,
  WINDOWS_VERSION_WCE       = 201,
  WINDOWS_VERSION_WCE_LAST  = 299,
};

enum WindowsVersionPlatform
{
  WINDOWS_VERSION_PLATFORM_WINDOWS = 1,
  WINDOWS_VERSION_PLATFORM_NT,
  WINDOWS_VERSION_PLATFORM_CE,
};


boost::optional<std::string>
Config::GetFontFilename ( std::string const& fontName, bool bold = false, bool italic = false )
{
  char buf[MAX_PATH];
  GetWindowsDirectory (buf, MAX_PATH - 1);
  std::string win_dir = buf;
  std::cout << "windows directory : " << win_dir << std::endl;

  WindowsVersion win_version = GetWindowsVersion();
  std::cout << "windows version   :" << version << std::endl;

  // Get the registry path.
  // If version in {101, ..., 199}, it is Windows NT
  if ((version >= WINDOWS_VERSION_WNT_FIRST) &&
      (version <= WINDOWS_VERSION_Wnt_LAST))
    std::string reg_path = "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";
  // Otherwise it's standard Windows
  else
    std::string reg_path = "Software\\Microsoft\\Windows\\CurrentVersion\\Fonts";

  std::string fontname = fontName;
  if (bold) {
    if (italic)
      fontname += " Bold Italic";
    else
      fontname += " Bold";
  }
  else
    if (italic) fontname = " Italic";

  std::string name;
  std::string data;
  while ( GetNextNameValue (reg_path, name, data) == ERROR_SUCCESS ) {
    std::string::size_type pos = name.find (fontname, 0);
    std::cout << " * pos : " << pos << " " << std::string::npos << std::endl;
    if (pos != std::string::npos) {
      std::cout << "Name : " << name << std::endl;
      std::cout << "Data : " << data << std::endl;
      break;
    }
    reg_path = "";
  }
  //close the registrery key
  RegCloseKey(HKEY_LOCAL_MACHINE);

  std::string fontFilename = win_dir + "\\FONTS\\" + data;
  std::cout << "val : " << fontFilename << std::endl;

  return win_dir + "\\FONTS\\" + data;
}

WindowsVersion
Config::GetWindowsVersion ()
{
  OSVERSIONINFO osinfo;
  osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

  if ( !GetVersionEx(&osinfo) )
    throw exception::Generic("Can not get Windows version");

  DWORD platform_id   = osinfo.dwPlatformId;
  DWORD minor_version = osinfo.dwMinorVersion;
  DWORD major_version = osinfo.dwMajorVersion;
  DWORD build_number  = osinfo.dwBuildNumber & 0xFFFF;   // Win 95 needs this
  std::cout << platform_id << ": " << major_version << "." << minor_version << "." << build_number << std::endl;

  // Windows 95 / 98 / ME
  if ( (platform_id == WINDOWS_VERSION_PLATFORM_WINDOWS) &&
      (major_version == 4) ) {
    // Windows 95
    if ((minor_version < 10) && (build_number == 950))
      return WINDOWS_VERSION_W95;
    // Windows 95 SP1
    else if ((minor_version < 10) &&
             ((build_number > 950) && (build_number <= 1080)))
      return WINDOWS_VERSION_W95SP1;
    // Windows 95 OSR2
    else if ((minor_version < 10) && (build_number > 1080))
      return WINDOWS_VERSION_W95OSR2;
    // Windows 98
    else if ((minor_version == 10) && (build_number == 1998))
      return WINDOWS_VERSION_W98;
    // Windows 98 SP1
    else if ((minor_version == 10) &&
             ((build_number > 1998) && (build_number < 2183)))
      return WINDOWS_VERSION_W98SP1;
    // Windows 98 SE
    else if ((minor_version == 10) && (build_number >= 2183))
      return WINDOWS_VERSION_W98SE;
    // Windows ME
    else if (minor_version == 90)
      return WINDOWS_VERSION_WME;
    // Error
    else
      throw exception::Generic("Can not get Windows version");
  }
  // Windows NT
  else if (platform_id == WINDOWS_VERSION_PLATFORM_NT) {
    // Windows NT 3.51
    if ((major_version == 3) && (minor_version == 51))
      return WINDOWS_VERSION_WNT351;
    // Windows NT 4
    else if ((major_version == 4) && (minor_version == 0))
      return WINDOWS_VERSION_WNT4;
    // Windows 2000
    else if ((major_version == 5) && (minor_version == 0))
      return WINDOWS_VERSION_W2K;
    // Windows XP
    else if ((major_version == 5) && (minor_version == 1))
      return WINDOWS_VERSION_WXP;
    // Error
    else
      throw exception::Generic("Can not get Windows version");
  }
  // Windows CE
  else if (platform_id == WINDOWS_VERSION_PLATFORM_CE) {
      return WINDOWS_VERSION_WCE;
  }
  // Error
  else
    throw exception::Generic("Can not get Windows version");

  return true;
}

long
Config::GetNextNameValue(std::string const& sub_key,
                         std::string & name,
                         std::string & data)
{
  static HKEY hkey = NULL;  // registry handle, kept open between calls
  static DWORD index = 0;   // count of values returned
  long ret;

  if ( sub_key.size() != 0 ) {
    ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, sub_key.c_str(), 0, KEY_ALL_ACCESS, &hkey);
    if ( ret != ERROR_SUCCESS )
      throw exception::Generic("Can not get open Registry key");
//       std::cout << "Error : can't open key" << std::endl;
    index = 0;
  }
  else index++;

  char value_name[MAX_PATH];
  DWORD value_name_size = sizeof (value_name);
  char value_data[MAX_PATH];
  DWORD value_data_size = sizeof (value_data);
  DWORD type;

  ret = RegEnumValue(hkey, index,
                     value_name, &value_name_size, NULL,
                     &type, (BYTE *)value_data, &value_data_size);
  if (ret == ERROR_SUCCESS) {
    name = value_name;
    data = value_data;
    std::cout << "name : " << name << std::endl;
    std::cout << "data : " << data << std::endl;
  }
  else
    throw exception::Generic("Can not get value key");
//     std::cout << "Error : can't get value key" << std::endl;

  return ret;
}

#endif //WIN32
