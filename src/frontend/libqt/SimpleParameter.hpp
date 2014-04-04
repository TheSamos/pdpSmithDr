#ifndef SIMPLEPARAMETER_HPP
#define SIMPLEPARAMETER_HPP

#include <string>
#include <map>
#include <iostream>
#include <vector>

#include "SDRParameter.hpp"




    namespace sd {
  
  namespace libqt {



template <typename T, int DataType_>
class SimpleParameter : public SDRParameter
{

public:

    
    SimpleParameter(std::string name, T min, T max, T default_val, std::string widgetName)
    {
        m_name = name;
        m_widget_name = widgetName;
        m_min = min;
        m_max = max;
        m_default_val = default_val;
        m_MinDefined = true;
        m_MaxDefined = true;

        m_type = sd::libqt::Simple;
    }

    SimpleParameter(std::string name, T min, T max, T default_val)
    {
        m_MinDefined = true;
        m_MaxDefined = true;
        m_name = name;
        m_min = min;
        m_max = max;
        m_default_val = default_val;
    }

        SimpleParameter(std::string name, T default_val)
    {
        m_name = name;
        m_MinDefined = false;
        m_MaxDefined = false;
        m_default_val = default_val;
    }

    SimpleParameter(T value)
    {
        m_value = value;
        m_MinDefined = false;
        m_MaxDefined = false;
    }

    SimpleParameter() {}

    T getMin() { return m_min; }
    T getMax() { return m_max; }
    T getDefault() { return m_default_val; }

    T getValue() { return m_value; }
    
    bool isMinDefined(){
        return m_MinDefined;
    }

    bool isMaxDefined(){
        return m_MaxDefined;
    }
    void setMin(T min) { m_min = min; }
    void setMax(T max) { m_max = max; }
    void setDefault(T default_val) { m_default_val = default_val; }

    void setValue(T value) { m_value = value; } 

    void setAttributFromName(std::string name, T value)
    {
        if(name == "max"){
            m_max = value;
            m_MaxDefined = true;
        }
        else if(name == "min"){
            m_min = value;
            m_MinDefined = true;
        }
        else if(name == "default")
            m_default_val = value;
        else if(name == "name")
            m_name = value;
        else if(name == "value")
            m_value = value;
        else
            std::cout << "Unknown attribut name" << std::endl;
    }

    std::string dataTypeToString() { return typenames[sd::libqt::DataType(DataType_)]; }

    sd::libqt::DataType getDataType() { return (sd::libqt::DataType)DataType_; }
  

    void print()
    {
        std::cout << "[Parameter:(" << dataTypeToString() << ") " << m_name << ", min: " << m_min << ", max: " << m_max;
        std::cout << ", default: " << m_default_val << ", value: " << m_value << ", widget: " << m_widget_name << "]" << std::endl;
    }

private:
    T m_min;
    T m_max;
    T m_default_val;
    T m_value;
    bool m_MinDefined;
    bool m_MaxDefined;

};

typedef SimpleParameter<int, Int> SimpleIntParameter;
typedef SimpleParameter<double, Double> SimpleDoubleParameter;
typedef SimpleParameter<float, Float> SimpleFloatParameter;
typedef SimpleParameter<std::string, String> SimpleStringParameter;
typedef SimpleParameter<bool, Bool> SimpleBoolParameter;


}
}

#endif // SIMPLEPARAMETER_HPP
