/*

Copyright 2014 Adam Reichold

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

#ifndef BOOKMARKMODEL_H
#define BOOKMARKMODEL_H

#include <QAbstractListModel>

#include <QDateTime>

struct BookmarkItem
{
    int page;

    QString label;
    QString comment;
    QDateTime modified;

    BookmarkItem(int page, const QString& label = QString(), const QString& comment = QString(), const QDateTime& modified = QDateTime::currentDateTime()) :
        page(page),
        label(label),
        comment(comment),
        modified(modified) {}

};

class BookmarkModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit BookmarkModel(QObject* parent = 0);

    void addBookmark(const BookmarkItem& bookmark);
    void removeBookmark(const BookmarkItem& bookmark);
    void findBookmark(BookmarkItem& bookmark) const;

    enum
    {
        PageRole = Qt::UserRole + 1,
        LabelRole = Qt::DisplayRole,
        CommentRole = Qt::ToolTipRole,
        ModifiedRole = Qt::UserRole + 2
    };

    Qt::ItemFlags flags(const QModelIndex&) const;

    int columnCount(const QModelIndex&) const;
    int rowCount(const QModelIndex& parent) const;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    QList< BookmarkItem > m_bookmarks;

};

#endif // BOOKMARKMODEL_H