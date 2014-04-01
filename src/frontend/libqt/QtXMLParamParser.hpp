#ifndef QTXMLPARAMPARSER_HPP
#define QTXMLPARAMPARSER_HPP

#include <QtXml>
#include <QXmlSchema>
#include <map>
#include <vector>
//#include <frontend/lib/Parameter.hpp>
#include "SDRParameter.hpp"
#include "SimpleParameter.hpp"


namespace sd {
  
  namespace libqt {
    
static const std::string schema_file_location;
static const bool namespace_processing = true;

/* Class that parses an XML file that represents */
/* a list of parameters.                         */

class QtXMLParamParser
{

public:
    QtXMLParamParser(std::string xml_string);
    /*QtXMLParamParser(QFile input_file);*/
    ~QtXMLParamParser();

    sd::libqt::ParameterList getParameterList();
    void getParameter(std::string name);

    SDRParameter* parseSimpleParameter(QDomElement simple_param);
    //frontend::Parameter *parseComplexParameter(QDomElement complex_param);


private:
    int initialize();
    bool loadXSD();

    void fillSimpleParamFromMap(std::map<std::string, QDomElement>& elements, SDRParameter *param);


private:
    std::vector<std::string> m_element_names = {"min", "max", "default", "widget"};

    std::string m_xmlstring;
    QDomDocument m_qdoc;
    QXmlSchema m_schema;

    int *m_err_line;
    int *m_err_column;
    QString *m_error_msg;

};
}
}

#endif // QTXMLPARAMPARSER_HPP
