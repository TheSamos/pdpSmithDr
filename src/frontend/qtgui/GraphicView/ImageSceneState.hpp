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

#ifndef SD_QTGUI_GRAPHIC_VIEW_IMAGESCENESTATE_HPP
#define SD_QTGUI_GRAPHIC_VIEW_IMAGESCENESTATE_HPP

#include <GL/glew.h>

#include "ImageResource.hpp"
#include "ViewingWindow.hpp"
#include "SliceWindow.hpp"
#include "SceneState.hpp"

#include <set>
#include <functional>

class QPainter;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneWheelEvent;

namespace sd {
  namespace qtgui {
    namespace graphicView {

      class Scene;
      class ImageScene;

      class ImageSceneState
	: public SceneState
      {

      public:

	enum Type { State2dType, State3dType };

	virtual ~ImageSceneState();

	Type getType() const;

	virtual void drawOverlay(QPainter* painter);

	virtual void imageChanged();

      protected:

	friend class ImageScene;
	ImageSceneState(ImageScene& scene, ImageSceneState::Type t, bool picking);

	virtual bool mouseDoubleClick(QGraphicsSceneMouseEvent* event);
	virtual bool mouseMove(QGraphicsSceneMouseEvent* event);

	ImageResourcePtr getResource() const;
	ImageResourcePtr getResource(ImageScene& scene) const; //< used for connected scenes

	ViewingWindowPtr getViewWindow() const;
	SliceWindowPtr getSliceWindow() const;

	void adjustBrightnessContrast(std::function<void(void)> displayImage);

      private:

	ImageScene& m_imageScene;
	float m_mousePos[2];
	const Type m_type;

      };

    }
  }
}

#endif /*! SD_QTGUI_GRAPHIC_VIEW_IMAGESCENESTATE_HPP */
