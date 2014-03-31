#ifndef SIMPLEPARAMETER_HPP
#define SIMPLEPARAMETER_HPP

#include <string>
#include <map>
#include <iostream>
#include <vector>

#include "SDRParameter.hpp"




    namespace sd {
  
  namespace libqt {

    enum DataType {Int=0, Double, Float, Undefined};
std::map<DataType, std::string> typenames = {
        {Int, "int"},
        {Double, "double"},
        {Float, "float"},
        {Undefined, "undefined"}
    };

template <typename T, int DataType_>
class SimpleParameter : public SDRParameter
{

public:

    SimpleParameter(std::string name, T min, T max, T default_val)
    {
        m_name = name;
        m_min = min;
        m_max = max;
        m_default_val = default_val;
    }

    SimpleParameter(T value)
    {
        m_value = value;
    }

    T getMin() { return m_min; }
    T getMax() { return m_max; }
    T getDefault() { return m_default_val; }

    T getValue() { return m_value; }
    

    void setMin(T min) { m_min = min; }
    void setMax(T max) { m_max = max; }
    void setDefault(T default_val) { m_default_val = default_val; }

    void setValue(T value) { m_value = value; } 

  std::string dataTypeToString() { return typenames[DataType(DataType_)]; }

  DataType getDataType() { return DataType_; }
  

    void print()
    {
        std::cout << "[Parameter:(" << dataTypeToString() << ") " << m_name << ", min: " << m_min << ", max: " << m_max;
        std::cout << ", default: " << m_default_val << ", widget: " << m_widget_name << "]" << std::endl;
    }

private:
    T m_min;
    T m_max;
    T m_default_val;

    T m_value;

};



typedef SimpleParameter<int, Int> SimpleIntParameter;
typedef SimpleParameter<double, Double> SimpleDoubleParameter;
typedef SimpleParameter<float, Float> SimpleFloatParameter;

}
}

#endif // SIMPLEPARAMETER_HPP
