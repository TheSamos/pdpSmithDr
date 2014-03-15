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

#ifndef SD_IO_IMAGE_SAVER_QT_HXX
#define SD_IO_IMAGE_SAVER_QT_HXX

#include "ImageSaverQt.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#include <core/image/Image.hpp>
#include <QColor>
#include <QImage>
#include <QString>

namespace sd {
  
  namespace io {

    template<typename T>
    bool
    ImageSaverQt<T>::saveImage(const core::ImageView_<T>* image, const std::string& fileURI) const {
      
      if (sd::core::DataType_<T>() != sd::core::DataType_<sd::UINT8>())
	return NULL;
      
      QImage::Format format = QImage::Format_Invalid;
      
      if (image->colorSpace() == GRAYLEVEL) {
	format = QImage::Format_Mono;
	
      } else if (image->colorSpace() == RGB) {
	format = QImage::Format_RGB32;
      } else if (image->colorSpace() == RGBA) {
	format = QImage::Format_ARGB32;
      }
#ifdef DEBUG
      else
	std::cerr << "Unable to save with Qt" << std::endl;
#endif
      
      QImage qimage(image->size().width(), image->size().height(), format);
      
      typename sd::core::ImageView_<T>::const_iterator it = image->begin();
      for(int i=0; i<qimage.width()*qimage.height(); ++i, ++it) {
	int x = i%qimage.width();
	int y = i/qimage.width();
	if (image->colorSpace() == GRAYLEVEL)
	  qimage.setPixel(x, y, QColor(*it, *it, *it).rgb());
	else if (image->colorSpace() == RGB) {
	  qimage.setPixel(x, y, QColor(it(0), it(1), it(2)).rgb());
	} else if (image->colorSpace() == RGBA)
	  qimage.setPixel(x, y,  QColor(it(0), it(1), it(2), it(3)).rgb());
      }
      
      return qimage.save(QString::fromStdString(fileURI));
      
    }
    
  }
  
}

#endif /*! SD_IO_IMAGE_SAVER_QT_HXX */
