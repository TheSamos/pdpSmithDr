#include "QtXMLValueGenerator.hpp"
#include "SimpleParameter.hpp"
//#include <frontend/libqt/SimpleParameter.hpp>

namespace sd {
  
  namespace libqt {

std::string
QtXMLValueGenerator::simpleXMLValue(SDRParameter* param)
{

    std::cout << "in simpleXMLValue" << std::endl;
    if(param == nullptr)
        std::cout << "param is null" << std::endl;
   
    if(param->getType() == sd::libqt::Simple){
        //SimpleParameter* p = static_cast<SimpleParameter*>(param);
        //SimpleParameter* p = (SimpleParameter*)param;

    if(param->getDataType() == String){

        std::cout << "In string" << std::endl;
        SimpleStringParameter* p = static_cast<SimpleStringParameter*>(param);
        QString res(QString::fromStdString(p->getValue()));

        m_xml.clear();

        QDomElement root = m_xml.createElement("parameter");
        root.setAttribute("name", QString::fromStdString(p->getName()));
        root.setAttribute("type", "string");

        QDomElement valueNode = m_xml.createElement("value");
        valueNode.appendChild(m_xml.createTextNode(res));

        root.appendChild(valueNode);

        m_xml.appendChild(root);

        delete p;

        return m_xml.toString().toStdString();

    }

      if(param->getDataType() == Int){
        SimpleIntParameter* p = static_cast<SimpleIntParameter*>(param);
        std::cout << "it's a int: " << std::endl;
        QString res;
        res.setNum(p->getValue());
    
        m_xml.clear();

        QDomElement root = m_xml.createElement("parameter");
        root.setAttribute("name", QString::fromStdString(p->getName()));
        root.setAttribute("type", "int");

        QDomElement valueNode = m_xml.createElement("value");
        valueNode.appendChild(m_xml.createTextNode(res));

        root.appendChild(valueNode);

        m_xml.appendChild(root);

        return m_xml.toString().toStdString();

    }



    if(param->getDataType() == Float){
        SimpleFloatParameter* p = static_cast<SimpleFloatParameter*>(param);
        std::cout << "it's a float: " << std::endl;
        QString res;
        res.setNum(p->getValue());
    
        m_xml.clear();

        QDomElement root = m_xml.createElement("parameter");
        root.setAttribute("name", QString::fromStdString(p->getName()));
        root.setAttribute("type", "float");

        QDomElement valueNode = m_xml.createElement("value");
        valueNode.appendChild(m_xml.createTextNode(res));

        root.appendChild(valueNode);

        m_xml.appendChild(root);

        return m_xml.toString().toStdString();
}

/*if(param.isBoolean()){
        std::cout << "it's a bool: " << std::endl;

        bool res = param.getBoolean();
    
        m_xml.clear();

        QDomElement root = m_xml.createElement("parameter");
        root.setAttribute("name", QString::fromStdString(param.name()));
        root.setAttribute("type", "boolean");

        QDomElement valueNode = m_xml.createElement("value");
        if(res)
            valueNode.appendChild(m_xml.createTextNode("true"));
        else
            valueNode.appendChild(m_xml.createTextNode("false"));

        root.appendChild(valueNode);

        m_xml.appendChild(root);

        return m_xml.toString().toStdString();

    }*/
    }
}


}
}