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


#include "videoframe.h"
#include "videoproperty.h"
#include <functional>   //for unary_function 
#include <algorithm>    //for remove_if()  
#pragma warning(disable:4786)



/**********************************************************************************************/
/*************************************** VideoFrame *******************************************/
/**********************************************************************************************/



void VideoFrame::StackHorizontal(CPVideoFrame other)
{  
  //TODO: the checks  
  
  int original_width = GetVideoWidth(); 
  SizeChange(0, -other->GetVideoWidth(), 0, 0);
  Copy(other, original_width, 0);
}

void VideoFrame::StackVertical(CPVideoFrame other)
{
  //TODO: the checks

  int original_height = GetVideoHeight();
  SizeChange(0, 0, 0, -other->GetVideoHeight());
  Copy(other, 0, original_height);
}




/**********************************************************************************************/
/************************************ TaggedVideoFrame ****************************************/
/**********************************************************************************************/


TaggedVideoFrame::TaggedVideoFrame(const ColorSpace& space, int width, int height, bool IsField)
{
  PPropertySet _prop = new PropertySet();
  _prop->Set(ColorSpaceProperty::GetInstance(space));  //colorspace property set
  _prop->Set(new WidthProperty(width));                //width set
  _prop->Set(new HeightProperty(height));              //height set

  _prop->Set(new FrameTypeProperty(space, IsField));   //Frame type set

  propSet = _prop;   //integritycheck automatically called, errors thrown as needed
}


const ColorSpace& TaggedVideoFrame::GetColorSpace() const
{
  CPColorSpaceProperty prop = propSet->Get(&ColorSpaceProperty::KEY);
  return prop->space;
}

int TaggedVideoFrame::GetVideoWidth() const
{
  CPIntProperty prop = propSet->Get(&WidthProperty::KEY);
  return prop->value;
}

int TaggedVideoFrame::GetVideoHeight() const
{
  CPIntProperty prop = propSet->Get(&HeightProperty::KEY);
  return prop->value;
}

void TaggedVideoFrame::SizeChange(int left, int right, int top, int bottom) //args in pixels
{
  //fetch ColorSpace
  const ColorSpace& space = GetColorSpace();
  //partial check of args validity (otherwise errors from side can compensate themselves)
  space.IsLegalHeightShift(left);
  space.IsLegalHeightShift(top, IsInterlaced());
  //now we do the full change in the prop set, if it works, args are legal
  PPropertySet temp = propSet;
  //update width
  temp->Set(new WidthProperty(GetVideoWidth() - left - right)); 
  //update height
  temp->Set(new HeightProperty(GetVideoHeight() - top - bottom));  
  //try committing
  propSet = temp;  //constraintviolation thrown if bad args 

  //now we have to do the job
  for(int i = 4; i-- > 0; )
  {
    Plane p = IndexToPlane(i);
    if (space.HasPlane(p))
    {
      BufferWindow & buf = GetPlane(p);
      buf = BufferWindow(buf, space.WidthToRowSize(left, p), space.WidthToRowSize(right, p),
        space.HeightToPlaneHeight(top, p), space.HeightToPlaneHeight(bottom, p));
    }
  }//done
}

void TaggedVideoFrame::Copy(CPVideoFrame other, int left, int top)
{
  //TODO: the checks


}


/**********************************************************************************************/
/************************************* RGBVideoFrame ******************************************/
/**********************************************************************************************/

/*
void RGBVideoFrame::FlipHorizontal()
{  
  int row_size = main.GetRowSize();
  int height   = main.GetHeight();
  BufferWindow dst(row_size, height, -main.GetAlign());
  
  BYTE * dstp = dst.GetWritePtr();  
  const BYTE * srcp = main.GetReadPtr();
  int src_pitch = main.GetPitch();
  int dst_pitch = dst.GetPitch();

  int step = WidthToRowSize(1);
  int width = row_size/step;
  srcp += row_size - step;
  for(int h = height; h--> 0; )
  {
    for(int x = width; x--> 0; ) 
      for(int i = step; i-->0; )
        dstp[step*x+i] = srcp[-step*x+i];
    srcp += src_pitch;
    dstp += dst_pitch;
  }
  main = dst;  //replace window by the flipped one  
}*/


/**********************************************************************************************/
/************************************ RGB32VideoFrame *****************************************/
/**********************************************************************************************/

CPVideoFrame RGB32VideoFrame::ConvertTo(const ColorSpace& space, int align) const
{
  switch(space.id)
  {
    case I_RGB24: return new RGB24VideoFrame(*this);
    case I_YUY2:  return new YUY2VideoFrame(*this);
    case I_YV12:  return new PlanarVideoFrame(*this);
  }
  AddRef();   
  return this; //aka case I_RGB32:
}



/**********************************************************************************************/
/************************************ RGB24VideoFrame *****************************************/
/**********************************************************************************************/

CPVideoFrame RGB24VideoFrame::ConvertTo(const ColorSpace& space, int align) const
{
  switch(space.id)
  {
    case I_RGB32: return new RGB32VideoFrame(*this);
    case I_YUY2:  return new YUY2VideoFrame(*this);
    case I_YV12:  return new PlanarVideoFrame(*this);
  }
  AddRef();   
  return this; //aka case I_RGB24:
}


/**********************************************************************************************/
/************************************* YUY2VideoFrame *****************************************/
/**********************************************************************************************/





CPVideoFrame YUY2VideoFrame::ConvertTo(const ColorSpace& space, int align) const
{
  switch(space.id)
  {
    case I_RGB32: return new RGB32VideoFrame(*this);
    case I_RGB24: return new RGB24VideoFrame(*this);
    case I_YV12:  return new PlanarVideoFrame(*this);
  }
  AddRef();   
  return this; //aka case I_YUY2:
}

void YUY2VideoFrame::FlipHorizontal()
{  
  int row_size = main.GetRowSize();
  int height   = main.GetHeight();
  BufferWindow dst(row_size, height, -main.GetAlign());
  
  BYTE * dstp = dst.GetWritePtr();  
  const BYTE * srcp = main.GetReadPtr();
  int src_pitch = main.GetPitch();
  int dst_pitch = dst.GetPitch();

  srcp += row_size - 4;
  for(int h = height; h--> 0; )
  {
    for(int x = row_size>>2; x--> 0; ) {
      dstp[4*x]   = srcp[-4*x+2];
      dstp[4*x+1] = srcp[-4*x+1];
      dstp[4*x+2] = srcp[-4*x];
      dstp[4*x+3] = srcp[-4*x+3];
    }
    srcp += src_pitch;
    dstp += dst_pitch;
  }
  main = dst;  //replace window by the flipped one  
}

/**********************************************************************************************/
/************************************ PlanarVideoFrame ****************************************/
/**********************************************************************************************/





CPVideoFrame PlanarVideoFrame::ConvertTo(const ColorSpace& space, int align) const
{
  switch(space.id)
  {
    case I_RGB32: return new RGB32VideoFrame(*this);
    case I_RGB24: return new RGB24VideoFrame(*this);
    case I_YUY2:  return new YUY2VideoFrame(*this);
  }
  AddRef();   
  return this; //aka case I_YUY2:
}




void PlanarVideoFrame::FlipVertical()
{
  for(int i = 3; i--> 0; ) //reverse loop  
    GetPlane(IndexToPlane(i)).FlipVertical();
}

void PlanarVideoFrame::FlipHorizontal()
{
  for(int i = 3; i--> 0; ) //reverse loop
  {
    BufferWindow& src = GetPlane(IndexToPlane(i));
    int row_size = src.GetRowSize();
    int height   = src.GetHeight();
    BufferWindow dst(row_size, height, -src.GetAlign());
    BYTE* dstp = dst.GetWritePtr();  
    const BYTE* srcp = src.GetReadPtr();
    int src_pitch = src.GetPitch();
    int dst_pitch = dst.GetPitch();

    srcp +=row_size-1;
    for(int h = height; h--> 0; )
    {
      for(int x = row_size; x--> 0; )
        dstp[x] = srcp[-x];
      srcp += src_pitch;
      dstp += dst_pitch;
    }
    src = dst;  //replace window by the flipped one
  }
}

