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

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QWidget>
RESTORE_CONTEXT()

#include <string>

#define UNDEFINED_VAL 99999

namespace sd {
  namespace qtgui {
    namespace nativewidgets {

      QWidget* buildBooleanEditor(bool b, QWidget* parameterizer);
      QWidget* buildIntegerEditor(int value, int mini, int maxi, QWidget* parameterizer);
      QWidget* buildFloatEditor(double value, double mini, double maxi, QWidget* parameterizer);
      QWidget* buildStringEditor(const std::string& value, QWidget* parameterizer);

      bool booleanReader(QWidget* w);
      int integralReader(QWidget* w);
      double floatingPointReader(QWidget* w);
      std::string stringReader(QWidget* w);

    }
  }
}
