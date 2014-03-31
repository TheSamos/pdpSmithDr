#ifndef QTXMLVALUEGENERATOR_HPP
#define QTXMLVALUEGENERATOR_HPP

#include <QtXml>
#include <QString>
#include <sstream>

//#include <frontend/lib/Parameter.hpp>
#include "SDRParameter.hpp"

namespace sd {
  
  namespace libqt {

class QtXMLValueGenerator 
{

public:
	QtXMLValueGenerator(): m_xml() {}

    std::string simpleXMLValue(SDRParameter* param);

private:
    QDomDocument m_xml;
};

}
}

#endif //QTXMLVALUEGENERATOR_HPP
