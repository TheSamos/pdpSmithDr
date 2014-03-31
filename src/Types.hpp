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

#ifndef SD_TYPES_HPP
#define SD_TYPES_HPP

namespace sd {

  typedef char INT8;
  typedef short INT16;
  typedef int INT32;
  typedef long int INT64;

  typedef unsigned char UINT8;
  typedef unsigned short UINT16;
  typedef unsigned int UINT32;
  typedef unsigned long int UINT64;

  typedef UINT32 UINT;
  typedef INT32 INT;

  typedef UINT8 BYTE;

  typedef float FLOAT;
  typedef double DOUBLE;
  typedef long double LONGDOUBLE;

  //:COMMENT:Pascal:19/04/10:if Z_AXIS=axial, Y_AXIS=sagittal, X_AXIS=frontal
  //:COMMENT:Cedric:01/09/11: WRONG!!! --> Z_AXIS=axial, Y_AXIS=frontal, X_AXIS=sagittal
  enum Orientation {
    X_AXIS = 0,
    Y_AXIS = 1,
    Z_AXIS = 2,

    Frontal  = Y_AXIS,
    Sagittal = X_AXIS,
    Axial    = Z_AXIS,

    NO_AXIS = ((unsigned int)-1)
  };

  enum ColorSpace {
    //BINARY,
    BITMAP,
    GRAYLEVEL,
    //FP, //:COMMENT:Pascal:30/07/10:For floating point values
    RGB,
    RGBA,
    USER_DEFINED
  };

  enum PixelStorage {
    ASCII,
    BINARY
  };

  enum ObjectType {
    UnknownType,
    MeshObjectType,
    ImageObjectType,
    AlgorithmObjectType
  };

  enum Destination {
    DISPLAY,
    COMPUTATION
  };

  enum QuantificationMethod {
    NONE,
    BINARIZATION,
    STANDARD,
    WINDOW
  };
    
}

#endif /*! SD_TYPES_HPP */
