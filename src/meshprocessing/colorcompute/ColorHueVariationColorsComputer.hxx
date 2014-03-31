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

#ifndef SD_MESH_PROCESSING_COLOR_COMPUTE_MESH_HUE_VARIATION_COLORS_COMPUTER_HXX
#define SD_MESH_PROCESSING_COLOR_COMPUTE_MESH_HUE_VARIATION_COLORS_COMPUTER_HXX

#include <QColor>

sd::meshprocessing::colorcompute::ColorHueVariationColorsComputer::ColorHueVariationColorsComputer(double min, double max,
												   bool centredOnZero)
  : ColorComputer(min, max, sd::core::RGBCoordinates<UINT8>((UINT8)0, (UINT8)0, (UINT8)0), centredOnZero)
{}

std::string
sd::meshprocessing::colorcompute::ColorHueVariationColorsComputer::name() {
  return "HueVariation-Hotmetal";
}

std::string
sd::meshprocessing::colorcompute::ColorHueVariationColorsComputer::getDescription() {
  return "";
}

void
sd::meshprocessing::colorcompute::ColorHueVariationColorsComputer::computeColor(double value,
										sd::core::RGBCoordinates<unsigned char>& rgb) const {
  
  if (std::isnan(value)) {
    rgb = errorColor();
    return;
  }
  
  double hue = ColorComputer::normalize(value) * 2. - 1.;
  
  hue = (1-1/pow(10, fabs(hue))) * ((hue != 0) ? (hue)/fabs(hue) : 1);
  hue += 1;
  hue /= 2;
  hue = (1-hue) * 2/3;
  
  QColor falseColor;
  falseColor.setHsvF(hue, .9, .9);
  
  QColor qrgb = falseColor.toRgb().rgb();
  rgb = sd::core::RGBCoordinates<unsigned char>(qrgb.red(),
						qrgb.green(),
						qrgb.blue());
}

sd::meshprocessing::colorcompute::ColorComputer*
sd::meshprocessing::colorcompute::ColorHueVariationColorsComputer::clone() const {
  return new ColorHueVariationColorsComputer(*this);
}

#endif /* ! SD_MESH_PROCESSING_COLOR_COMPUTE_MESH_HUE_VARIATION_COLORS_COMPUTER_HXX */
