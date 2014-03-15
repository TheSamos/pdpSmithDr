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

#include <SmithDRDefs.hpp>

#include <core/image/ImageViewInfo.hpp>
#include <core/image/Image.hpp>
#include <core/image/SliceArray.hpp>

#include <iostream>


void
testOnGrayImage()
{
  
  sd::Size sz(5, 4, 6);
  size_t n = sz.dataSize();
  int* data = new int[n];
  for (size_t i = 0; i < n; ++i)
    data[i] = i;

  sd::core::ImageViewInfo info(sd::GRAYLEVEL, sd::Z_AXIS, sz);
  sd::core::Image<int> img(info);
  img.setData(data);

  sd::core::Slice s(2, 7, 5);
  sd::core::SliceArray<int> sa = img.getSlice(s);

  // 7 values: 2, 7, 12, 17, 22, 27, 32
  std::cout << "First example\n";
  for (auto it = sa.begin(); it != sa.end(); ++it)
    std::cout << *it << "  ";
  std::cout << std::endl;
  std::cout << std::endl;

  // 2*3 values: 1, 3, 5, 8, 10, 12
  std::cout << "Second example\n";
  s = sd::core::Slice(1, {2,3}, {7,2});
  sa = img.getSlice(s);
  for (auto it = sa.begin(); it != sa.end(); ++it)
    std::cout << *it << "  ";
  std::cout << std::endl;
  std::cout << std::endl;

  // 6*4 values: 2,7,12,17, 22,27,32,37, ..., 102,107,112,117
  std::cout << "Slice X=2\n";
  sa = img.getSlice(sd::X_AXIS, 2);
  for (auto it = sa.begin(); it != sa.end(); ++it)
    std::cout << *it << "  ";
  std::cout << std::endl;

  std::cout << "The same, testing iterator post-increment\n";
  for (auto it = sa.begin(); it != sa.end();)
    std::cout << *it++ << "  ";
  std::cout << std::endl;
  std::cout << std::endl;

  // 6*5 values: 10,11,12,13,14, 30,31,32,33,34, ..., 110,111,112,113,114
  std::cout << "Slice Y=2\n";
  sa = img.getSlice(sd::Y_AXIS, 2);
  for (auto it = sa.begin(); it != sa.end(); ++it)
    std::cout << *it << "  ";
  std::cout << std::endl;
  std::cout << std::endl;

  // 4*5 values: 40,41,42,...,59
  std::cout << "Slice Z=2\n";
  sa = img.getSlice(sd::Z_AXIS, 2);
  for (auto it = sa.begin(); it != sa.end(); ++it)
    std::cout << *it << "  ";
  std::cout << std::endl;
  std::cout << std::endl;
}

void
testOnRGBImage()
{
  sd::Size sz(5, 4, 6);
  sd::UINT16 nchan = 3;
  size_t n = sz.dataSize();
  float* data = new float[n*nchan];
  for (size_t i = 0; i < n; ++i) {
    data[nchan*i+0] = i+1 + 0.1f;
    data[nchan*i+1] = i+1 + 0.2f;
    data[nchan*i+2] = i+1 + 0.3f;
  }

  sd::core::ImageViewInfo info(sd::RGB, sd::Z_AXIS, sz);
  sd::core::Image<float> img(info);
  img.setData(data);

  sd::core::SliceArray<float> sa = img.getSlice(sd::Z_AXIS, 2);

  // 5*4*3 values: pixels 41 to 60 with decimals .1, .2, .3 for each
  std::cout << "First example\n";
  for (auto it = sa.begin(); it != sa.end(); ++it)
    std::cout << *it << "  ";
  std::cout << std::endl;
}

int
main(int /*argc*/, char* /*argv*/[])
{
  
  std::cout << "Testing on Graylevel image\n";
  testOnGrayImage();

  std::cout << "\nTesting on RGB image\n";
  testOnRGBImage();
  
  return EXIT_SUCCESS;
}
