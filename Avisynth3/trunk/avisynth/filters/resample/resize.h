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


#ifndef __AVS_FILTERS_RESIZE_H__
#define __AVS_FILTERS_RESIZE_H__


//avisynth includes
#include "../videoprocess.h"
#include "../../videoinfo.h"

//boost include
#include <boost/utility.hpp>  //for noncopyable


#pragma warning ( push )           //push warning state
#pragma warning (disable : 4250)   //get rid of MI dominance decisions


namespace avs { namespace filters {

//temporary fix
struct SubRange
{

  double lower, upper;
};

class Resize : public VideoProcess::Pipeline
{

public:  //typedefs

  class Filter;
  typedef smart_ref<const Filter> PFilter;


private:  //members

  VideoInfo vi;
  PFilter filter_;
  SubRange subrange_;


public:  //constructor

  Resize(PClip child, PFilter filter, const SubRange& subrange)
    : VideoProcess::Pipeline( child )
    , vi( child->GetVideoInfo() )
    , filter_( filter )
    , subrange_( subrange ) { }


public:  //clip general interface

  virtual const VideoInfo& GetVideoInfo() const { return vi; }


protected:  //write access

  VideoInfo& GetVideoInfo() { return vi; }


public:  //implementation inner subclasses

  class Horizontal;
  class Vertical;


public:  //filter classes

  ///////////////////////////////////////////////////////////////////////////////
  //  Filter
  //
  //
  //
  class Filter
  {

  public:  //Filter interface

    virtual double operator()(double x) const = 0;

    virtual double support() const = 0;

    //computes sum(i = 0; i < count; filter(start + i * step))
    double total(double start, double step, int count) const;


  public:  //implementation subclasses (inner for the sake of nice naming)

    class Point;
    class Triangle;
    class MitchellNetravali;
    class Lanczos3;

  };

  
  /////////////////////////////////////////////////////////////////////////////  
  //  Filter::Point
  //
  //  Nearest neighbour (point sampler), used in PointResize
  //
  class Filter::Point : public Filter
  {

  public:  //Filter interface

    virtual double operator()(double x) const;

    virtual double support() const { return 1.0; }

  };


  /////////////////////////////////////////////////////////////////////////////
  //  Filter::Triangle
  //
  //  Simple triangle filter, used in BilinearResize
  //
  class Filter::Triangle : public Filter
  {

  public:  //Filter interface

    virtual double operator()(double x) const;

    virtual double support() const { return 1.0; }

  };


  //////////////////////////////////////////////////////////////////////////////
  //  Filter::MitchellNetravali
  //
  //  Mitchell-Netraveli filter, used in BicubicResize
  //
  class Filter::MitchellNetravali : public Filter
  {

    double p0, p2, p3, q0, q1, q2, q3;


  public:  //constructor

    MitchellNetravali(double b, double c);


  public:  //Filter interface

    virtual double operator()(double x) const;

    virtual double support() const { return 2.0; }

  };


  ///////////////////////////////////////////////////////////////////////////////
  //  Filter::Lanczos3
  //
  //  Lanczos3 filter, used in Lanczos3Resize
  //
  //  09-14-2002 - Vlad59 - Lanczos3Resize
  //
  class Filter::Lanczos3 : public Filter
  {

  public:  //Filter interface

    virtual double operator()(double x) const;

    virtual double support() const { return 3.0; }


  private:  //sinus cardinal function (sin(x)/x)

    double sinc(double x) const;

  };


public:  //resampling pattern classes
                                     
  class ResamplingPattern : public boost::noncopyable
  {

  protected:  //members

    int * pattern;
    int count_;


  public:  //constructor

    ResamplingPattern(const SubRange& subrange, int src_width);


  public:  //access

    int * get() const { return pattern; }
    int count() const { return count_; }


  public:  //classes

    //implementation inner classes
    class Straight;
    class MMXInterleave;

    //helper class
    class Balancer;

  };

  class ResamplingPattern::Straight : public ResamplingPattern
  {

  public:  //constructor

    Straight(const Filter& filter, const SubRange& subrange, int width, int src_width);
    ~Straight();

  };

  class ResamplingPattern::MMXInterleave : public ResamplingPattern
  {

  public:

    MMXInterleave(const Filter& filter, const SubRange& subrange, int width, int src_width);
    ~MMXInterleave();

  };


};//Resize



}; }; //namespace avs::filters

#pragma warning ( pop )

#endif //__AVS_FILTERS_RESIZE_H__