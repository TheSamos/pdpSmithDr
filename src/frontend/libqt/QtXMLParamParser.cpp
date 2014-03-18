#include "QtXMLParamParser.hpp"
#include <iostream>


QtXMLParamParser::QtXMLParamParser(std::string xml_string) : m_xmlstring(xml_string)
{
    initialize();
}

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

void QtXMLParamParser::getParameterList()
{
    QDomElement param_root = m_qdoc.firstChildElement("parameters");
    QDomElement param = param_root.firstChildElement("parameter");

    for(; !param.isNull(); param = param.nextSiblingElement("parameter"))
    {
        QDomAttr param_type = param.attributeNode("type");
        //QDomNodeList children = param.childNodes();

        if(param_type.value() == "complex")
            parseComplexParameter(param);
        else
            parseSimpleParameter(param);

    }


}

void QtXMLParamParser::getParameter(){}

void QtXMLParamParser::parseSimpleParameter(QDomElement param)
{
    QDomAttr param_type = param.attributeNode("type");
    std::cout << "simple parameter: ";
    std::cout << " name: " << (param.attributeNode("name").value().toStdString());
    std::cout << ", ẗype: " << (param.attributeNode("type").value().toStdString()) << std::endl;
}


void QtXMLParamParser::parseComplexParameter(QDomElement complex_param) {}
















