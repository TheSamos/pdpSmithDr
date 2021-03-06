#include "QtXMLParamParser.hpp"
//#include "SimpleParameter.hpp"
//#include <frontend/libqt/SimpleParameter.hpp>
//#include <frontend/lib/Parameter.hpp>

#include "SDRParameterFactory.hpp"

#include <iostream>
namespace sd
{

namespace libqt
{

QtXMLParamParser::QtXMLParamParser(std::string xml_string) : m_xmlstring(xml_string)
{
    initialize();
}

QtXMLParamParser::~QtXMLParamParser() {}

/*QtXMLParamParser::QtXMLParamParser(QFile input_file)
{

}*/

int QtXMLParamParser::initialize()
{
    //loadXSD();

    bool hasParsed = m_qdoc.setContent(QString::fromStdString(m_xmlstring), namespace_processing,
                                       m_error_msg, m_err_line, m_err_column);

    if (!hasParsed)
    {
        std::cout << "Parser failed line " << m_err_line << ", column " << m_err_column << " : ";
        std::cout << m_error_msg->toStdString() << std::endl;
    }

    return 1;

}

bool QtXMLParamParser::loadXSD()
{
    QFile file("phony");
    file.open(QIODevice::ReadOnly);

    m_schema.load(QUrl::fromLocalFile(file.fileName()));

    if (!m_schema.isValid())
    {
        qDebug() << "schema is invalid";
        return false;
    }

    return true;
}

sd::libqt::ParameterList QtXMLParamParser::getParameterList()
{
    sd::libqt::ParameterList parameters;

    QDomElement param_root = m_qdoc.firstChildElement("parameters");
    QDomElement param = param_root.firstChildElement("parameter");

    for (; !param.isNull(); param = param.nextSiblingElement("parameter"))
    {
        QDomAttr param_type = param.attributeNode("type");

        //if(param_type.value() == "simple")
        //parameters.push_back(parseComplexParameter(param));
        //else
        parameters.push_back(parseSimpleParameter(param));

    }

    return parameters;


}

//void QtXMLParamParser::getParameter(std::string name) {}


SDRParameter *QtXMLParamParser::parseSimpleParameter(QDomElement param)
{
    QDomAttr param_type = param.attributeNode("type");
    std::string name = param.attributeNode("name").value().toStdString();
    std::string type = param.attributeNode("type").value().toStdString();

    std::map<std::string, QString> elements;  

    QDomElement element; 
    for (unsigned int i = 0; i < m_element_names.size(); i++)
    {
        element = param.namedItem(QString::fromStdString(m_element_names[i])).toElement();
        if(!element.isNull())
        {
            elements[m_element_names[i]] = element.text();
            //std::cout << "Just added: " << m_element_names[i] << ", " << elements[m_element_names[i]].toStdString() << std::endl;
        }
    }
    
    element = param.namedItem(QString::fromStdString("widget")).toElement();
    std::string widget = element.text().toStdString();

    //std::cout << "Parameter type: " << elements["type"].toStdString() << std::endl;

    SDRParameterFactory paramFactory;
    SDRParameter *p = paramFactory.create(type, name, elements, widget);

    std::cout << "Parsed simple parameter: ";
    p->print();

    return p;
}


//frontend::Parameter QtXMLParamParser::parseComplexParameter(QDomElement complex_param) { return nullptr; }

}
}
















