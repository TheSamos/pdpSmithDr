#ifndef QTXMLVALUEGENERATOR_HPP
#define QTXMLVALUEGENERATOR_HPP

#include <QtXml>
#include <QString>
#include <sstream>

#include <frontend/libqt/Parameter.hpp>

namespace sd {
  
  namespace libqt {

class QtXMLValueGenerator {

public:
    std::string simpleXMLValue(Parameter *param);
    QString intToQString(int n);

private:
    QDomDocument m_xml;
};

}
}

#endif //QTXMLVALUEGENERATOR_HPP
