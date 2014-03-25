#include <iostream>

#include "SimpleParameter.hpp"

namespace sd {

  namespace libqt {

SimpleParameter::SimpleParameter()
{
}

SimpleParameter::SimpleParameter(std::string name) : Parameter(name)
{
}

SimpleParameter::SimpleParameter(std::string name, std::string widget_name) : Parameter(name, widget_name)
{
}

std::string SimpleParameter::dataTypeToString()
{
    if(datatype == Double)
        return "double";
    else if(datatype == Int)
        return "int";
    else
        return "Undefined";

}

/* Double */

SimpleDoubleParameter::SimpleDoubleParameter(std::string name, double min, double max, double default_val) : SimpleParameter(name)
{
    m_min = min;
    m_max = max;
    m_default_val = default_val;
    datatype = Double;
}

SimpleDoubleParameter::SimpleDoubleParameter(std::string name, double min, double max, double default_val, std::string widget) : SimpleParameter(name, widget)
{
    m_min = min;
    m_max = max;
    m_default_val = default_val;
    datatype = Double;
}

void SimpleDoubleParameter::print()
{
    std::cout << "[Parameter (" << dataTypeToString() << "): " << m_name << ", min: " << m_min << ", max: " << m_max << ", default: ";
    std::cout << m_default_val << ", widget: " << m_widget_name << std::endl;
}

/* Int */

SimpleIntParameter::SimpleIntParameter(std::string name, int min, int max, int default_val) : SimpleParameter(name)
{
    m_min = min;
    m_max = max;
    m_default_val = default_val;
    m_value = default_val;
    datatype = Int;
}

SimpleIntParameter::SimpleIntParameter(std::string name, int min, int max, int default_val, std::string widget) : SimpleParameter(name, widget)
{
    m_min = min;
    m_max = max;
    m_default_val = default_val;
    m_value = default_val;
    datatype = Int;
}

void SimpleIntParameter::print()
{
    std::cout << "[Parameter (" << dataTypeToString() << "): " << m_name << ", min: " << m_min << ", max: " << m_max << ", default: ";
    std::cout << m_default_val << ", widget: " << m_widget_name << std::endl;
}

  }
}
