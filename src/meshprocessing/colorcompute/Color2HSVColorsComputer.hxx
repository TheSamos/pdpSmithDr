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

#ifndef SD_MESH_PROCESSING_COLOR_COMPUTE_MESH_2_HSV_COLORS_COMPUTER_HXX
#define SD_MESH_PROCESSING_COLOR_COMPUTE_MESH_2_HSV_COLORS_COMPUTER_HXX

#include <QColor>

sd::meshprocessing::colorcompute::Color2HSVColorsComputer::Color2HSVColorsComputer(double min,
										   double max,
										   bool centredOnZero)
  : ColorComputer(min, max, sd::core::RGBCoordinates<UINT8>((UINT8)0, (UINT8)0, (UINT8)0), centredOnZero)
{}

std::string
sd::meshprocessing::colorcompute::Color2HSVColorsComputer::name() {
  return "B.Hamann_Colorization";
}

std::string
sd::meshprocessing::colorcompute::Color2HSVColorsComputer::getDescription() {
  return "";
}

void
sd::meshprocessing::colorcompute::Color2HSVColorsComputer::computeColor(double value,
									sd::core::RGBCoordinates<unsigned char>& rgb) const {
  
  if (std::isnan(value)) {
    rgb = errorColor();
    return;
  }
  
  value = ColorComputer::normalize(value) * 2. - 1.;
  
  double minH = 0., minS = 0., minV = 0.,
    maxH = 0., maxS = 0., maxV = 0.;
  
  if (value >= 0) {
    minH = 1.; minS = 1.; minV = 1./2.;
    maxH = 1./2.; maxS = 1.; maxV = 1.;
  } else {
    minH = 1./3.; minS = 1.; minV = 1./3.;
    maxH = 1./6.; maxS = 1.; maxV = 1.;
    value += 1;
  }
  
  double h, s, v;
  if (maxH >= minH)
    h = (maxH != minH) ? value * fabs(maxH-minH) + minH : maxH;
  else
    h = (1 - value) * fabs(maxH-minH) + maxH;
  
  if (maxS >= minS)
    s = (maxS != minS) ? value * fabs(maxS-minS) + minS : maxS;
  else
    s = (1 - value) * fabs(maxS-minS) + maxS;
  
  if (maxV >= minV)
    v = (maxV != minV) ? value * fabs(maxV-minV) + minV : maxV;
  else
    v = (1 - value) * fabs(maxV-minV) + maxV;
  
  QColor falseColor;
  falseColor.setHsvF(h, s, v);
  
  QColor qrgb = falseColor.toRgb().rgb();
  rgb = sd::core::RGBCoordinates<unsigned char>(qrgb.red(),
						qrgb.green(),
						qrgb.blue());
}

sd::meshprocessing::colorcompute::ColorComputer*
sd::meshprocessing::colorcompute::Color2HSVColorsComputer::clone() const {
  return new Color2HSVColorsComputer(*this);
}

#endif /* ! SD_MESH_PROCESSING_COLOR_COMPUTE_MESH_2_HSV_COLORS_COMPUTER_HXX */
