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

#ifndef SD_IMAGE_PROCESSING_FILTER_HPP
#define SD_IMAGE_PROCESSING_FILTER_HPP

#include <vector>

#include <core/image/ImageView_.hpp>

namespace sd {
  namespace imageprocessing {

    template<typename T>
    class Filter
    {

    public:

      Filter(const double* kernel, size_t width, size_t height, size_t depth);
      virtual ~Filter();

      virtual bool applyTo(sd::core::Image<T>& dest, const sd::core::ImageView_<T>& src) = 0;
      
      inline const std::vector<double>& getKernel() const { return m_kernel; }
      inline size_t getWidth() const { return m_width; }
      inline size_t getHeight() const { return m_height; }
      inline size_t getDepth() const { return m_depth; }

    protected:

      inline std::vector<double>& kernel() { return m_kernel; }
      inline size_t width() const { return m_width; }
      inline size_t height() const { return m_height; }
      inline size_t depth() const { return m_depth; }
      inline bool hasNegativeValues() const;

    private:

      Filter(const Filter& f) = delete;
      Filter& operator=(const Filter& f) = delete;

    private:

      std::vector<double> m_kernel;
      size_t m_width;
      size_t m_height;
      size_t m_depth;

    };

    template<typename T> bool isUnsignedType();

  }
}

#include "Filter.hxx"

#endif /* #! SD_IMAGE_PROCESSING_FILTER_HPP */
