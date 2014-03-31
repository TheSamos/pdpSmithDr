#ifndef QTXMLVALUEPARSER_HPP
#define QTXMLVALUEPARSER_HPP


#include <iostream>
#include <frontend/lib/Parameter.hpp>
#include <QtXml>


bool namespace_processing = true;

namespace sd {
  
  namespace libqt {

class QtXMLValueParser
{

public:
    QtXMLValueParser(std::string xml_string);
    ~QtXMLValueParser();

    frontend::Parameter parseSimpleParameter();
    //frontend::Parameter parseComplexParameter();


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
