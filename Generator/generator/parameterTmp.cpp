#include <string>
#include "parameterTmp.h"
#include <iostream>

parameterTmp::parameterTmp(std::string name, std::string type, int min, int max, int paramDefault, int val):
    m_name(name), m_type(type), m_min(min), m_max(max), m_default(paramDefault), m_val(val){}


int parameterTmp::getDefault(){
    return this->m_default;
}

std::string parameterTmp::getName(){
    return this->m_name;
}

int parameterTmp::getMax(){
    return this->m_max;
}

int parameterTmp::getMin(){
    return this->m_min;
}

int parameterTmp::getVal(){
    return this->m_val;
}

std::string parameterTmp::getType(){
    return this->m_type;
}

void parameterTmp::toString(){
    std::cout << "param : " << this->getName() << "  " << this->getType() << "  " << this->getDefault() << "  " << this->getMin() << "  " << this->getMax() << std::endl;
}
