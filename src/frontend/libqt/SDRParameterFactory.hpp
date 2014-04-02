#ifndef SDRPARAMETERFACTORY_HPP
#define SDRPARAMETERFACTORY_HPP

#include <string>
#include <vector>
#include <map>
#include <QString>

#include "SDRParameter.hpp"

namespace sd {
  
  namespace libqt {


 class SDRParameterFactory
 {

public:

 	SDRParameter *create(std::string type, std::string name, 
 		std::map<std::string, QString> map, std::string widget = "");

 };


}
}


#endif