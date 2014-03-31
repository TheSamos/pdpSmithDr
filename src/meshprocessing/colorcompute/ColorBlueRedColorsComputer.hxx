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

#ifndef SD_MESH_PROCESSING_COLOR_COMPUTE_MESH_BLUE_RED_COLORS_COMPUTER_HXX
#define SD_MESH_PROCESSING_COLOR_COMPUTE_MESH_BLUE_RED_COLORS_COMPUTER_HXX

sd::meshprocessing::colorcompute::ColorBlueRedColorsComputer::ColorBlueRedColorsComputer(double min, double max,
											 bool centredOnZero)
  : ColorComputer(min, max, sd::core::RGBCoordinates<UINT8>((UINT8)0, (UINT8)0, (UINT8)0), centredOnZero)
{}

std::string
sd::meshprocessing::colorcompute::ColorBlueRedColorsComputer::name() {
  return "BlueRed";
}

std::string
sd::meshprocessing::colorcompute::ColorBlueRedColorsComputer::getDescription() {
  return "";
}


void
sd::meshprocessing::colorcompute::ColorBlueRedColorsComputer::computeColor(double value,
									      sd::core::RGBCoordinates<unsigned char>& rgb) const {
  
  if (std::isnan(value)) {
    rgb = errorColor();
    return;
  }
  
  double result = ColorComputer::normalize(value);
  
  rgb = sd::core::RGBCoordinates<unsigned char>(result*255, 0, 255-result*255);
}

sd::meshprocessing::colorcompute::ColorComputer*
sd::meshprocessing::colorcompute::ColorBlueRedColorsComputer::clone() const {
  return new ColorBlueRedColorsComputer(*this);
}

#endif /* ! SD_MESH_PROCESSING_COLOR_COMPUTE_MESH_BLUE_RED_COLORS_COMPUTER_HXX */
