#ifndef QTXMLVALUEGENERATOR_HPP
#define QTXMLVALUEGENERATOR_HPP

#include <QtXml>
#include <QString>
#include <sstream>

#include "parameterTmp.h"

namespace sd {
  
  namespace libqt {

class QtXMLValueGenerator {

public:
    std::string simpleXMLValue(parameterTmp);
    QString intToQString(int n);

private:
    QDomDocument m_xml;
};

}
}

#endif //QTXMLVALUEGENERATOR_HPP
