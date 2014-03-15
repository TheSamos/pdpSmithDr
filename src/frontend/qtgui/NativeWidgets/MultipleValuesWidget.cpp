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

#include "MultipleValuesWidget.hpp"
#include "SingleValueAccessor.hpp"

#include <SmithDRDefs.hpp>
NO_QT_WARNINGS()
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QScrollArea>
RESTORE_CONTEXT()

#include <cassert>

// :GLITCH: Using default Qt objects, all integrals are int and all floating points are double --> may change Parameter type (such as long-->int or short-->int)

namespace sd {
  namespace qtgui {
    namespace nativewidgets {

      /*
       * VectorEditor
       */
      VectorEditor::VectorEditor(ItemCreator defaultItemCreator)
	: QListWidget(),
	  m_itemCreator(defaultItemCreator)
      {
      }

      void
      VectorEditor::addElement()
      {
	addNewItem(m_itemCreator);
	updated();
      }

      void
      VectorEditor::removeElement()
      {
	auto i = takeItem(count()-1);
	if (i)
	  delete i;
	updated();
      }

      void
      VectorEditor::addNewItem(ItemCreator itemCreator)
      {
	QListWidgetItem* i = new QListWidgetItem("", this);
	addItem(i);
	setItemWidget(i, itemCreator());
      }

      namespace {

	QWidget*
	buildVectorEditor(VectorEditor* ve, QWidget* parameterizer)
	{
	  QPushButton* removeValueButton = new QPushButton("Remove last value");
	  QObject::connect(removeValueButton, SIGNAL(clicked()), ve, SLOT(removeElement()));
	  removeValueButton->setDefault(false);
	  removeValueButton->setAutoDefault(false);
	  QPushButton* addValueButton = new QPushButton("Add value");
	  QObject::connect(addValueButton, SIGNAL(clicked()), ve, SLOT(addElement()));
	  addValueButton->setDefault(false);
	  addValueButton->setAutoDefault(false);
	  QDialogButtonBox* addRemoveBox = new QDialogButtonBox(Qt::Horizontal);
	  addRemoveBox->addButton(removeValueButton, QDialogButtonBox::ActionRole);
	  addRemoveBox->addButton(addValueButton, QDialogButtonBox::ActionRole);

	  QObject::connect(ve, SIGNAL(updated()), parameterizer, SIGNAL(parametersChanged()));

	  QWidget* w = new QWidget;
	  QVBoxLayout* wLayout = new QVBoxLayout;
	  wLayout->addWidget(ve);
	  wLayout->addWidget(addRemoveBox);
	  w->setLayout(wLayout);

	  QScrollArea* sc = new QScrollArea;
	  sc->setWidget(w);
	  return sc;
	}

#define BUILD_TYPE_VECTOR_EDITOR_MIN_MAX(TypeName, Type)		\
	QWidget*							\
	build##TypeName##VectorEditor(const std::vector<Type>& values,	\
				      const Type& mini, const Type& maxi, \
				      QWidget* parameterizer)		\
	{								\
	  VectorEditor* ve = new VectorEditor(std::bind(build##TypeName##Editor, Type(), mini, maxi, parameterizer)); \
	  for (size_t i = 0; i < values.size(); ++i) {			\
	    ve->addNewItem(std::bind(build##TypeName##Editor, values[i], mini, maxi, parameterizer)); \
	  }								\
	  return buildVectorEditor(ve, parameterizer);			\
	}

#define BUILD_TYPE_VECTOR_EDITOR(TypeName, Type)			\
	QWidget*							\
	build##TypeName##VectorEditor(const std::vector<Type>& values,	\
				      QWidget* parameterizer)		\
	{								\
	  VectorEditor* ve = new VectorEditor(std::bind(build##TypeName##Editor, Type(), parameterizer)); \
	  for (size_t i = 0; i < values.size(); ++i) {			\
	    ve->addNewItem(std::bind(build##TypeName##Editor, values[i], parameterizer)); \
	  }								\
	  return buildVectorEditor(ve, parameterizer);			\
	}


	BUILD_TYPE_VECTOR_EDITOR_MIN_MAX(Integer, int)
	BUILD_TYPE_VECTOR_EDITOR_MIN_MAX(Float, double)
	BUILD_TYPE_VECTOR_EDITOR(Boolean, bool)
	BUILD_TYPE_VECTOR_EDITOR(String, std::string)

#undef BUILD_TYPE_VECTOR_EDITOR_MIN_MAX
#undef BUILD_TYPE_VECTOR_EDITOR

	QWidget*
	buildEditor(const frontend::Parameter& p, QWidget* parameterizer)
	{
	  if (p.isIntegerVector()) {
	    int mini = p.isMinDefined() ? p.getMin<int>() : -UNDEFINED_VAL;
	    int maxi = p.isMaxDefined() ? p.getMax<int>() : +UNDEFINED_VAL;
	    return buildIntegerVectorEditor(p.getAsVectorOf<int>(), mini, maxi, parameterizer);
	  }
	  else if (p.isFloatVector()) {
	    double mini = p.isMinDefined() ? p.getMin<double>() : -UNDEFINED_VAL;
	    double maxi = p.isMaxDefined() ? p.getMax<double>() : +UNDEFINED_VAL;
	    return buildFloatVectorEditor(p.getAsVectorOf<double>(), mini, maxi, parameterizer);
	  }
	  else if (p.isBooleanVector()) {
	    return buildBooleanVectorEditor(p.getAsVectorOf<bool>(), parameterizer);
	  }
	  else if (p.isStringVector()) {
	    return buildStringVectorEditor(p.getAsVectorOf<std::string>(), parameterizer);
	  }

	  assert(!"Should never get here!");
	  return NULL;
	}

      }

      MultipleValuesWidget::MultipleValuesWidget(frontend::Parameter& p)
	: QtParametrizationWidget(p)
      {
      }

      MultipleValuesWidget::~MultipleValuesWidget()
      {
      }

      void
      MultipleValuesWidget::build(QWidget* parameterizer)
      {
	m_widget = buildEditor(m_p, parameterizer);

	m_layout = new QHBoxLayout;
	m_layout->addWidget(new QLabel(m_p.name().c_str()));
	m_layout->addWidget(m_widget);
      }

      namespace {

	template<typename Type, typename ValueReader>
	void
	getVectorParameter(QWidget* w, ValueReader valueReader, frontend::Parameter& p)
	{
	  QScrollArea* sc = dynamic_cast<QScrollArea*>(w);
	  QLayout* childLayout = sc->widget()->layout();
	  for (int j = 0; j < childLayout->count(); ++j) {
	    QLayoutItem* item = childLayout->itemAt(j);
	    VectorEditor* editor = dynamic_cast<VectorEditor*>(item->widget());
	    if (editor != NULL) {
	      std::vector<Type> values;
	      for (int k = 0; k < editor->count(); ++k) {
		values.push_back(valueReader(editor->itemWidget(editor->item(k))));
	      }
	      p = values;
	      return;
	    }
	  }
	  assert(!"Should never get here!");
	}

      }

      void
      MultipleValuesWidget::updateParameter()
      {
	if (m_p.isIntegerVector()) {
	  getVectorParameter<int>(m_widget, integralReader, m_p);
	}
	else if (m_p.isFloatVector()) {
	  getVectorParameter<double>(m_widget, floatingPointReader, m_p);
	}
	else if (m_p.isBooleanVector()) {
	  getVectorParameter<bool>(m_widget, booleanReader, m_p);
	}
	else if (m_p.isStringVector()) {
	  getVectorParameter<std::string>(m_widget, stringReader, m_p);
	}
	else
	  assert(!"Should never get here!");
      }

    }
  }
}
