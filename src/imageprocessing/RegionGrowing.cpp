/*
  Copyright (C) 2011 The SmithDR team, smithdr@labri.fr

  This file is part of SmithDR.

  SmithDR is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  SmithDR is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with SmithDR.
  If not, see <http://www.gnu.org/licenses/>.
*/

#include "RegionGrowing.hpp"

#include <stack>

namespace sd {
  namespace imageprocessing {

    template<typename T>
    RegionGrowing<T>::RegionGrowing()
    {}

    template<typename T>
    RegionGrowing<T>::~RegionGrowing()
    {}

    template<typename T>
    T getPixel2d(T* data, size_t xSize, size_t x, size_t y) { return data[x+y*xSize]; }

#define XY( ox, oy ) (int)(x+(ox)), (int)(y+(oy))
#define FFSTEP2D( ox, oy )						\
    T r = getPixel2d(data+sliceOffset, width, XY( ox, oy ));		\
    if ( float(seedColor)+sup >= r && float(seedColor)-inf <= r &&	\
	 !ffset.count( FloodFill2dCoord( XY( ox, oy ) ) ) ) {		\
      stack.push( FloodFill2dCoord( XY( ox, oy ) ) );			\
    }

    template<typename T>
    void
    RegionGrowing<T>::floodFill2d(const sd::core::ImageView_<T>& iv,
				  sd::Point seed,
				  float inf, float sup,
				  FloodFill2dSet& ffset,
				  bool connexity8) {
      T* data = iv.begin().operator->();
      size_t width = iv.size().width();
      size_t height = iv.size().height();
      size_t sliceOffset = seed.z() * width * height;
      T seedColor = getPixel2d(data+sliceOffset, width, seed.x(), seed.y());

      std::stack<FloodFill2dCoord> stack;
      stack.push(FloodFill2dCoord(seed.x(), seed.y()));
      while (!stack.empty()) {
	FloodFill2dCoord coord = stack.top();
	stack.pop();
	ffset.insert(coord);
	sd::UINT16 x = std::get<0>(coord);
	sd::UINT16 y = std::get<1>(coord);

	if ( x < width-1 )  {   FFSTEP2D(  1,  0 )   }
	if ( x > 1 )        {   FFSTEP2D( -1,  0 )   }

	if ( y < height-1 ) {   FFSTEP2D(  0,  1 )   }
	if ( y > 1 )        {   FFSTEP2D(  0, -1 )   }

	if ( connexity8 ) {
	  if ( x < width-1 && y < height-1 ) {   FFSTEP2D(  1,  1 )   }
	  if ( x > 1 && y > 1 )              {   FFSTEP2D( -1, -1 )   }
	  if ( x > 1 && y < height-1 )       {   FFSTEP2D( -1,  1 )   }
	  if ( x < width-1 && y > 1 )        {   FFSTEP2D(  1, -1 )   }
	}
      }
    }

    template<typename T>
    T getPixel3d(T* data, size_t xSize, size_t ySize, size_t x, size_t y, size_t z) { return data[x+y*xSize+z*xSize*ySize]; }

#define XYZ( ox, oy, oz ) (int)(x+(ox)), (int)(y+(oy)), (int)(z+(oz))
#define FFSTEP3D( ox, oy, oz )						\
    T r = getPixel3d(data, width, height, XYZ( ox, oy, oz ));		\
    if ( float(seedColor)+sup >= r && float(seedColor)-inf <= r &&	\
	 !ffset.count( FloodFill3dCoord( XYZ( ox, oy, oz ) ) ) ) {	\
      stack.push( FloodFill3dCoord( XYZ( ox, oy, oz ) ) );		\
    }

    template<typename T>
    void
    RegionGrowing<T>::floodFill3d(const sd::core::ImageView_<T>& iv,
				  sd::Point seed,
				  float inf, float sup,
				  FloodFill3dSet& ffset,
				  bool connexity26) {
      T* data = iv.begin().operator->();
      size_t width = iv.size().width();
      size_t height = iv.size().height();
      size_t depth = iv.size().depth();
      T seedColor = getPixel3d(data, width, height, seed.x(), seed.y(), seed.z());

      std::stack<FloodFill3dCoord> stack;
      stack.push( FloodFill3dCoord( seed.x(), seed.y(), seed.z() ) );
      while ( !stack.empty() ) {
	FloodFill3dCoord coord = stack.top();
	stack.pop();
	ffset.insert( coord );
	sd::UINT16 x = std::get<0>( coord );
	sd::UINT16 y = std::get<1>( coord );
	sd::UINT16 z = std::get<2>( coord );

	if ( x < width-1 )  {   FFSTEP3D(  1,  0,  0 )   }
	if ( x > 1 )        {   FFSTEP3D( -1,  0,  0 )   }

	if ( y < height-1 ) {   FFSTEP3D(  0,  1,  0 )   }
	if ( y > 1 )        {   FFSTEP3D(  0, -1,  0 )   }

	if ( z < depth-1 )  {   FFSTEP3D(  0,  0,  1 )   }
	if ( z > 1 )        {   FFSTEP3D(  0,  0, -1 )   }

	if ( connexity26 ) {
	  if ( x < width-1  && y < height-1 ) {   FFSTEP3D(  1,  1,  0 )   }
	  if ( x > 1        && y < height-1 ) {   FFSTEP3D( -1,  1,  0 )   }
	  if ( x < width-1  && y > 1 )        {   FFSTEP3D(  1, -1,  0 )   }
	  if ( x > 1        && y > 1 )        {   FFSTEP3D( -1, -1,  0 )   }

	  if ( x < width-1  && z < depth-1 )  {   FFSTEP3D(  1,  0,  1 )   }
	  if ( x > 1        && z < depth-1 )  {   FFSTEP3D( -1,  0,  1 )   }
	  if ( x < width-1  && z > 1 )        {   FFSTEP3D(  1,  0, -1 )   }
	  if ( x > 1        && z > 1 )        {   FFSTEP3D( -1,  0, -1 )   }

	  if ( y < height-1 && z < depth-1 )  {   FFSTEP3D(  0,  1,  1 )   }
	  if ( y > 1        && z < depth-1 )  {   FFSTEP3D(  0, -1,  1 )   }
	  if ( y < height-1 && z > 1 )        {   FFSTEP3D(  0,  1, -1 )   }
	  if ( y > 1        && z > 1 )        {   FFSTEP3D(  0, -1, -1 )   }

	  if ( x < width-1 && y < height-1 && z < depth-1 )  {   FFSTEP3D(  1,  1,  1 )   }
	  if ( x < width-1 && y > 1        && z > 1 )        {   FFSTEP3D(  1, -1, -1 )   }
	  if ( x < width-1 && y > 1        && z < depth-1 )  {   FFSTEP3D(  1, -1,  1 )   }
	  if ( x < width-1 && y < height-1 && z > 1 )        {   FFSTEP3D(  1,  1, -1 )   }

	  if ( x > 1       && y < height-1 && z < depth-1 )  {   FFSTEP3D( -1,  1,  1 )   }
	  if ( x > 1       && y > 1        && z < depth-1 )  {   FFSTEP3D( -1, -1,  1 )   }
	  if ( x > 1       && y < height-1 && z > 1 )        {   FFSTEP3D( -1,  1, -1 )   }
	  if ( x > 1       && y > 1        && z > 1 )        {   FFSTEP3D( -1, -1, -1 )   }
	}
      }
    }


    
#define MAKE_TEMPLATED(T)						\
    template RegionGrowing<T>::RegionGrowing();				\
    template RegionGrowing<T>::~RegionGrowing();			\
    									\
    template void RegionGrowing<T>::floodFill2d(const sd::core::ImageView_<T>& iv, \
						sd::Point seed,	\
						float inf, float sup,	\
						FloodFill2dSet& ffset,	\
						bool connexity8 = false); \
									\
    template void RegionGrowing<T>::floodFill3d(const sd::core::ImageView_<T>& iv, \
						sd::Point seed,	\
						float inf, float sup,	\
						FloodFill3dSet& ffset,	\
						bool connexity26 = false);
    
    MAKE_TEMPLATED(float)
    MAKE_TEMPLATED(double)
    MAKE_TEMPLATED(sd::INT8)
    MAKE_TEMPLATED(sd::UINT8)
    MAKE_TEMPLATED(sd::INT16)
    MAKE_TEMPLATED(sd::UINT16)
    MAKE_TEMPLATED(sd::INT32)
    MAKE_TEMPLATED(sd::UINT32)

#undef MAKE_TEMPLATED

  }
}
