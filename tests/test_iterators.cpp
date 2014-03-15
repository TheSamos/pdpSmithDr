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

#include <core/image/Image.hpp>
#include <core/image/iterators/Iterator.hpp>
#include <core/image/iterators/PointerIterator.hpp>

#include <iostream>
#include <vector>
#include <stdexcept>
#include <iterator>

template<typename T, size_t SizeT>
class StaticArray
{
protected:
  T* m_pData;

protected:
  void __allocate()
  { m_pData = new T[SizeT]; if (!m_pData) throw std::runtime_error("!m_pData"); }
  void __free()
  { if (m_pData) delete[] m_pData; }

public:
  typedef sd::core::ConstIterator<T> const_iterator;
  typedef sd::core::Iterator<T> iterator;

  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;

  StaticArray() : m_pData(NULL) { __allocate(); }
  StaticArray(const StaticArray& r) : m_pData(NULL)
  {
    __allocate();
    copy(r.m_pData, r.m_pData + SizeT, m_pData);
  }

  virtual ~StaticArray() { __free(); }

  StaticArray<T,SizeT>&
  operator=(const StaticArray<T,SizeT>& rhs)
  {
    if (&rhs != this) {
      __allocate();
      copy(rhs.m_pData, rhs.m_pData + SizeT, m_pData);
    }

    return *this;
  }

  const T& operator[](size_t ulInd) const
  { return m_pData[ulInd]; }

  T& operator[](size_t ulInd)
  { return m_pData[ulInd]; }

  const_iterator begin() const
  { return const_iterator(new sd::core::PointerIterator<T>(m_pData)); }

  const_iterator end() const
  { return const_iterator(new sd::core::PointerIterator<T>(m_pData + SizeT)); }

  const_reverse_iterator rbegin() const
  { return const_reverse_iterator(end()); }

  const_reverse_iterator rend() const
  { return const_reverse_iterator(begin()); }

  iterator begin()
  { return iterator(new sd::core::PointerIterator<T>(m_pData)); }

  iterator end()
  { return iterator(new sd::core::PointerIterator<T>(m_pData + SizeT)); }

  reverse_iterator rbegin()
  { return reverse_iterator(end()); }

  reverse_iterator rend()
  { return reverse_iterator(begin()); }
};

template<class C>
void
runTest(C* c)
{
  typename C::iterator it = c->begin();
  typename C::const_iterator cit;
  typename C::iterator cit2;
  cit = it;
  cit2 = it;

  typename C::reverse_iterator rit = c->rbegin();
  typename C::reverse_iterator rEnd = c->rend();
  typename C::const_reverse_iterator rcit = rit;
  typename C::const_reverse_iterator rcEnd;

  rcEnd = rEnd;

  std::cout << "Dist: " << distance(rcit, rcEnd) << std::endl;
  copy(rcit, rcEnd, std::ostream_iterator<float>(std::cout, "\t"));
  std::cout << std::endl;
  
  std::vector<float> vfVals;
  copy(c->begin(), c->end(), back_inserter(vfVals));
  copy(vfVals.begin() + 4, vfVals.end(), std::ostream_iterator<float>(std::cout, "\t"));

  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << std::boolalpha;

  // test all operators
  cit = c->begin(); std::cout << *cit << std::endl; // on first element
  ++cit; std::cout << *cit << std::endl; // on second element
  cit++; std::cout << *cit << std::endl; // on third element
  *cit++; std::cout << *cit << std::endl; // on fourth element

  bool b = (cit == c->begin()); std::cout << b << std::endl; // false
  b = (cit != c->begin()); std::cout << b << std::endl; // true

  float f = *cit; std::cout << f << std::endl; // fourth element
  *cit = 3; std::cout << *cit << std::endl; // fourth element is 3
  *cit++ = 7; std::cout << *cit << std::endl; // fourth element is 7, on fifth element

  copy(c->begin(), c->end(), std::ostream_iterator<float>(std::cout, "\t"));
  std::cout << std::endl;

  --cit; std::cout << *cit << std::endl; // on fourth element
  cit--; std::cout << *cit << std::endl; // on third element
  *cit--; std::cout << *cit << std::endl; // on second element

  auto tmp = cit + 4; std::cout << *tmp << std::endl; // tmp on sixth element
  tmp = 4 + cit; std::cout << *tmp << std::endl; // tmp on sixth element

  cit = tmp - 3 ; std::cout << *cit << std::endl; // cit on third element
  int n = tmp - cit; std::cout << n << std::endl; // 3
  n = cit - tmp; std::cout << n << std::endl; // -3

  b = (cit < tmp); std::cout << b << std::endl; // true
  b = (cit > tmp); std::cout << b << std::endl; // false
  b = (cit <= tmp); std::cout << b << std::endl; // true
  b = (cit >= tmp); std::cout << b << std::endl; // false

  cit += 3; std::cout << *cit << std::endl; // cit on sixth element
  cit -= 3; std::cout << *cit << std::endl; // cit on third element

  n = cit[2]; std::cout << n << std::endl; // fifth element
}

void
testOnArray()
{
  StaticArray<float, 10>* arr = new StaticArray<float, 10>;
  for (size_t u = 0; u < 10; ++u)
    (*arr)[u] = u + 1;

  runTest(arr);

  delete arr;
}

void
testOnImage()
{
  const int width = 3;
  const int height = 4;
  int *data = new int[12];
  for (int i = 0; i < width*height; ++i)
    data[i] = i+1;

  const sd::Size sz(width, height);

  sd::core::ImageViewInfo info(sd::GRAYLEVEL, sd::Z_AXIS, sz);
  sd::core::Image<int>* img = new sd::core::Image<int>(info);
  img->setData(data);

  runTest(img);

  delete img;
}

void
testOnRGBImage()
{
  const int width = 3;
  const int height = 4;
  const int nchan = 3;
  float *data = new float[width*height*nchan];
  for (int i = 0; i < width*height; ++i) {
    data[nchan*i+0] = i+1 + 0.1;
    data[nchan*i+1] = i+1 + 0.2;
    data[nchan*i+2] = i+1 + 0.3;
  }

  const sd::Size sz(width, height);

  sd::core::ImageViewInfo info(sd::RGB, sd::Z_AXIS, sz);
  sd::core::Image<float>* img = new sd::core::Image<float>(info);
  img->setData(data);

  for (auto it = img->begin(); it != img->end(); ++it) {
    std::cout << "RGB = [ ";
    std::cout << it(0) << ", " << it(1) << ", " << it(2);
    std::cout << " ]\n";
  }
  std::cout << std::endl;

  delete img;
}

int
main(int /*argc*/, char* /*argv*/[])
{
  std::cout << "Test with array\n";
  testOnArray();
  std::cout << std::endl << std::endl;

  std::cout << "Test with image\n";
  testOnImage();

  std::cout << "Test with RGB image\n";
  testOnRGBImage();

  return EXIT_SUCCESS;
}
