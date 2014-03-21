#include "QtXMLValueGenerator.hpp"

std::string
QtXMLValueGenerator::simpleXMLValue(parameterTmp param){
    m_xml.clear();

    QDomElement root = m_xml.createElement("parameter");
    QDomElement valueNode = m_xml.createElement("value");
    QDomText value = m_xml.createTextNode("value");

    m_xml.appendChild(root);
    root.appendChild(valueNode);
    valueNode.appendChild(value);

    root.setAttribute("name", param.getName().c_str());
    root.setAttribute("type", param.getType().c_str());

    value.setNodeValue(this->intToQString(param.getVal()));
    value.setData(this->intToQString(param.getVal()));

    return m_xml.toString().toStdString();
}

QString
QtXMLValueGenerator::intToQString(int n){
    std::stringstream ssValue;
    std::string sValue;
    QString qsValue;

    ssValue << n;
    sValue = ssValue.str();
    qsValue = QString(sValue.c_str());
    return qsValue;
}
