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


#ifndef __AVSVALUE_H__
#define __AVSVALUE_H__

#include <boost/any.hpp>  //which includes <typeinfo>
using boost::any_cast;


class AVSValue {

  boost::any content;
  bool defined;

public:
  AVSValue() : defined(false) { }
  template <typename T> AVSValue(const T& value) : content(value), defined(true) { }
  template<> AVSValue(const AVSValue& other) : content(other.content), defined(other.defined) { }

  template <typename T> Is() const { return typeid(T) == content.type(); }
  bool IsDefined() const { return defined; }
  const std::type_info & Type() const { return content.type(); }

  template <typename T> const AVSValue& operator=(const T& value)
  {
    any_cast<T>(content);  //to throw bad_any_cast if typemismatch
    content = _value;
    return *this;
  }
  template<> const AVSValue& operator=(const AVSValue& other)
  {
    content = other.content;
    defined = other.defined;
    return *this;
  }

  template <typename T> operator T() const { return IsDefined()? any_cast<T>(value) : any_cast<void>(value); }

  template <typename T> const AVSValue& SetType() { content = any(T()); return *this; }
};

typedef vector<AVSValue> ArgVector;


#endif // #define __AVSVALUE_H__