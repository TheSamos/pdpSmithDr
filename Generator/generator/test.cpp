#include "QtXMLValueGenerator.hpp"

int main(){

    QtXMLValueGenerator generator;

    parameterTmp p1 = parameterTmp("param1", "position", -100, 100, 12, 45);
    parameterTmp p2 = parameterTmp("param2", "vittesse", 0, 200, 13, 67);

    std::cout << "p1: " << generator.simpleXMLValue(p1) << std::endl;
    std::cout << "p2: " << generator.simpleXMLValue(p2) << std::endl;

    return 0;
}
