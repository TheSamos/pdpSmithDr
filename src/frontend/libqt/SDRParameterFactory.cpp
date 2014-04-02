#include "SDRParameterFactory.hpp"
#include "SimpleParameter.hpp"

namespace sd {
  
  namespace libqt {


  	SDRParameter* SDRParameterFactory::create(std::string type, std::string name, 
  		std::map<std::string, QString> map, std::string widget)
  	{

  		if(type == "string")
  		{
  			SimpleStringParameter *p = new SimpleStringParameter();

  			for (auto it=map.begin(); it!=map.end(); ++it)
  			{
        		p->setAttributFromName(it->first, it->second.toStdString());
			} 

			p->setName(name);
			p->setWidgetName(widget);

			return p;
  			
  		}
  		else if(type == "int")
  		{
  			SimpleIntParameter *p = new SimpleIntParameter();

  			for (auto it=map.begin(); it!=map.end(); ++it)
  			{
        		p->setAttributFromName(it->first, it->second.toInt());
        		std::cout << "setting: " << it->first << ", " <<  it->second.toInt() << std::endl;
			}

			p->setName(name);
			p->setWidgetName(widget);  

			return p;

  		}
  		else if(type == "double")
  		{
  			SimpleDoubleParameter *p = new SimpleDoubleParameter();

  			for (auto it=map.begin(); it!=map.end(); ++it)
  			{
        		p->setAttributFromName(it->first, it->second.toDouble());
			}  

			p->setName(name);
			p->setWidgetName(widget);

			return p;

  		}
  		else if(type == "bool")
  		{

  		}
  		else
  		{
  			std::cout << "Param Factory: unknown type: " << type << std::endl;
  		}

  		return nullptr;
  	}


	}

}