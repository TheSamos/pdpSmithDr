#ifndef PARAMETERTMP_H
#define PARAMETERTMP_H
#include <iostream>
#include <vector>

class parameterTmp {

public:
    parameterTmp(std::string name, std::string type, int min, int max, int paramDefault, int val);
    std::string getType();
    int getDefault();
    int getMax();
    int getMin();
    int getVal();
    std::string getName();
    void toString();

private:
    std::string m_type;
    std::string m_name;
    int m_val;
    int m_min;
    int m_max;
    int m_default;
};

typedef std::vector<parameterTmp> ParameterList;

#endif // PARAMETERTMP_H
