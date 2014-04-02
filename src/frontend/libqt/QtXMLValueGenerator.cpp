#include "QtXMLValueGenerator.hpp"
#include "SimpleParameter.hpp"
//#include <frontend/libqt/SimpleParameter.hpp>

namespace sd
{

namespace libqt
{

std::string
QtXMLValueGenerator::simpleXMLValue(SDRParameter *param)
{

    if (param == nullptr)
        std::cout << "QtXMLValueGenerator: param is null" << std::endl;


    m_xml.clear();

    QDomElement root = m_xml.createElement("parameter");
    root.setAttribute("name", QString::fromStdString(param->getName()));
    root.setAttribute("type", "string");

    QDomElement valueNode = m_xml.createElement("value");

    QString res;

    DataType d_type = param->getDataType();

    if (d_type == sd::libqt::String)
    {
        SimpleStringParameter *p = static_cast<SimpleStringParameter *>(param);
        QString res(QString::fromStdString(p->getValue()));
        valueNode.appendChild(m_xml.createTextNode(QString::fromStdString(p->getValue())));
    }
    else if (d_type == sd::libqt::Float)
    {
        SimpleFloatParameter *p = static_cast<SimpleFloatParameter *>(param);
        valueNode.appendChild(m_xml.createTextNode(QString::number(p->getValue())));
    }
    else if (d_type == sd::libqt::Int)
    {
        SimpleIntParameter *p = static_cast<SimpleIntParameter *>(param);
        valueNode.appendChild(m_xml.createTextNode(QString::number(p->getValue())));
    }
    else if (d_type == sd::libqt::Bool)
    {
        SimpleBoolParameter *p = static_cast<SimpleBoolParameter *>(param);
        std::string bool_value = (p->getValue() == true) ? "true" : "false";
        valueNode.appendChild(m_xml.createTextNode(QString::fromStdString(bool_value)));
    }
    else
    {
        std::cout << "QtXMLValueGenerator: unknown DataType in simpleXMLValue" << std::endl;
    }

    root.appendChild(valueNode);
    m_xml.appendChild(root);

    return m_xml.toString().toStdString();
}


}
}