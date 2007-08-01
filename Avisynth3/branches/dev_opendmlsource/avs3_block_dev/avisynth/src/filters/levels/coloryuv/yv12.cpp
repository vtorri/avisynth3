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


//avisynth includes


namespace avs { namespace filters { namespace coloryuv {



/*
CPVideoFrame YV12::MakeFrame(CPVideoFrame const& source) const
{
  PVideoFrame frame = source;
  //WindowPtr wp = frame->WriteTo('Y');

  if (analyze||autowhite||autogain)
  {
    //Reset memory
    memset(yPParam_.accum, 0, 256*sizeof(unsigned long int));
    memset(uPParam_.accum, 0, 256*sizeof(unsigned long int));
    memset(vPParam_.accum, 0, 256*sizeof(unsigned long int));

    //First YV12 specific code
    //Some extensions possible:
    // - could work for planar colorspaces
    // - use an iterator instead of processing each plane
    // - MMX optimize it
    
    WindowPtr wp = frame->WriteTo('Y')
    for ( int y = wp.height; y-- > 0; wp.pad() )
      for ( int x = wp.width; x-- > 0; wp.to(1, 0) ) 
        ++yPParam_.accum[ *wp.ptr ];

    wp = frame->WriteTo('U');
    for ( int y = wp.height; y-- > 0; wp.pad() )
      for ( int x = wp.width; x-- > 0; wp.to(1, 0) ) 
        ++uPParam_.accum[ *wp.ptr ];

    wp = frame->WriteTo('V');
    for ( int y = wp.height; y-- > 0; wp.pad() )
      for ( int x = wp.width; x-- > 0; wp.to(1, 0) ) 
        ++vPParam_.accum[ *wp.ptr ];

    float avg_u=0, avg_v=0, avg_y=0;
    int min_u=0, min_v=0, min_y=0;
    int max_u=0, max_v=0, max_y=0;
    bool hit_y=false,hit_u=false,hit_v=false;
    int Amin_u=0, Amin_v=0, Amin_y=0;
    int Amax_u=0, Amax_v=0, Amax_y=0;
    bool Ahit_miny=false,Ahit_minu=false,Ahit_minv=false;
    bool Ahit_maxy=false,Ahit_maxu=false,Ahit_maxv=false;
    int At_y2=int(floorf(pixels/256.0f+0.5f)); // When 1/256th of all pixels have been reached, trigger "Loose min/max"
    int At_uv2=int(floorf(pixels/1024.0f+0.5f)); 
    
    for ( int i = 0; i < 256; ++i )
    {
      avg_y += float( yPParam_.accum[i] * i );
      avg_u += float(uPParam_.accum[i]*i);
      avg_v += float(vPParam_.accum[i]*i);

      //??1
      if ( yPParam_.accum[i] != 0 )
      { 
        max_y = i; 
        hit_y = true; 
      }
      else 
        if ( ! hit_y ) 
          min_y = i + 1;
      if (uPParam_.accum[i]!=0) { max_u=i; hit_u=true; }
      else if (!hit_u) min_u=i+1; 
      if (vPParam_.accum[i]!=0) { max_v=i; hit_v=true; }
      else if (!hit_v) min_v=i+1;
      
      //??2
      if ( ! Ahit_miny )
      {
        Amin_y += yPParam_.accum[i];
        if ( Amin_y > At_y2 )
        { 
          Ahit_miny = true; 
          Amin_y = i; 
        }
      }
      if (!Ahit_minu)
      {
        Amin_u+=uPParam_.accum[i];
        if (Amin_u>At_uv2) { Ahit_minu=true; Amin_u=i; }
      }
      if (!Ahit_minv) {
        Amin_v+=vPParam_.accum[i];
        if (Amin_v>At_uv2) { Ahit_minv=true; Amin_v=i; }
      }
      
      //??3
      if (!Ahit_maxy)
      {
        Amax_y+=yPParam_.accum[255-i];
        if (Amax_y>At_y2) { Ahit_maxy=true; Amax_y=255-i; }
      }
      if (!Ahit_maxu)
      {
        Amax_u+=uPParam_.accum[255-i];
        if (Amax_u>At_uv2) { Ahit_maxu=true; Amax_u=255-i; }
      }
      if (!Ahit_maxv)
      {
        Amax_v+=vPParam_.accum[255-i];
        if (Amax_v>At_uv2) { Ahit_maxv=true; Amax_v=255-i; }
      }
    }  

    float Favg_y=avg_y/(float)pixels;
    float Favg_u=(avg_u*4.0f)/(float)pixels;
    float Favg_v=(avg_v*4.0f)/(float)pixels;

#if 0 //FIXME: char + old ApplyMessage
    if (analyze)
    {
      char text[400];
      sprintf(text,
              "        Frame: %-8u ( Luma Y / ChromaU / ChromaV )\n"
              "      Average:      ( %7.2f / %7.2f / %7.2f )\n"
              "      Minimum:      (   %3d   /   %3d   /   %3d    )\n"
              "      Maximum:      (   %3d   /   %3d   /   %3d    )\n"
              "Loose Minimum:      (   %3d   /   %3d   /   %3d    )\n"
              "Loose Maximum:      (   %3d   /   %3d   /   %3d    )\n"
              ,
              frame,
              Favg_y,Favg_u,Favg_v,
              min_y,min_u,min_v,
              max_y,max_u,max_v,
              Amin_y,Amin_u,Amin_v,
              Amax_y,Amax_u,Amax_v
              );
      
      ApplyMessage(&src, vi, text, vi.width/4, 0xa0a0a0,0,0 , env );
      if (!(autowhite||autogain)) {
        return frame;
      }
    }
#endif

    if (autowhite)
    {
      boost::get<1>(uIParam_)=127-(int)Favg_u;
      boost::get<1>(vIParam_)=127-(int)Favg_v;
    }
    if (autogain)
    {
      Amax_y=std::min(Amax_y,236);
      Amin_y=std::max(Amin_y,16);  // Never scale above luma range!
      if (Amin_y!=Amax_y)
      {
        int y_range = Amax_y-Amin_y;
        float scale = (220.0 / y_range);
        boost::get<0>(yIParam_) = (int) (256.0 * scale)-256;
        boost::get<1>(yIParam_) = -(int)(scale * (float)(Amin_y)-16);
      }
    }
    MakeGammaLUT();
  }

  //Third and last part specific to YV12
  wp = frame->WriteTo('Y');
  for ( int y = wp.height; y-- > 0; wp.pad() )
    for ( int x = wp.width; x-- > 0; wp.to(1, 0) ) 
      *wp.ptr = yPParam_.LUT[ *wp.ptr ]

  wp = frame->WriteTo('U');
  for ( int y = wp.height; y-- > 0; wp.pad() )
    for ( int x = wp.width; x-- > 0; wp.to(1, 0) ) 
      *wp.ptr = uPParam_.LUT[ *wp.ptr ]

  wp = frame->WriteTo('V');
  for ( int y = wp.height; y-- > 0; wp.pad() )
    for ( int x = wp.width; x-- > 0; wp.to(1, 0) ) 
      *wp.ptr = vPParam_.LUT[ *wp.ptr ]

  return frame;
}*/



} } } //namespace avs::filters::coloryuv
