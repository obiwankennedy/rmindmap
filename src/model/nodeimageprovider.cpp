/***************************************************************************
 *	Copyright (C) 2021 by Renaud Guezennec                               *
 *   http://www.rolisteam.org/contact                                      *
 *                                                                         *
 *   This software is free software; you can redistribute it and/or modify *
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
#include "nodeimageprovider.h"

NodeImageProvider::NodeImageProvider(ImageModel* model)
    : QQuickImageProvider(QQuickImageProvider::Pixmap), m_dataModel(model)
{
}

QPixmap NodeImageProvider::requestPixmap(const QString& id, QSize* size, const QSize& requestedSize)
{
    Q_UNUSED(size)

    QPixmap map= m_dataModel->pixmapFromId(id);

    if(requestedSize.isValid())
        map= map.scaled(requestedSize, Qt::KeepAspectRatio);

    *size= map.size();

    return map;
}

ImageModel::ImageModel(QObject* parent) : QObject(parent) {}

void ImageModel::appendData(const QString& id, const QPixmap& map)
{
    m_pixmaps.insert(id, map);
}

QPixmap ImageModel::pixmapFromId(const QString& id) const
{
    return m_pixmaps.value(id);
}

bool ImageModel::hasPixmap(const QString& id) const
{
    return m_pixmaps.contains(id);
}

const QHash<QString, QPixmap>& ImageModel::data() const
{
    return m_pixmaps;
}
