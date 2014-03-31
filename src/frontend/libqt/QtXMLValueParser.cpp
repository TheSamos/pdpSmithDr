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

    SDRParameter* pfake = new SimpleIntParameter(6);
    std::cout << "parsing value" << std::endl;
    QDomElement param_root = m_qdoc.firstChildElement("parameter");
    std::string name = param_root.attributeNode("name").value().toStdString();
    std::string type = param_root.attributeNode("type").value().toStdString();

    if(type == "int"){
        int value;
        QDomElement param_elem = param_root.firstChildElement();

        value = param_elem.text().toFloat();

        SDRParameter* p = new SimpleIntParameter(value);
        //p = value;
        return p;

    }

        if(type == "float"){
        float value;
        QDomElement param_elem = param_root.firstChildElement();

        value = param_elem.text().toFloat();

        SDRParameter* p = new SimpleFloatParameter(value);
        //p = value;
        return p;

    }


        /*if(type == "boolean"){
            std::string value;
            QDomElement param_elem = param_root.firstChildElement();

            value = param_elem.text().toStdString();
            if(value == "true"){
                std::cout << "it's true" << std::endl;
                frontend::Parameter p(name, true);
                return p;
        }
            else{
                frontend::Parameter p(name, false);
                return p;
            }



    }*/

    return pfake;

}

/*sd::libqt::Parameter QtXMLValueParser::parseComplexParameter() 
{
    return nullptr;
}*/

}}





