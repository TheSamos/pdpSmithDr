#ifndef SIMPLEPARAMETER_HPP
#define SIMPLEPARAMETER_HPP

#include <string>
#include "Parameter.hpp"

namespace sd {

  namespace libqt {

/*class SimpleParameter;
class SimpleDoubleParameter;*/

class SimpleParameter : public Parameter
{
public:
    SimpleParameter();
    SimpleParameter(std::string name);
    SimpleParameter(std::string name, std::string widget_name);

    enum DataType {Int, Double, Undefined};
    DataType datatype = Undefined;

    std::string dataTypeToString();

};

/* Double */

class SimpleDoubleParameter : public SimpleParameter
{
public:
    SimpleDoubleParameter(std::string m_name, double min, double max, double default_val);
    SimpleDoubleParameter(std::string m_name, double min, double max, double default_val, std::string widget);
    void print();

    double getMin() { return m_min; }
    double getMax() { return m_max; }
    double getDefault() { return m_default_val; }

private:
    double m_min;
    double m_max;
    double m_default_val;
    double m_value;
};

/* Int */

class SimpleIntParameter : public SimpleParameter
{
public:
    SimpleIntParameter(std::string m_name, int min, int max, int default_val);
    SimpleIntParameter(std::string m_name, int min, int max, int default_val, std::string widget);
    void print();

    int getMin() { return m_min; }
    int getMax() { return m_max; }
    int getDefault() { return m_default_val; }

private:
    int m_min;
    int m_max;
    int m_default_val;
    int m_value;
};

  }

}

#endif // SIMPLEPARAMETER_HPP
