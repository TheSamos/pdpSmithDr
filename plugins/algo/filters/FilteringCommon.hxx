
/*
  Copyright (C) 2011 The SmithDR team, smithdr@labri.fr

  This file is part of SmithDR.

  SmithDR is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  SmithDR is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with SmithDR.
  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SD_FILTERING_COMMON_HXX
#define SD_FILTERING_COMMON_HXX

#include <core/ViewNode.hpp>
#include <core/image/ImageView.hpp>
#include <core/image/Image.hpp>
#include <frontend/lib/ComputedImageViewAlgorithm.hpp>
#include <frontend/lib/AlgorithmInfo.hpp>
#include <frontend/lib/Parameter.hpp>
#include <PluginDefs.hpp>
#include <Types.hpp>

#include <imageprocessing/filters/Filter.hpp>
#include <imageprocessing/filters/StructuralElement.hpp>
#include <imageprocessing/filters/GaussianLinearFilter.hpp>
#include <imageprocessing/filters/DogFilter.hpp>
#include <imageprocessing/filters/LogFilter.hpp>
#include <imageprocessing/filters/UniformFilter.hpp>
#include <imageprocessing/filters/MedianFilter.hpp>
#include <imageprocessing/filters/PrewittFilter.hpp>
#include <imageprocessing/filters/RobertsFilter.hpp>
#include <imageprocessing/filters/SobelFilter.hpp>
#include <imageprocessing/filters/KirschFilter.hpp>
#include <imageprocessing/filters/DilationFilter.hpp>
#include <imageprocessing/filters/ErosionFilter.hpp>
#include <imageprocessing/filters/GaussianFrequencyFilter.hpp>

template<typename T>
sd::core::ImageView_<T>* getImage(sd::frontend::ComputedImageViewAlgorithm<T>* self) {
  const std::vector<sd::core::ViewNode*>& parents = self->getPreviousViewNodes();
  if (parents.size() != 1) {
#ifdef DEBUG
    std::cout << "\tExpecting 1 parent exactly, found " << parents.size() << ". Abort!\n";
#endif
    return NULL;
  }

  sd::core::ImageView_<T>* image = static_cast<sd::core::ImageView_<T>*>(parents[0]);
#ifdef DEBUG
  std::cout << "\tWorking with image: " << image->name() << std::endl;
#endif

  if (image->channelType() != sd::core::DataType_<T>()) {
#ifdef DEBUG
    std::cout << "Bad image type! Abort!\n";
#endif
    return NULL;
  }

  return image;
}

#define DECLARE_FILTERING_PLUGIN_CLASS(PluginName, Path, FilterName)	\
  template<typename T>							\
  class PluginName							\
    : public sd::frontend::ComputedImageViewAlgorithm<T> {		\
  									\
    									\
  public:								\
  									\
    PluginName()							\
      : sd::frontend::ComputedImageViewAlgorithm<T>(m_name,		\
						    m_input) {}		\
    									\
    virtual ~PluginName()						\
    {}									\
									\
    virtual const std::string& name() const {				\
      return m_name;							\
    }									\
    									\
    virtual PluginName*	clone() {					\
      return new PluginName;						\
    }									\
    									\
    virtual bool run();							\
    									\
  private:								\
    static const std::string m_name;					\
    static const sd::frontend::AlgorithmInfo m_input;			\
  };									\
  									\
  template<typename T> const std::string PluginName<T>::m_name = #Path#FilterName; \
  template<typename T> const sd::frontend::AlgorithmInfo PluginName<T>::m_input \
  = sd::frontend::make_info(sd::core::ObjectDescription::ImageView(sd::core::DataType_<T>(), \
								   sd::core::ObjectDescription::ANY_CARD, \
								 1));
  
#define DECLARE_FILTERING_PLUGIN(FilterName, Path)			\
  DECLARE_FILTERING_PLUGIN_CLASS(FilterName##ImageFilteringPlugin, Path, FilterName)

#define DECLARE_RUN_HEAD(FilterName, Path)		\
  DECLARE_FILTERING_PLUGIN(FilterName, Path)		\
  template<typename T>					\
  bool							\
  FilterName##ImageFilteringPlugin<T>::run()		\
  {							\
  sd::core::ImageView_<T>* image = getImage(this);	\
  if (!image)						\
    return false;

#define DECLARE_RUN_FOOT()				\
  if (filter) {						\
    /*auto result = */filter->applyTo(*this, *image);	\
    /*setResult(result);*/				\
    delete filter;					\
  }							\
  							\
  return true;						\
  }

#define DECLARE_FILTERING_PLUGIN_NO_ARG(FilterName, Path)	\
  DECLARE_RUN_HEAD(FilterName, Path)				\
  Filter<T>* filter = new FilterName##Filter<T>;		\
  DECLARE_RUN_FOOT()

#define DECLARE_FILTERING_PLUGIN_ONE_ARG_KERNEL_HEAD(FilterName, Path)	\
  DECLARE_RUN_HEAD(FilterName, Path)					\
  sd::frontend::Parameter p;						\
  this->getParams("kernel_size", p);					\
  int ksize = p.getInteger();

#define DECLARE_FILTERING_PLUGIN_ONE_ARG_KERNEL_V1(FilterName, Path)	\
  DECLARE_FILTERING_PLUGIN_ONE_ARG_KERNEL_HEAD(FilterName, Path)	\
  Filter<T>* filter = new FilterName##Filter<T>(ksize, ksize, 1);	\
  DECLARE_RUN_FOOT()

#define DECLARE_FILTERING_PLUGIN_ONE_ARG_KERNEL_V2(FilterName, Path)	\
  DECLARE_FILTERING_PLUGIN_ONE_ARG_KERNEL_HEAD(FilterName, Path)	\
  Filter<T>* filter = new FilterName##Filter<T>(ksize);			\
  DECLARE_RUN_FOOT()

#define DECLARE_FILTERING_PLUGIN_ONE_ARG_SIGMA(FilterName, Path)	\
  DECLARE_RUN_HEAD(FilterName, Path)					\
  sd::frontend::Parameter p;						\
  this->getParams("sigma", p);						\
  double sigma = p.getFloat();					\
    									\
  size_t width = image->size().width();					\
  size_t height = image->size().height();					\
  Filter<T>* filter = new FilterName##Filter<T>(width, height, 1, sigma); \
  DECLARE_RUN_FOOT()

#define DECLARE_FILTERING_PLUGIN_TWO_ARGS_KERNEL_SIGMA(FilterName, Path) \
  DECLARE_RUN_HEAD(FilterName, Path)					\
  sd::frontend::Parameter p;						\
  this->getParams("kernel_size", p);					\
  int ksize = p.getInteger();						\
  this->getParams("sigma", p);						\
  double sigma = p.getFloat();					\
									\
  Filter<T>* filter = new FilterName##Filter<T>(ksize, ksize, 1, sigma); \
  DECLARE_RUN_FOOT()

#define DECLARE_FILTERING_PLUGIN_THREE_ARGS_KERNEL_SIGMA1_SIGMA2(FilterName, Path) \
  DECLARE_RUN_HEAD(FilterName, Path)					\
  sd::frontend::Parameter p;						\
  this->getParams("kernel_size", p);					\
  int ksize = p.getInteger();						\
  this->getParams("sigma1", p);						\
  double sigma1 = p.getFloat();					\
  this->getParams("sigma2", p);						\
  double sigma2 = p.getFloat();					\
									\
  Filter<T>* filter = new FilterName##Filter<T>(ksize, ksize, 1, sigma1, sigma2); \
  DECLARE_RUN_FOOT()

#define DECLARE_FILTERING_PLUGIN_MORPHO(Path)			\
  DECLARE_RUN_HEAD(Morphological, Path)				\
  sd::frontend::Parameter p;					\
  this->getParams("method", p);					\
  std::string method = p.getString();				\
  if (method != "Dilation" && method != "Erosion"		\
      && method != "Opening" && method != "Closing")		\
    return false;						\
  this->getParams("structural element", p);			\
  std::string structElt = p.getString();			\
  if (structElt != "square" && structElt != "circle"		\
      && structElt != "cross")					\
    return false;						\
  this->getParams("side", p);					\
  int side = p.getInteger();					\
  this->getParams("radius", p);					\
  int radius = p.getInteger();					\
  this->getParams("is diagonal", p);				\
  bool isDiagonal = p.getBoolean();				\
  Filter<T>* filter = NULL;					\
  StructuralElement* sel = NULL;				\
  if (structElt == "square")					\
    sel = StructuralElement::buildSquare(side);			\
  else if (structElt == "circle")				\
    sel = StructuralElement::buildCircle(radius);		\
  else if (structElt == "cross")				\
    sel = StructuralElement::buildCross(side, isDiagonal);	\
  if (method == "Dilation" || method == "Closing")		\
    filter = new DilationFilter<T>(*sel);			\
  else if (method == "Erosion" || method == "Opening")		\
    filter = new ErosionFilter<T>(*sel);			\
  if (method == "Opening") {					\
    sd::core::Image<T> conv;					\
    filter->applyTo(conv, *image);				\
    delete filter;						\
    filter = new DilationFilter<T>(*sel);			\
    filter->applyTo(*this, conv);				\
    delete filter;						\
  }								\
  else if (method == "Closing") {				\
    sd::core::Image<T> conv;					\
    filter->applyTo(conv, *image);				\
    delete filter;						\
    filter = new ErosionFilter<T>(*sel);			\
    filter->applyTo(*this, conv);				\
    delete filter;						\
  }								\
  else if (filter) {						\
    filter->applyTo(*this, *image);				\
    delete filter;						\
  }								\
  delete filter;						\
  return true;							\
  }

#endif /*! SD_FILTERING_COMMON_HXX */
