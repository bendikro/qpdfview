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

#include "presentationview.h"

PresentationView::PresentationView(QMutex* mutex, Poppler::Document* document) : QWidget(),
    m_mutex(0),
    m_document(0),
    m_numberOfPages(-1),
    m_currentPage(-1),
    m_returnToPage(-1),
    m_links(),
    m_scaleFactor(1.0),
    m_boundingRect(),
    m_linkTransform(),
    m_image1(),
    m_image2(),
    m_render(0)
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setWindowState(windowState() | Qt::WindowFullScreen);
    setMouseTracking(true);

    m_render = new QFutureWatcher< void >(this);
    connect(m_render, SIGNAL(finished()), SLOT(on_render_finished()));

    m_mutex = mutex;
    m_document = document;

    m_numberOfPages = m_document->numPages();
    m_currentPage = 1;
    m_returnToPage = -1;

    prepareView();
}

PresentationView::~PresentationView()
{
    m_render->cancel();
    m_render->waitForFinished();
}

int PresentationView::numberOfPages() const
{
    return m_numberOfPages;
}

int PresentationView::currentPage() const
{
    return m_currentPage;
}

void PresentationView::previousPage()
{
    jumpToPage(currentPage() - 1);
}

void PresentationView::nextPage()
{
    jumpToPage(currentPage() + 1);
}

void PresentationView::firstPage()
{
    jumpToPage(1);
}

void PresentationView::lastPage()
{
    jumpToPage(numberOfPages());
}

void PresentationView::jumpToPage(int page)
{
    if(m_currentPage != page && page >= 1 && page <= m_numberOfPages)
    {
        m_returnToPage = m_currentPage;
        m_currentPage = page;

        prepareView();
    }
}

void PresentationView::on_render_finished()
{
    if(!m_render->isCanceled())
    {
        m_image1 = m_image2;
    }

    if(!m_render->isRunning())
    {
        m_image2 = QImage();
    }

    update();
}

void PresentationView::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);

    prepareView();
}

void PresentationView::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter;
    painter.begin(this);

    painter.fillRect(rect(), QBrush(Qt::black));

    if(!m_image1.isNull())
    {
        painter.drawImage(m_boundingRect.topLeft(), m_image1);
    }
    else
    {
        if(!m_render->isRunning())
        {
            m_render->setFuture(QtConcurrent::run(this, &PresentationView::render, m_currentPage - 1, m_scaleFactor));
        }
    }

    painter.end();
}

void PresentationView::keyPressEvent(QKeyEvent* event)
{
    if(event->modifiers() == Qt::NoModifier)
    {
        switch(event->key())
        {
        case Qt::Key_PageUp:
        case Qt::Key_Up:
        case Qt::Key_Left:
        case Qt::Key_Backspace:
            previousPage();

            event->accept();
            return;
        case Qt::Key_PageDown:
        case Qt::Key_Down:
        case Qt::Key_Right:
        case Qt::Key_Space:
            nextPage();

            event->accept();
            return;
        case Qt::Key_Home:
            firstPage();

            event->accept();
            return;
        case Qt::Key_End:
            lastPage();

            event->accept();
            return;
        case Qt::Key_Return:
            jumpToPage(m_returnToPage);

            event->accept();
            return;
        case Qt::Key_F12:
        case Qt::Key_Escape:
            close();

            event->accept();
            return;
        }
    }

    QWidget::keyPressEvent(event);
}

void PresentationView::mouseMoveEvent(QMouseEvent* event)
{
    QApplication::restoreOverrideCursor();

    foreach(Poppler::Link* link, m_links)
    {
        if(m_linkTransform.mapRect(link->linkArea().normalized()).contains(event->pos()))
        {
            if(link->linkType() == Poppler::Link::Goto)
            {
                if(!static_cast< Poppler::LinkGoto* >(link)->isExternal())
                {
                    QApplication::setOverrideCursor(Qt::PointingHandCursor);
                    QToolTip::showText(event->globalPos(), tr("Go to page %1.").arg(static_cast< Poppler::LinkGoto* >(link)->destination().pageNumber()));

                    return;
                }
            }
        }
    }

    QToolTip::hideText();
}

void PresentationView::mousePressEvent(QMouseEvent* event)
{
    foreach(Poppler::Link* link, m_links)
    {
        if(m_linkTransform.mapRect(link->linkArea().normalized()).contains(event->pos()))
        {
            if(link->linkType() == Poppler::Link::Goto)
            {
                Poppler::LinkGoto* linkGoto = static_cast< Poppler::LinkGoto* >(link);

                if(!linkGoto->isExternal())
                {
                    int page = linkGoto->destination().pageNumber();

                    page = page >= 1 ? page : 1;
                    page = page <= m_numberOfPages ? page : m_numberOfPages;

                    jumpToPage(page);
                }
            }

            return;
        }
    }
}

void PresentationView::prepareView()
{
    m_mutex->lock();

    Poppler::Page* page = m_document->page(m_currentPage - 1);

    qDeleteAll(m_links);
    m_links = page->links();

    QSizeF size = page->pageSizeF();

    delete page;

    m_mutex->unlock();

    {
        m_scaleFactor = qMin(width() / size.width(), height() / size.height());

        m_boundingRect.setLeft(0.5 * (width() - m_scaleFactor * size.width()));
        m_boundingRect.setTop(0.5 * (height() - m_scaleFactor * size.height()));
        m_boundingRect.setWidth(m_scaleFactor * size.width());
        m_boundingRect.setHeight(m_scaleFactor * size.height());

        m_linkTransform.reset();
        m_linkTransform.translate(m_boundingRect.left(), m_boundingRect.top());
        m_linkTransform.scale(m_boundingRect.width(), m_boundingRect.height());
    }

    m_image1 = QImage();
    m_render->cancel();

    update();
}

void PresentationView::render(int index, qreal scaleFactor)
{
    QMutexLocker mutexLocker(m_mutex);

    if(m_render->isCanceled())
    {
        return;
    }

    Poppler::Page* page = m_document->page(index);

    m_image2 = page->renderToImage(scaleFactor * 72.0, scaleFactor * 72.0);

    delete page;
}
