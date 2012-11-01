/*

Copyright 2012 Adam Reichold

This file is part of qpdfview.

qpdfview is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

qpdfview is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with qpdfview.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef PRINTOPTIONSWIDGET_H
#define PRINTOPTIONSWIDGET_H

#include <QtCore>
#include <QtGui>

#include "documentview.h"

class PrintOptionsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PrintOptionsWidget(QWidget* parent = 0);

    DocumentView::PrintOptions printOptions() const;

private:
    QFormLayout* m_formLayout;

    QCheckBox* m_fitToPageCheckBox;
    QCheckBox* m_landscapeCheckBox;

    QComboBox* m_pageSetComboBox;

    QComboBox* m_numberUpComboBox;
    QComboBox* m_numberUpLayoutComboBox;

};

#endif // PRINTOPTIONSWIDGET_H