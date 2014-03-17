#include "qtparser.h"
#include <iostream>
#include <QByteArray>
#include <QtXml>
#include <QByteArray>
#include <QDomElement>
#include <string>


qtParser::qtParser()
{
}



ParameterList qtParser::getParameters(std::string stringXML){
    ParameterList parameters;

    QDomDocument *dom = new QDomDocument("entree_xml");
    QFile xml_doc((char*)stringXML.c_str());

    if(!xml_doc.open(QIODevice::ReadOnly))// Si l'on n'arrive pas à ouvrir le fichier XML.
    {
         std::cout << "error" << std::endl;
    }



    if (!dom->setContent(&xml_doc)) // Si l'on n'arrive pas à associer le fichier XML à l'objet DOM.
    {
         xml_doc.close();
         std::cout << "error2" << std::endl;

     }



    QDomElement dom_element = dom->documentElement();
    QDomNode noeud = dom_element.firstChild();


    while(!noeud.isNull())// boucle des parametres
    {
        if (noeud.toElement().tagName().toStdString() == "parameter" ){
            std::string name = noeud.toElement().attribute("name", "null").toStdString();

            parameters = recursion(noeud, parameters, name);

        }
        noeud = noeud.nextSibling();
    }


    return parameters;
}

ParameterList qtParser::recursion(QDomNode node, ParameterList params, std::string name){
    int defaultValue, min, max;
    std::string type;
    QDomNodeList listFils = node.childNodes();
    for(int i = 0; i < listFils.size(); ++i){//boucles des fils
        if (listFils.at(i).toElement().tagName().toStdString().compare("")){

            if(listFils.at(i).toElement().tagName().toStdString() == "type")
                type = listFils.at(i).toElement().text().toStdString();

            if(listFils.at(i).toElement().tagName().toStdString() == "default")
                defaultValue = std::atoi(listFils.at(i).toElement().text().toStdString().c_str());

            if(listFils.at(i).toElement().tagName().toStdString() == "min")
                min = std::atoi(listFils.at(i).toElement().text().toStdString().c_str());

            if(listFils.at(i).toElement().tagName().toStdString() == "max")
                max = std::atoi(listFils.at(i).toElement().text().toStdString().c_str());
        }


        recursion(listFils.at(i), params, name);
    }
    if(type == "bool")
        params.push_back(parameterTmp(name, type, NULL, NULL, defaultValue));
    else
        params.push_back(parameterTmp(name, type, min, max, defaultValue));
    return params;
}














