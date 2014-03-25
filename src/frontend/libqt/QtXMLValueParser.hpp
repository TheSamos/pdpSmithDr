#ifndef QTXMLVALUEPARSER_HPP
#define QTXMLVALUEPARSER_HPP

#include <QtXml>
#include <iostream>
#include <frontend/libqt/Parameter.hpp>

static const bool namespace_processing = true;

namespace sd {
  
  namespace libqt {

class QtXMLValueParser
{


public:
    QtXMLValueParser(std::string xml_string);
    ~QtXMLValueParser();

    sd::libqt::Parameter *parseSimpleParameter();
    sd::libqt::Parameter *parseComplexParameter();


private:
    int initialize();

private:
    std::string m_xmlstring;
    QDomDocument m_qdoc;

    int *m_err_line;
    int *m_err_column;
    QString *m_error_msg;

};

}
}

#endif // QTXMLVALUEPARSER_HPP
