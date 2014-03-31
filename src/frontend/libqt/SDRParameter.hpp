#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include <string>

namespace sd {
  
  namespace libqt {

class SDRParameter
{
    enum Type {Simple, Complex, Undefined};
    Type m_type; // = Undefined;

public:

    virtual ~SDRParameter() {}

    SDRParameter() : m_type(Undefined) {};
    SDRParameter(std::string name) : m_name(name) {}

    Type getType()
    {
        return m_type;
    }

    virtual void print() = 0;

    typedef std::vector<SDRParameter> ParameterList;

protected:
    std::string m_name;
    std::string m_widget_name;
};

}
}

#endif // PARAMETER_HPP

