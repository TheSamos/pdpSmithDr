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

#include <core/image/ImageView.hpp>
#include <core/image/ImageView_.hpp>
#include <frontend/lib/Loader.hpp>
#include <frontend/lib/LoaderManager.hpp>
#include <PluginDefs.hpp>

#include <Types.hpp>

#include <utils/sdRawUtils.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

static const std::string parameterFilename = "Filename";
static const std::string parameterNbImages = "Nb images (slices)";
static const std::string parameterNbColumns = "Nb columns";
static const std::string parameterNbRows = "Nb rows";
static const std::string parameterColumnResolution = "Column pixel resolution";
static const std::string parameterRowResolution = "Row pixel resolution";
static const std::string parameterSliceResolution = "Slice resolution";
static const std::string parameterHeaderOffset = "Header offset [bytes]";
static const std::string parameterOrientation = "Orientation";
static const std::string parameterSwapBytes = "Swap pixel bytes";
static const std::string parameterDataType = "Data type";

static const std::string orientationX = "X-axis (YZ-plane, sagittal)";
static const std::string orientationY = "Y-axis (XZ-plane, frontal)";
static const std::string orientationZ = "Z-axis (XY-plane, axial)";

//static const std::string dataType1bit = "1-bit";
static const std::string dataTypeAscii = "ascii";
static const std::string dataTypeInt8 = "int8";
static const std::string dataTypeUint8 = "uint8";
static const std::string dataTypeInt16 = "int16";
static const std::string dataTypeUint16 = "uint16";
static const std::string dataTypeInt32 = "int32";
static const std::string dataTypeUint32 = "uint32";
//static const std::string dataTypeInt64 = "int64";
//static const std::string dataTypeUint64 = "uint64";
static const std::string dataTypeFloat = "float";
static const std::string dataTypeDouble = "double";
//static const std::string dataTypeRGB = "RGB";

class LoaderRawImagePlugin : public sd::frontend::Loader
{

public:

  LoaderRawImagePlugin()
    : sd::frontend::Loader(m_name, m_output)
  {
  }

  virtual ~LoaderRawImagePlugin()
  {
#ifdef DEBUG
    std::cout << "~" << m_name << std::endl;
#endif
  }

  virtual const std::string&
  name() const
  {
    return m_name;
  }

  virtual LoaderRawImagePlugin*
  clone()
  {
    return new LoaderRawImagePlugin;
  }

  virtual const std::string&
  formatName() const
  {
    static const std::string fName = "RawImage";
    return fName;
  }

  virtual const std::string&
  fileExtensionFilters() const
  {
    static const std::string ext = "*.*";
    return ext;
  }

  virtual Loader::Type
  loaderType() const
  {
    return Loader::ImageLoader;
  }

  virtual bool
  load()
  {
    std::string filename = "";
    sd::frontend::Parameter p;
    this->getParams(parameterFilename, p);
    if (p.isString())
      filename = p.getAs<std::string>();

    if (filename.empty()) {
      std::cerr << "Empty filename. Abort!\n";
      return false;
    }

    this->getParams(parameterNbImages, p);
    int nbImages = p.getInteger();
    this->getParams(parameterNbColumns, p);
    int columns = p.getInteger();
    this->getParams(parameterNbRows, p);
    int rows = p.getInteger();

    this->getParams(parameterColumnResolution, p);
    double columnRes = p.getFloat();
    this->getParams(parameterRowResolution, p);
    double rowRes = p.getFloat();
    this->getParams(parameterSliceResolution, p);
    double sliceRes = p.getFloat();

    this->getParams(parameterHeaderOffset, p);
    int headerOffset = p.getInteger();

    this->getParams(parameterOrientation, p);
    std::string orientationStr = p.getString();
    sd::Orientation orientation;
    if (orientationStr == orientationX)
      orientation = sd::X_AXIS;
    else if (orientationStr == orientationY)
      orientation = sd::Y_AXIS;
    else if (orientationStr == orientationZ)
      orientation = sd::Z_AXIS;
    else
      return false;

    this->getParams(parameterSwapBytes, p);
    bool swapPixelBytes = p.getBoolean();

    this->getParams(parameterDataType, p);
    std::string dataTypeStr = p.getString();

    // load image file and skip header
    std::ifstream is(filename.c_str(), std::ios::binary);
    if (!is.is_open())
      return false;

    is.seekg(0L, std::ios::end);
    size_t file_len = is.tellg();
    is.seekg((long) headerOffset, std::ios::beg);

    if (headerOffset >= (int) file_len)
      return false;

    file_len -= headerOffset;
    std::vector<char> pdata(file_len+1);
    char *data = &pdata[0];
    is.read(data, file_len);
    is.close();
    size_t readable = file_len - headerOffset;

    // read pixels
    std::string errMsg;
    sd::core::ImageView* img = NULL;

    sd::Size sz(columns, rows, nbImages);
    sd::Resolution res(columnRes, rowRes, sliceRes);

    /*if (dataTypeStr == dataType1bit) {
    }
    else*/ if (dataTypeStr == dataTypeAscii) {
      // we have no idea of pixel data type; using float
      img = sd::raw::loadAscii<float>(data, readable, sd::GRAYLEVEL, 1, orientation, sz, res, errMsg);
    }
    else if (dataTypeStr == dataTypeInt8) {
      img = sd::raw::loadBinary<sd::INT8>(data, readable, sd::GRAYLEVEL, 1, orientation, sz, res, errMsg);
    }
    else if (dataTypeStr == dataTypeUint8) {
      img = sd::raw::loadBinary<sd::UINT8>(data, readable, sd::GRAYLEVEL, 1, orientation, sz, res, errMsg);
    }
    else if (dataTypeStr == dataTypeInt16) {
      if (swapPixelBytes)
	sd::raw::swap_2bytes(readable, data);
      img = sd::raw::loadBinary<sd::INT16>(data, readable, sd::GRAYLEVEL, 1, orientation, sz, res, errMsg);
    }
    else if (dataTypeStr == dataTypeUint16) {
      if (swapPixelBytes)
	sd::raw::swap_2bytes(readable, data);
      img = sd::raw::loadBinary<sd::UINT16>(data, readable, sd::GRAYLEVEL, 1, orientation, sz, res, errMsg);
    }
    else if (dataTypeStr == dataTypeInt32) {
      if (swapPixelBytes)
	sd::raw::swap_4bytes(readable, data);
      img = sd::raw::loadBinary<sd::INT32>(data, readable, sd::GRAYLEVEL, 1, orientation, sz, res, errMsg);
    }
    else if (dataTypeStr == dataTypeUint32) {
      if (swapPixelBytes)
	sd::raw::swap_4bytes(readable, data);
      img = sd::raw::loadBinary<sd::UINT32>(data, readable, sd::GRAYLEVEL, 1, orientation, sz, res, errMsg);
    }
    /*else if (dataTypeStr == dataTypeInt64) {
      if (swapPixelBytes)
	sd::raw::swap_8bytes(readable, data);
      img = sd::raw::loadBinary<sd::INT64>(data, readable, sd::GRAYLEVEL, 1, orientation, sz, res, errMsg);
    }
    else if (dataTypeStr == dataTypeUint64) {
      if (swapPixelBytes)
	sd::raw::swap_8bytes(readable, data);
      img = sd::raw::loadBinary<sd::UINT64>(data, readable, sd::GRAYLEVEL, 1, orientation, sz, res, errMsg);
      }*/
    else if (dataTypeStr == dataTypeFloat) {
      if (swapPixelBytes)
	sd::raw::swap_4bytes(readable, data);
      img = sd::raw::loadBinary<float>(data, readable, sd::GRAYLEVEL, 1, orientation, sz, res, errMsg);
    }
    else if (dataTypeStr == dataTypeDouble) {
      if (swapPixelBytes)
	sd::raw::swap_8bytes(readable, data);
      img = sd::raw::loadBinary<double>(data, readable, sd::GRAYLEVEL, 1, orientation, sz, res, errMsg);
    }
    /*else if (dataTypeStr == dataTypeRGB) {
      }*/
    else
      return false;

    if (!img)
      return false;

    img->addParent(this);

    return true;
  }

private:
  static const std::string m_name;
  static const sd::frontend::AlgorithmInfo m_output;
};

const std::string LoaderRawImagePlugin::m_name = "LoaderRawImagePlugin";
const sd::frontend::AlgorithmInfo LoaderRawImagePlugin::m_output = sd::frontend::make_info((sd::core::ImageView*) 0);

// Register our plugin
extern "C"
SMITHDR_PLUGIN_API
void
registerPlugin()
{
  const sd::frontend::Any undefined = sd::frontend::Parameter::undefined;

  std::vector<std::string> availableOrientations = {
    orientationX,
    orientationY,
    orientationZ
  };
  sd::frontend::Parameter orientation(parameterOrientation, availableOrientations[2], availableOrientations);

  std::vector<std::string> availableDataTypes = {
    //dataType1bit,
    dataTypeAscii,
    dataTypeInt8,
    dataTypeUint8,
    dataTypeInt16,
    dataTypeUint16,
    dataTypeInt32,
    dataTypeUint32,
    //dataTypeInt64,
    //dataTypeUint64,
    dataTypeFloat,
    dataTypeDouble//,
    //dataTypeRGB
  };
  sd::frontend::Parameter dataType(parameterDataType, availableDataTypes[2], availableDataTypes);

  sd::frontend::ParameterList parameters;
  sd::frontend::Parameter p(parameterFilename, "");
  p.setWidgetFactoryName("OpenFileButtonPlugin");
  parameters.push_back(p);
  parameters.push_back(sd::frontend::Parameter(parameterNbImages, 1, 1, undefined));
  parameters.push_back(sd::frontend::Parameter(parameterNbColumns, 1, 1, undefined));
  parameters.push_back(sd::frontend::Parameter(parameterNbRows, 1, 1, undefined));

  parameters.push_back(sd::frontend::Parameter(parameterColumnResolution, 1., 0., undefined));
  parameters.push_back(sd::frontend::Parameter(parameterRowResolution, 1., 0., undefined));
  parameters.push_back(sd::frontend::Parameter(parameterSliceResolution, 1., 0., undefined));

  parameters.push_back(sd::frontend::Parameter(parameterHeaderOffset, 0, 0, undefined));
  parameters.push_back(orientation);
  parameters.push_back(sd::frontend::Parameter(parameterSwapBytes, false));
  parameters.push_back(dataType);

  sd::frontend::registerLoader(new LoaderRawImagePlugin, parameters);
}
