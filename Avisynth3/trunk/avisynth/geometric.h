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


#ifndef __GEOMETRIC_H__   //if anybody got a better name...
#define __GEOMETRIC_H__

#include <string>  //needed or invalid_argument won't work



class Vecteur {

protected:
  int x, y;

public:
  Vecteur() : x(0), y(0) { }
  Vecteur(int _x, int _y) : x(_x), y(_y) { }
  Vecteur(const Vecteur& other) : x(other.x), y(other.y) { }

  int GetX() const { return x; }
  int GetY() const { return y; }

  const Vecteur& operator=(const Vecteur& other) { x = other.x; y = other.y; return *this; }

  Vecteur operator-() const { return Vecteur(-x, -y); }
};



//exception thrown when dimension with negative width or height
class bad_dimension : public std::logic_error {

public:
  bad_dimension() : std::logic_error("Bad Dimension") { }
};


class Dimension : public Vecteur {

  static inline int CheckSign(int val)
  {
    if (val < 0)
      throw bad_dimension();
    return val;
  }

public:
  Dimension() { }
  Dimension(int width, int height) : Vecteur(CheckSign(width), CheckSign(height)) { }
  Dimension(const Dimension& other) : Vecteur(other) { }

  const Dimension& operator=(const Dimension& other) { x = other.x; y = other.y; return *this; }

  int GetWidth() const { return x; }
  int GetHeight() const { return y; }

  void SetWidth(int width) { x = CheckSign(width); }
  void SetHeight(int height) { y = CheckSign(height); }

  bool operator==(const Dimension& other) const { return x == other.x && y == other.y; }
  bool empty() const { return x == 0 || y == 0; }

  Dimension operator+(const Vecteur& vect) { return Dimension(x + vect.GetX(), y + vect.GetY()); }
  Dimension operator-(const Vecteur& vect) { return Dimension(x - vect.GetX(), y - vect.GetY()); }

  Dimension operator+(const Dimension& other) { return Dimension(x + other.x, y + other.y); }
};


Vecteur operator+(const Vecteur& left, const Vecteur& right) { return Vecteur(left.GetX() + right.GetX(), left.GetY() + right.GetY()); }
Dimension operator-(const Vecteur& left, const Vecteur& right) { return Dimension(left.GetX() - right.GetX(), left.GetY() - right.GetY()); }


#endif //#define __GEOMETRIC_H__