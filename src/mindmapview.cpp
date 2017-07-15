/***************************************************************************
    *   Copyright (C) 2016 by Renaud Guezennec                                *
    *   http://www.rolisteam.org/contact                                      *
    *                                                                         *
    *   rolisteam is free software; you can redistribute it and/or modify     *
    *   it under the terms of the GNU General Public License as published by  *
    *   the Free Software Foundation; either version 2 of the License, or     *
    *   (at your option) any later version.                                   *
    *                                                                         *
    *   This program is distributed in the hope that it will be useful,       *
    *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
    *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
    *   GNU General Public License for more details.                          *
    *                                                                         *
    *   You should have received a copy of the GNU General Public License     *
    *   along with this program; if not, write to the                         *
    *   Free Software Foundation, Inc.,                                       *
    *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
    ***************************************************************************/

#include "mindmapview.h"

MindMapView::MindMapView(QWidget* parent)
    : QGraphicsView(parent),m_counterZoom(0)
{
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform );
    setRubberBandSelectionMode(Qt::IntersectsItemBoundingRect);
    setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MindMapView::zoomIn()
{
    scaleView(qreal(1.2));
}

void MindMapView::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
void MindMapView::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}


void MindMapView::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() & Qt::ShiftModifier)
    {
        setResizeAnchor(QGraphicsView::AnchorUnderMouse);
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        // Scale the view / do the zoom
        double scaleFactor = 1.1;

        if((event->delta() > 0)&&(m_counterZoom<20))
        {
            scale(scaleFactor, scaleFactor);
            ++m_counterZoom;

        }
        else if(m_counterZoom>-20)
        {
            --m_counterZoom;
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }

        setResizeAnchor(QGraphicsView::NoAnchor);
        setTransformationAnchor(QGraphicsView::NoAnchor);
    }
    else
        QGraphicsView::wheelEvent(event);
}
void MindMapView::mouseReleaseEvent(QMouseEvent *event)
{
    m_lastPoint = QPoint();
    QGraphicsView::mouseReleaseEvent(event);
    if(dragMode()==QGraphicsView::ScrollHandDrag)
    {
        setDragMode(QGraphicsView::RubberBandDrag);
    }
}
void MindMapView::mouseMoveEvent(QMouseEvent *event)
{
    if((event->modifiers() & Qt::ShiftModifier)&&
       (event->buttons() & Qt::LeftButton))
    {
        if(!m_lastPoint.isNull())
        {
            QRectF rect = sceneRect();
            int dx = event->x() - m_lastPoint.x();
            int dy = event->y() - m_lastPoint.y();
            rect.translate(-dx,-dy);
            m_lastPoint = event->pos();
            setSceneRect(rect);
        }
        m_lastPoint = event->pos();
    }
    else
    {
        QGraphicsView::mouseMoveEvent(event);
    }
}
