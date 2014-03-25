#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include <string>
#include <vector>

namespace sd {

  namespace libqt {

class Parameter
{

    enum Type {Simple, Complex, Undefined};
    Type type = Undefined;

public:
    Parameter();
    Parameter(std::string name) : m_name(name) {}
    Parameter(std::string name, std::string widget_name) : m_name(name), m_widget_name(widget_name) {}

    void setName(std::string name) { m_name = name; }
    void setWidgetName(std::string widget_name) { m_widget_name = widget_name; }

    Type getType() { return type; }

    virtual void print() = 0;

protected:
    std::string m_name;
    std::string m_widget_name;
};

    typedef std::vector<Parameter*> ParameterList;

  }
}

#endif // PARAMETER_HPP
