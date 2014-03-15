/*
  Copyright (C) 2011 The SmithDR team, smithdr@labri.fr

  This file is part of SmithDR.

  SmithDR is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  SmithDR is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with SmithDR.
  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SD_MESH_PROCESSING_COLOR_COMPUTE_COLORS_COMPUTER_HXX
#define SD_MESH_PROCESSING_COLOR_COMPUTE_COLORS_COMPUTER_HXX

template<typename T, unsigned int N, typename U>
void 
sd::meshprocessing::colorcompute::ColorComputer::computeColors(sd::core::MeshView_<T, N>* mesh, sd::core::AttributeKey akey) {
  typename sd::core::MeshView_<T, N>::vertices_const_geometry_iterator it = mesh->beginVerticesConstGeometry();
  const typename sd::core::MeshView_<T, N>::vertices_const_geometry_iterator end = mesh->endVerticesConstGeometry();
	  
  sd::core::RGBCoordinates<unsigned char> rgb;
  for (; it != end; ++it) {
    if ((*it).isAttributeSet(akey)) {
      U value = (*it).template getAttribute<U>(akey);
      if (!std::isnan(value)) {
	computeColor(value, rgb);
      } else {
	rgb = m_errorColor;
      }
    }
    (*it).setRGBCoordinates(rgb);
  }
}

template<typename T, unsigned int N, typename U>
void 
sd::meshprocessing::colorcompute::ColorComputer::computeColors(sd::core::MeshView_<T, N>* mesh, sd::core::AttributeKey akey,
						unsigned int rank) {
  typename sd::core::MeshView_<T, N>::vertices_const_geometry_iterator it = mesh->beginVerticesConstGeometry();
  const typename sd::core::MeshView_<T, N>::vertices_const_geometry_iterator end = mesh->endVerticesConstGeometry();
	  
  sd::core::RGBCoordinates<unsigned char> rgb;
  for (; it != end; ++it) {
    if ((*it).isAttributeSet(akey)) {
      U value = (*it).template getAttribute<sd::core::DynamicDimensionalCoordinates<U> >(akey).getAt(rank);
      if (!std::isnan(value)) {
	computeColor(value, rgb);
      } else {
	rgb = m_errorColor;
      }
    }
    (*it).setRGBCoordinates(rgb);
  }
}

double
sd::meshprocessing::colorcompute::ColorComputer::normalize(double value) const {

  double result;
  
  if (m_centredOnZero) {
    double delta1 = 0 - m_min;
    double delta2 = m_max - 0;
    if (value > 0. && delta2 != 0.) {
      result = std::max(std::min((value) / delta2, 1.),
			0.);
    } else if (value < 0. && delta1 != 0.) {
      result = -1. * (1-std::max(std::min((value - m_min) / delta1, 1.),
				 0.));
    } else {
      result = 0;
    }
  } else {
    double delta = fabs(m_max - m_min);  
    if (delta != 0)
      result = std::max(std::min(((value - m_min) * 2. / delta) - 1., 1.),
			-1.);
    else
      result = 0;
  }
  
  result = (result + 1.) / 2.;
  
  assert(result >= 0. && result <= 1.);
  
  return result;
}

#endif /* ! SD_MESH_PROCESSING_COLOR_COMPUTE_COLORS_COMPUTER_HXX */
