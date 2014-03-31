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

#ifndef SD_MESH_PROCESSING_COLOR_COMPUTE_COLORS_COMPUTER_HPP
#define SD_MESH_PROCESSING_COLOR_COMPUTE_COLORS_COMPUTER_HPP

#include <core/Clonable.hpp>

#include <core/mesh/MeshView_.hpp>

#include <string>

namespace sd {

  namespace meshprocessing {

    namespace colorcompute {
      
      class ColorComputer : public sd::core::Clonable {
  
      public:  
  
	virtual std::string name() = 0;

	virtual std::string getDescription() = 0;

	inline void setMin(double min) {m_min = min;}
	inline void setMax(double max) {m_max = max;}
	inline void setMinMax(double min, double max) {m_min = min; m_max = max;}
	inline void setCentredOnZero(bool centredOnZero) {m_centredOnZero = centredOnZero;}
	inline double getMin() const {return m_min;}
	inline double getMax() const {return m_max;}
	inline const sd::core::RGBCoordinates<unsigned char>& errorColor() const { return m_errorColor;}
	
	template<typename T, unsigned int N, typename U>
	inline void computeColors(sd::core::MeshView_<T, N>* mesh, sd::core::AttributeKey akey);
	
	template<typename T, unsigned int N, typename U>
	inline void computeColors(sd::core::MeshView_<T, N>* mesh, sd::core::AttributeKey akey,
				  unsigned int rank);
	
	virtual void computeColor(double value, sd::core::RGBCoordinates<unsigned char>& rgb) const = 0;
  
	virtual ColorComputer* clone() const = 0;
	
      protected:
  
	inline ColorComputer(double min, double max,
			     const sd::core::RGBCoordinates<unsigned char>& errorColor,
			     bool centredOnZero)
	  : m_min(min), m_max(max),
	    m_errorColor(errorColor),
	    m_centredOnZero(centredOnZero)
	{}
  
	inline double normalize(double value) const;

      protected:
	
	double m_min;
	double m_max;
  	
      private:

  	sd::core::RGBCoordinates<unsigned char> m_errorColor;
	bool m_centredOnZero;
	
      };

    }
    
  }
  
}

#include "ColorComputer.hxx"

#endif /* ! SD_MESH_PROCESSING_COLOR_COMPUTE_COLORS_COMPUTER_HPP */
