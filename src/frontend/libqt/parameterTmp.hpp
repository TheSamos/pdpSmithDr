#ifndef PARAMETERTMP_HMM
#define PARAMETERTMP_HMM
#include <iostream>
#include <vector>

namespace sd {
  
  namespace libqt {
    
class parameterTmp {

public:
    parameterTmp(std::string name, std::string type, int min, int max, int paramDefault);
    std::string getType();
    int getDefault();
    int getMax();
    int getMin();
    std::string getName();
    void toString();

private:
    std::string m_name;
    std::string m_type;
    int m_min;
    int m_max;
    int m_default;
};

typedef std::vector<parameterTmp> ParameterList;

}
}

#endif // PARAMETERTMP_H
