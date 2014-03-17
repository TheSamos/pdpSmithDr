#ifndef QTPARSER_H
#define QTPARSER_H
#include "parameterTmp.h"
#include <QFile>
#include <QDomNode>
#include <vector>

class qtParser
{
public:
    qtParser();
    ParameterList getParameters(std::string stringXML);

private:
    ParameterList recursion(QDomNode node, ParameterList params, std::string name);

};

#endif // QTPARSER_H
