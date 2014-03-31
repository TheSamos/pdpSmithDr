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

#ifndef SD_MESH_PROCESSING_COLOR_COMPUTE_COLOR_SCALE_LAYER_HXX
#define SD_MESH_PROCESSING_COLOR_COMPUTE_COLOR_SCALE_LAYER_HXX

#include <QRectF>
#include <QPainter>

sd::meshprocessing::colorcompute::ColorScaleLayer::ColorScaleLayer(ColorComputer* cc)
  : m_cc(cc) {}


sd::meshprocessing::colorcompute::ColorScaleLayer::ColorScaleLayer(const ColorScaleLayer& csl) 
  : m_cc(csl.m_cc->clone()) {
}

sd::meshprocessing::colorcompute::ColorScaleLayer::~ColorScaleLayer() {
  delete m_cc;
}

void
sd::meshprocessing::colorcompute::ColorScaleLayer::draw(const QRectF& rect, QPainter* painter) const {
  const float marge = 10.;
  const float width = 15.;
  const float length = rect.height() - 2.*marge - .15*rect.height();
  const QPointF offset(rect.width()-marge-width, marge);
  
  double delta = m_cc->getMax() - m_cc->getMin();
  
  QString text = QString::number(m_cc->getMin())
    + QString::fromUtf8(" - ")
    + QString::number(m_cc->getMax());
  
  sd::core::RGBCoordinates<unsigned char> rgb;
  for (double i=-4; i<length+4; ++i) {
    m_cc->computeColor(m_cc->getMin() + i*delta/length, rgb);
    QColor c(rgb.r(), rgb.g(), rgb.b());
    painter->setPen(c);
    painter->drawLine(offset.x(), offset.y()+length-i, offset.x()+width, offset.y()+length-i);
  }
  
  painter->setPen(Qt::black);
  
  int textHeight = painter->fontMetrics().height();
  int textWidth = painter->fontMetrics().width(QString::number(m_cc->getMin()));
  painter->drawText(QPointF(offset.x() - textWidth - marge,
			    offset.y() + textHeight/2 + length),
		    QString::number(m_cc->getMin()));
  painter->drawLine(offset.x()-(marge/2.), offset.y() + length,
		    offset.x()+width+(marge/2.), offset.y() + length);
  if (m_cc->getMin() < 0 && m_cc->getMax() > 0) {
    float zeroY = length * fabs(m_cc->getMin()) / (m_cc->getMax() - m_cc->getMin());
    textWidth = painter->fontMetrics().width(QString::number(0));
    painter->drawText(QPointF(offset.x() - textWidth - marge,
			      offset.y() + textHeight/2 + length - zeroY),
		      QString::number(0));
    painter->drawLine(offset.x()-(marge/2.), offset.y() + length - zeroY,
		      offset.x()+width+(marge/2.), offset.y() + length - zeroY);
  }
  textWidth = painter->fontMetrics().width(QString::number(m_cc->getMax()));
  painter->drawText(QPointF(offset.x() - textWidth - marge,
			    offset.y() + textHeight/2),
		    QString::number(m_cc->getMax()));
  painter->drawLine(offset.x()-(marge/2.), offset.y(),
		    offset.x()+width+(marge/2.), offset.y());
}

#endif /* ! SD_MESH_PROCESSING_COLOR_COMPUTE_COLOR_SCALE_LAYER_HXX */
