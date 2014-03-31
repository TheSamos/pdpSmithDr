#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include <string>
#include <vector>

namespace sd {
  
  namespace libqt {

    enum Type {Simple, Complex, Undefined_};

    enum DataType {Int=0, Double, Float, Undefined};


class SDRParameter
{
    //enum Type {Simple, Complex, Undefined};
    Type m_type; // = Undefined;

public:

    std::map<DataType, std::string> typenames = {
        {Int, "int"},
        {Double, "double"},
        {Float, "float"},
        {Undefined, "undefined"}
    };

    virtual ~SDRParameter() {}

    SDRParameter() : m_type(Undefined_) {};
    SDRParameter(std::string name) : m_name(name) {}

    Type getType() { return m_type; }

    std::string getName() { return m_name; }
    std::string getWidgetName() { return m_widget_name; }

    void setName(std::string name) { this->m_name = name; }
    void setWidgetName(std::string widgetName) { this->m_widget_name = widgetName; }

    virtual void print() = 0;

    virtual DataType getDataType() = 0;

    typedef std::vector<SDRParameter> ParameterList;

protected:
    std::string m_name;
    std::string m_widget_name;
};

}
}

#endif // PARAMETER_HPP

