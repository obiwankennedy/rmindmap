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
#ifndef NODEIMAGEPROVIDER_H
#define NODEIMAGEPROVIDER_H

#include <QQuickImageProvider>

class ImageModel : public QObject
{
    Q_OBJECT
public:
    ImageModel(QObject* parent= nullptr);

    void appendData(const QString& id, const QPixmap& map);

    QPixmap pixmapFromId(const QString& id) const;

    bool hasPixmap(const QString& id) const;

    const QHash<QString, QPixmap>& data() const;

private:
    QHash<QString, QPixmap> m_pixmaps;
};

class NodeImageProvider : public QQuickImageProvider
{
public:
    NodeImageProvider(ImageModel* model);

    QPixmap requestPixmap(const QString& id, QSize* size, const QSize& requestedSize) override;

private:
    QPointer<ImageModel> m_dataModel;
};

#endif // NODEIMAGEPROVIDER_H
