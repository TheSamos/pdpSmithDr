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

#include <iostream>
#include <fstream>
#include <math.h>

#include <Types.hpp>
#include <utils/sdTypeUtils.hpp>

typedef double T;

int
main(int argc, char* argv[]) {    
  
  for (double a=-1.; a<=1.; a+=.1) {
    
    for (double b=-1.; b<=1.; b+=.1) {
      
      T ampl = sqrt(a*a+b*b);
      
      T phi1 = 0;
      if (a >= 0) {
	if (a != 0.)
	  phi1 = -atan(b/a);
	else
	  phi1 = -fabs(b)/b * M_PI/2.;
      } else {
	phi1 = -(M_PI + atan(b/a));
      }
      
      T phi2 = 0;
      if (a >= 0) {
	if (a != 0.)
	  phi2 = -atan(b/a)/2.;
	else
	  phi2 = -fabs(b)/b * M_PI/4.;
      } else {
	phi2 = -(M_PI + atan(b/a))/2.;
      }
      
      std::string plot(std::string("/usr/bin/gnuplot -e \"set terminal png; set output '/tmp/plotGlobal")
		       + a +"_"+ b +".png'; set xrange[-2*pi:2*pi]; set yrange[-1:1]; plot ");
      plot += std::string(" (")+a+")*cos(x)+("+b+")*sin(x) title 'CosSinNormal' lc rgb '#888888'";
      plot += std::string(", (")+ampl+")*cos(x+("+phi1+")) title 'ExpCosSinNormal' lc rgb '#FF0000'";
      plot += std::string(", (")+a+")*cos(2*x)+("+b+")*sin(2*x) title 'CosSinNormal2' lc rgb '#0000FF'";
      plot += std::string(", (2*")+ampl+")*cos(x+("+phi2+"))*cos(x+("+phi2+"))-"+(ampl)+" title 'ExpCosSinNormal2' lc rgb '#00FF00'";
      plot += std::string(", 0 title 'Zero' lc rgb '#000000'\"");
      
      system(plot.c_str());
      
    }
    
  }
  
}
