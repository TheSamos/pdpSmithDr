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

#ifndef SD_MESH_PROCESSING_COLOR_COMPUTE_MESH_DOUBLE_THRESHOLD_COLORS_COMPUTER_HXX
#define SD_MESH_PROCESSING_COLOR_COMPUTE_MESH_DOUBLE_THRESHOLD_COLORS_COMPUTER_HXX

sd::meshprocessing::colorcompute::ColorDoubleThresholdColorsComputer::ColorDoubleThresholdColorsComputer(double min, double max,
										   bool centredOnZero)
  : ColorComputer(min, max, sd::core::RGBCoordinates<UINT8>((UINT8)255, (UINT8)255, (UINT8)255), centredOnZero)
{}

std::string
sd::meshprocessing::colorcompute::ColorDoubleThresholdColorsComputer::name() {
  return "DoubleThreshold";
}

std::string
sd::meshprocessing::colorcompute::ColorDoubleThresholdColorsComputer::getDescription() {
  return "";
}

void
sd::meshprocessing::colorcompute::ColorDoubleThresholdColorsComputer::computeColor(double value,
										   sd::core::RGBCoordinates<unsigned char>& rgb) const {
  if (std::isnan(value)) {
    rgb = errorColor();
    return;
  }
  
  if (value >= m_max)
    rgb = sd::core::RGBCoordinates<unsigned char>((uchar)255, (uchar)0, (uchar)0);
  else if (value <= m_min)
    rgb = sd::core::RGBCoordinates<unsigned char>((uchar)0, (uchar)0, (uchar)255);
  else
    rgb = sd::core::RGBCoordinates<unsigned char>((uchar)0, (uchar)0, (uchar)0);
}

sd::meshprocessing::colorcompute::ColorComputer*
sd::meshprocessing::colorcompute::ColorDoubleThresholdColorsComputer::clone() const {
  return new ColorDoubleThresholdColorsComputer(*this);
}


#endif /* ! SD_MESH_PROCESSING_COLOR_COMPUTE_MESH_DOUBLE_THRESHOLD_COLORS_COMPUTER_HXX */
