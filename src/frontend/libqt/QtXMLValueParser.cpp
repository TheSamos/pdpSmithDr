#include "QtXMLValueParser.hpp"
#include "SimpleParameter.hpp"


namespace sd {
  
  namespace libqt {

QtXMLValueParser::QtXMLValueParser(std::string xml_string) : m_xmlstring(xml_string)
{
    initialize();
}



QtXMLValueParser::~QtXMLValueParser(){}



int QtXMLValueParser::initialize()
{

    bool hasParsed = m_qdoc.setContent(QString::fromStdString(m_xmlstring), namespace_processing,
                      m_error_msg, m_err_line, m_err_column);

    if(!hasParsed)
    {
        std::cout << "Parser failed line " << m_err_line << ", column " << m_err_column << " : ";
        std::cout << m_error_msg->toStdString() << std::endl;
    }

    return 1;

}

SDRParameter* QtXMLValueParser::parseSimpleParameter()
{

    std::cout << "parsing value" << std::endl;

    QDomElement param_root = m_qdoc.firstChildElement("parameter");
    std::string name = param_root.attributeNode("name").value().toStdString();
    std::string type = param_root.attributeNode("type").value().toStdString();

    QDomElement val_element = param_root.namedItem(QString::fromStdString("value")).toElement();

    SDRParameter *param;

    if(type == "string")
    {
        std::string value = val_element.text().toStdString();
        param = new SimpleStringParameter(value);
    }
    else if(type == "int")
    {
        int value = val_element.text().toInt();
        param = new SimpleIntParameter(value);
    }
    else if(type == "double")
    {
        double value = val_element.text().toDouble();
        param = new SimpleDoubleParameter(value);
    }
    else if(type == "float")
    {
        float value = val_element.text().toFloat();
        std::cout << "In value parser: " << value << std::endl;
        param = new SimpleFloatParameter(value);
    }
    else if(type == "bool")
    {
        bool value = (val_element.text().toStdString() == "true") ? true : false;
        param = new SimpleBoolParameter(value);
    }
    else
    {
        std::cout << "Error: QtXMLValueParser, value of unknow type." << std::endl;
        return nullptr;
    }

    param->setName(name);

    std::cout << "Created Parameter: " << std::endl;
    param->print();

    return param;

}

/*sd::libqt::Parameter QtXMLValueParser::parseComplexParameter() 
{
    return nullptr;
}*/

}}





