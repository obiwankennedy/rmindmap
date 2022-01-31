/***************************************************************************
 *	Copyright (C) 2019 by Renaud Guezennec                                 *
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
#ifndef MINDNODE_H
#define MINDNODE_H
#include <QObject>
#include <QPointF>
#include <QVector2D>

#include "data/positioneditem.h"
#include "minditem.h"

class Link;
class MindNode : public PositionedItem
{
    Q_OBJECT
    Q_PROPERTY(QString imageUri READ imageUri WRITE setImageUri NOTIFY imageUriChanged)
    Q_PROPERTY(int styleIndex READ styleIndex WRITE setStyleIndex NOTIFY styleIndexChanged)
public:
    MindNode(QObject* parent= nullptr);
    virtual ~MindNode();

    QString imageUri() const;
    int styleIndex() const;

public slots:
    void setImageUri(const QString& uri);
    void setStyleIndex(int idx);

signals:
    void imageUriChanged();
    void styleIndexChanged();

private:
    QString m_imageUri;
    int m_styleIndex= 0; // default
};

#endif // MINDNODE_H
