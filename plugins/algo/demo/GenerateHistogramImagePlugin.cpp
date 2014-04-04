#include <core/image/ImageView.hpp>
#include <core/image/Image.hpp>
#include <core/ViewNode.hpp>
#include <frontend/lib/ComputedImageViewAlgorithm.hpp>
#include <frontend/lib/AlgorithmManager.hpp>
#include <frontend/lib/AlgorithmInfo.hpp>
#include <frontend/libqt/SDRParameter.hpp>
#include <frontend/libqt/SimpleParameter.hpp>
#include <Types.hpp>
#include <PluginDefs.hpp>
#include <vector>

#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>

template<typename T>
class GenerateHistogramImagePlugin : public sd::frontend::ComputedImageViewAlgorithm<T>
{
public:

    GenerateHistogramImagePlugin() : sd::frontend::ComputedImageViewAlgorithm<T>(m_name, m_input) {}

    virtual ~GenerateHistogramImagePlugin()
    {
        std::cout << "~" << m_name << std::endl;
    }

    virtual const std::string &name() const
    {
        return m_name;
    }

    virtual GenerateHistogramImagePlugin *clone()
    {
        return new GenerateHistogramImagePlugin;
    }

    int quot(int a, int b)
    {
    	div_t divr = div(a, b);
    	return static_cast<int>(divr.quot);
    }

    virtual bool run()
    {
        const std::vector<sd::core::ViewNode *> &parents = this->getPreviousViewNodes();
	sd::core::ImageView_<T> *image = static_cast<sd::core::ImageView_<T>*>(parents[0]);

        if (parents.size() != 1)
        {
            std::cout << "\tExpecting 1 parent exactly, found " << parents.size() << ". Abort!\n";
            return false;
        }
        
        /*sd::frontend::Parameter pchannel;
        this->getParams("channel", pchannel);
        std::string wanted_channel = pchannel.getString();
        std::cout << "SELECTED channel: " << wanted_channel << std::endl;*/
	
	/*sd::frontend::Parameter pNbTiles;
	this->getParams("nbTiles", pNbTiles);
	int nbTiles = pNbTiles.getInteger();*/
    
    sd::libqt::SimpleIntParameter *p = static_cast<sd::libqt::SimpleIntParameter *>(this->getXMLParams("nbTiles"));
    int nbTiles = p->getValue();

        /* RGBA: R is 0, G is 1...*/
        int channel_id = 0;
        /*if(wanted_channel == "Red")
            channel_id = 0;
        else if(wanted_channel == "Green")
            channel_id = 1;
        else if(wanted_channel == "Blue")
            channel_id = 2;
        else if(wanted_channel == "Alpha")
            channel_id = 3;
        else
            std::cout << "Error on channel, default: red" << std::endl;	*/
	
	std::vector<float> histogram_data_1(256, 0);
	std::vector<float> histogram_data(nbTiles, 0);
	int loop_count = 0;  
        int value = 0;  
	for(auto source_it = image->begin(); source_it != image->end(); ++source_it){
            value = (float)source_it(channel_id);
            histogram_data_1[value]++;
            loop_count++;
	}
	
	int coef = 256/nbTiles;
    for(int i=0; i<nbTiles; i++){
	   for(int j=0; j<coef; j++){
	       histogram_data[i] += histogram_data_1[(i+1)*coef+j];
	  }
    }
	
	float maxHeight = 0;
	for(int i=0; i<=nbTiles; i++){
	  if(maxHeight < histogram_data[i]){
	    maxHeight = histogram_data[i];
	  }
	}
	
	if(maxHeight != 0){
	  for(int i=0; i<=nbTiles; i++){
        histogram_data[i] = (histogram_data[i]*256)/maxHeight;
	  }
	}
	
	int sizeX = nbTiles;
	int sizeY = nbTiles;
	sd::Size result_size(sizeX, sizeY);
	sd::core::ImageViewInfo info(sd::GRAYLEVEL, sd::Z_AXIS, result_size);
	this->init(info);
	T* resultData = this->getData();
	std::fill(resultData, resultData+result_size.dataSize(), 255);
	this->setMinMax(0, 255);

    auto itR = this->begin();
    for(int i = 0; i < nbTiles; i++){
        for(int j = 0; j < nbTiles; j++, itR++){
            for(int k = 0; k < 4; k++){
                if((i) < (int)(nbTiles-histogram_data[j]))
                    itR(k) = 255;
                else
                    itR(k) = 0;
            }
        }
    }

    return true;
    }

private:
    static const std::string m_name;
    static const sd::frontend::AlgorithmInfo m_input;
    static const sd::frontend::AlgorithmInfo m_output;

};

template<typename T> const std::string GenerateHistogramImagePlugin<T>::m_name = "Demonstration:GenerateHistogramImagePlugin";
template<typename T> const sd::frontend::AlgorithmInfo GenerateHistogramImagePlugin<T>::m_input = sd::frontend::make_info(sd::core::ObjectDescription::ImageView(sd::core::DataType_<T>(),
        sd::core::ObjectDescription::ANY_CARD,
        sd::core::ObjectDescription::ANY_CARD));
template<typename T> const sd::frontend::AlgorithmInfo GenerateHistogramImagePlugin<T>::m_output = sd::frontend::make_info((sd::core::ImageView *) 0);


// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin()
{

      std::string parameters = "<parameters><parameter name=\"nbTiles\" type=\"int\"> \
                                  <min>1</min> \
                                  <default>256</default> \
                                  <max>256</max> \
                            </parameter></parameters>";

    sd::frontend::registerAlgorithm(new GenerateHistogramImagePlugin<sd::UINT8>, parameters);
}