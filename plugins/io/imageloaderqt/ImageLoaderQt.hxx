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

#ifndef SD_IO_IMAGE_LOADER_QT_HXX
#define SD_IO_IMAGE_LOADER_QT_HXX

#include <QImage>

#include <Types.hpp>

#include <core/image/Image.hpp>

#include <iostream>

template<typename T>
sd::core::ImageView_<T> *
sd::io::ImageLoaderQt<T>::loadImage(const std::string &filename) const
{
    sd::core::Image<T> *image = NULL;
    QImage qimage(QString::fromUtf8(filename.c_str()));
    if (sd::core::DataType_<T>() == sd::core::DataType_<sd::UINT8>())
    {
        if (qimage.format() == QImage::Format_Indexed8)
        {
            std::cerr << "QImage::Format_Indexed8" << std::endl;
            return NULL;
            /*
            image = new typename sd::core::Image<T>(sd::core::ImageViewInfo(sd::GRAYLEVEL, sd::Z_AXIS,
                                        sd::Size(qimage.width(), qimage.height())),
                            filename);
            */
        }
        else if (qimage.format() == QImage::Format_RGB32)
        {
            std::cerr << "QImage::Format_RGB32" << std::endl;
            image = new typename sd::core::Image<T>(sd::core::ImageViewInfo(sd::RGB, sd::Z_AXIS,
                                                    sd::Size(qimage.width(), qimage.height())),
                                                    filename);
        }
        else if (qimage.format() == QImage::Format_ARGB32)
        {
            std::cerr << "QImage::Format_ARGB32" << std::endl;
            image = new typename sd::core::Image<T>(sd::core::ImageViewInfo(sd::RGBA, sd::Z_AXIS,
                                                    sd::Size(qimage.width(), qimage.height())),
                                                    filename);
#ifdef DEBUG
        }
        else if (qimage.format() == QImage::Format_Invalid)
        {
            std::cerr << "Unable to load with Qt" << std::endl;
        }
        else
        {
            std::cerr << "This plugin do not support with Qt format : " << qimage.format() << std::endl;
#endif
        }

    }
    else
    {
        return NULL;
    }

    if (!image)
        return NULL;

      //std::cout << "After return image null" << std::endl;

    const unsigned int nbChannels = image->nbChannels();
    const int dims = image->size().dataSize();
    const int size = dims * nbChannels;

    T *buf = image->getData();
    const T *end = image->getData() + size;
    const uchar *data =  qimage.bits();

    if (nbChannels > 1)
    {
        for (; buf < end; buf += nbChannels, data += 4)
        {
            *(buf + 0) = *(data + 2);
            if (nbChannels >= 3)
            {
                *(buf + 1) = *(data + 1);
                *(buf + 2) = *(data + 0);
                if (nbChannels > 3)
                    *(buf + 3) = *(data + 3);
            }
        }
    }
    else
    {
        memcpy(buf, qimage.bits(), size * sizeof(T));
    }

    return image;
}

#endif /*! SD_IO_IMAGE_LOADER_QT_HXX */
