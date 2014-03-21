#include "QtXMLParamParser.hpp"
#include "parameterTmp.hpp"


#include <iostream>
namespace sd {
  
  namespace libqt {

QtXMLParamParser::QtXMLParamParser(std::string xml_string) : m_xmlstring(xml_string)
{
    initialize();
}

QtXMLParamParser::~QtXMLParamParser(){}

/*QtXMLParamParser::QtXMLParamParser(QFile input_file)
{

}*/

int QtXMLParamParser::initialize()
{
    //loadXSD();

    bool hasParsed = m_qdoc.setContent(QString::fromStdString(m_xmlstring), namespace_processing,
                      m_error_msg, m_err_line, m_err_column);

    if(!hasParsed)
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

sd::frontend::ParameterList QtXMLParamParser::getParameterList()
{
    sd::frontend::ParameterList parameters;
  
    QDomElement param_root = m_qdoc.firstChildElement("parameters");
    QDomElement param = param_root.firstChildElement("parameter");

    for(; !param.isNull(); param = param.nextSiblingElement("parameter"))
    {
        QDomAttr param_type = param.attributeNode("type");

        if(param_type.value() == "complex")
            parameters.push_back(parseComplexParameter(param));
        else
            parameters.push_back(parseSimpleParameter(param));

    }

    return parameters;


}

void QtXMLParamParser::getParameter(std::string name){}

sd::frontend::Parameter QtXMLParamParser::parseSimpleParameter(QDomElement param)
{
    QDomAttr param_type = param.attributeNode("type");
    std::string name = param.attributeNode("name").value().toStdString();
    std::string type = param.attributeNode("type").value().toStdString();
    int min, max, default_val;
    //std::cout << "simple parameter: ";
    //std::cout << " name: " << (param.attributeNode("name").value().toStdString());
    //std::cout << ", ẗype: " << (param.attributeNode("type").value().toStdString()) << std::endl;

    QDomElement param_elem = param.firstChildElement();

    default_val = param_elem.text().toInt();
    param_elem = param_elem.nextSiblingElement();
    min = param_elem.text().toInt();
    param_elem = param_elem.nextSiblingElement();
    max = param_elem.text().toInt();

    sd::frontend::Parameter p(name, (double)default_val, (double)min, (double)max);
    //parameterTmp p(name, type, min, max, default_val);
    //p.toString();
    return p;

}


sd::frontend::Parameter QtXMLParamParser::parseComplexParameter(QDomElement complex_param) {}
}
}
















