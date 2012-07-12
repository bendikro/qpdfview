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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtCore>
#include <QtGui>

#include "miscellaneous.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog(QWidget* parent = 0);

public slots:
    void accept();

protected slots:
    void on_defaults_clicked();

private:
    QSettings* m_settings;

    QFormLayout* m_formLayout;
    QDialogButtonBox* m_dialogButtonBox;
    QPushButton* m_defaultsButton;

    QComboBox* m_tabPositionComboBox;
    QComboBox* m_tabVisibilityComboBox;

    QCheckBox* m_openUrlCheckBox;

    QCheckBox* m_autoRefreshCheckBox;

    QCheckBox* m_restoreTabsCheckBox;
    QCheckBox* m_restoreBookmarksCheckBox;

    QCheckBox* m_decoratePagesCheckBox;
    QCheckBox* m_decorateLinksCheckBox;

    QDoubleSpinBox* m_pageSpacingSpinBox;
    QDoubleSpinBox* m_thumbnailSpacingSpinBox;

    QDoubleSpinBox* m_thumbnailSizeSpinBox;

    QCheckBox* m_antialiasingCheckBox;
    QCheckBox* m_textAntialiasingCheckBox;
    QCheckBox* m_textHintingCheckBox;

    QComboBox* m_cacheSizeComboBox;

    QCheckBox* m_prefetchCheckBox;

    QLineEdit* m_fileToolBarLineEdit;
    QLineEdit* m_editToolBarLineEdit;
    QLineEdit* m_viewToolBarLineEdit;

};

#endif // SETTINGSDIALOG_H