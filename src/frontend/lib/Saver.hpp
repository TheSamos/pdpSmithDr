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

#ifndef SD_FRONTEND_SAVER_HPP
#define SD_FRONTEND_SAVER_HPP

#include <SmithDRDefs.hpp>
#include "BlackBoxAlgorithm.hpp"
#include "AlgorithmInfo.hpp"
//#include "Parameter.hpp"

#include <string>

namespace sd {
  namespace frontend {

    class Saver
      : public BlackBoxAlgorithm
    {

    public:

      enum Type { ImageSaver   = sd::ImageObjectType,
		  MeshSaver    = sd::MeshObjectType,
		  Unknown      = -1
      };

    public:

      virtual ~Saver();

      virtual bool save() = 0;
      virtual bool run() { return save(); }
      virtual Saver* clone() { return NULL; }

      virtual const std::string& formatName() const = 0;
      // extensions (with beginnine star(*) character and comas), separated by semicolons
      // for files with extensions *.abc or *fixed.def, return "*.abc;*fixed.def"
      // for files with extension *.uvw only, return "*.uvw"
      virtual const std::string& fileExtensionFilters() const = 0;
      virtual Type saverType() const = 0;

    protected:

      /*Saver(const std::string& name,
	    const AlgorithmInfo& input = AlgorithmInfo(),
	    const ParameterList& parameters = ParameterList());*/

      Saver(const std::string& name,
      const AlgorithmInfo& input = AlgorithmInfo(),
      const std::string& parameters = "");

    private:

      Saver(const Saver&) = delete;
      Saver& operator=(const Saver&) = delete;

    };

  }
}

#endif /*! SD_FRONTEND_SAVER_HPP */
