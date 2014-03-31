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

#ifndef SD_FRONTEND_LOADER_HPP
#define SD_FRONTEND_LOADER_HPP

#include <Types.hpp>
#include <SmithDRDefs.hpp>
#include "BlackBoxAlgorithm.hpp"
#include "AlgorithmInfo.hpp"
//#include "Parameter.hpp"

#include <frontend/libqt/SDRParameter.hpp>

#include <string>

namespace sd {
  namespace frontend {

    class Loader
      : public BlackBoxAlgorithm
    {

    public:

      enum Type { ImageLoader   = sd::ImageObjectType,
		  MeshLoader    = sd::MeshObjectType,
		  Unknown       = -1
      };

    public:

      virtual ~Loader();

      virtual bool load() = 0;
      virtual bool run() { return load(); }
      virtual Loader* clone() = 0;

      virtual const std::string& formatName() const = 0;
      // extensions (with beginnine star(*) character and comas), separated by semicolons
      // for files with extensions *.abc or *fixed.def, return "*.abc;*fixed.def"
      // for files with extension *.uvw only, return "*.uvw"
      virtual const std::string& fileExtensionFilters() const = 0;
      virtual Type loaderType() const = 0;

    protected:

      /*Loader(const std::string& loaderName,
	     const AlgorithmInfo& output = AlgorithmInfo(),
	     const ParameterList& parameters = ParameterList()
	     );*/

       /* Loader for new Parameters. */
       /* uses xml. */
      Loader(const std::string& loaderName,
       const AlgorithmInfo& output = AlgorithmInfo(),
       const std::string& = ""
       );

    private:

      Loader(const Loader&) = delete;
      Loader& operator=(const Loader&) = delete;

    };

  }
}

#endif /*! SD_FRONTEND_LOADER_HPP */
