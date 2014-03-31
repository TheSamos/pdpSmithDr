#include "QtXMLValueGenerator.hpp"

#include <frontend/libqt/SimpleParameter.hpp>

namespace sd {
  
  namespace libqt {

std::string
QtXMLValueGenerator::simpleXMLValue(frontend::Parameter& param)
{

   

    if(param.isFloat())
        std::cout << "it's a float: " << std::endl;

    if(param.isInteger())
        std::cout << "it's a int: " << std::endl;


    QString res;
    res.setNum(param.getInteger());
    
    m_xml.clear();

    QDomElement root = m_xml.createElement("parameter");
    root.setAttribute("name", QString::fromStdString(param.name()));
    root.setAttribute("type", "float");

    QDomElement valueNode = m_xml.createElement("value");
    valueNode.appendChild(m_xml.createTextNode(res));

    root.appendChild(valueNode);

    m_xml.appendChild(root);

    return m_xml.toString().toStdString();
}


}
}