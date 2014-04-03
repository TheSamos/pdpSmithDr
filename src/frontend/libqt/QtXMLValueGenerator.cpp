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

    QDomElement valueNode = m_xml.createElement("value");

    QString res;

    DataType d_type = param->getDataType();

    if (d_type == sd::libqt::String)
    {
        root.setAttribute("type", "string");

        SimpleStringParameter *p = static_cast<SimpleStringParameter *>(param);
        QString res(QString::fromStdString(p->getValue()));
        valueNode.appendChild(m_xml.createTextNode(QString::fromStdString(p->getValue())));
    }
    else if (d_type == sd::libqt::Float)
    {
        root.setAttribute("type", "float");

        SimpleFloatParameter *p = static_cast<SimpleFloatParameter *>(param);
        valueNode.appendChild(m_xml.createTextNode(QString::number(p->getValue())));
    }
    else if (d_type == sd::libqt::Int)
    {
        root.setAttribute("type", "int");

        SimpleIntParameter *p = static_cast<SimpleIntParameter *>(param);
        valueNode.appendChild(m_xml.createTextNode(QString::number(p->getValue())));
    }
    else if (d_type == sd::libqt::Bool)
    {
        root.setAttribute("type", "bool");

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