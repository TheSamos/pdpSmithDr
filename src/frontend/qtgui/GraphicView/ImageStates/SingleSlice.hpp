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

#ifndef SD_QTGUI_GRAPHIC_VIEW_IMAGESTATES_SINGLESLICE_HPP
#define SD_QTGUI_GRAPHIC_VIEW_IMAGESTATES_SINGLESLICE_HPP

#include <set>

#include <Types.hpp>

#include "../Widget.hpp"
#include "../ImageSceneState.hpp"
#include "../ImageResource.hpp"

namespace sd {
  namespace qtgui {
    namespace graphicView {
      namespace image_states {

	class SingleSlice
	  : public ImageSceneState
	{

	public:

	  SingleSlice(ImageScene& scene, bool picking);

	  virtual void glDraw();

	  virtual void imageChanged();

	  void setOrientation(Orientation dir);
	  Orientation getOrientation() const;

	  void setSliceNumber(UINT16 sNb);
	  UINT16 getSliceNumber() const;

	  const ImageSlicePtr getSlice() const;

	  void rotate90(int sign);

	protected:

	  virtual bool mouseDoubleClick(QGraphicsSceneMouseEvent* event);
	  virtual bool mousePress(QGraphicsSceneMouseEvent* event);
	  virtual bool mouseMove(QGraphicsSceneMouseEvent* event);
	  virtual bool wheelEvent(QGraphicsSceneWheelEvent* event);

	private:

	  float m_position[2];
	  float m_scaling;
	  int m_rotation; // multiple of 90 degrees

	  ImageSlicePtr m_slice;

	  Orientation m_ori;
	  UINT16 m_sliceNb;

	};

      }
    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_IMAGESTATES_SINGLESLICE_HPP */
