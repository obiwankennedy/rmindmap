/***************************************************************************
 *	Copyright (C) 2019 by Renaud Guezennec                               *
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
#ifndef QMLENGINECONTROLLER_H
#define QMLENGINECONTROLLER_H

#include <QObject>
#include <memory>

class QQmlEngine;
class QQmlFileSelector;
class Theme : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool nightMode READ nightMode WRITE setNightMode NOTIFY nightModeChanged)
    Q_PROPERTY(QString undoIcon READ undoIcon NOTIFY undoIconChanged)
    Q_PROPERTY(QString redoIcon READ redoIcon NOTIFY redoIconChanged)
    Q_PROPERTY(QString listIcon READ listIcon NOTIFY listIconChanged)
    Q_PROPERTY(QString editIcon READ editIcon NOTIFY editIconChanged)
    Q_PROPERTY(QString addGrayArrow READ addGrayArrow NOTIFY grayArrowChanged)
    Q_PROPERTY(QString saveIcon READ saveIcon NOTIFY saveIconChanged)
    Q_PROPERTY(QString trashIcon READ trashIcon NOTIFY trashIconChanged)
    Q_PROPERTY(QString refreshIcon READ refreshIcon NOTIFY refreshIconChanged)
public:
    explicit Theme(QQmlEngine* m_engine, QObject* parent= nullptr);
    ~Theme();

    bool nightMode() const;

    QString undoIcon() const;
    QString redoIcon() const;
    QString listIcon() const;
    QString editIcon() const;
    QString addGrayArrow() const;
    QString saveIcon() const;
    QString trashIcon() const;
    QString refreshIcon() const;

    QString imagePath(const QString& image) const;
signals:
    void nightModeChanged();
    void undoIconChanged();
    void redoIconChanged();
    void listIconChanged();
    void editIconChanged();
    void grayArrowChanged();
    void saveIconChanged();
    void trashIconChanged();
    void refreshIconChanged();

public slots:
    void setNightMode(bool b);

private:
    QQmlEngine* m_engine;
    std::unique_ptr<QQmlFileSelector> m_selector;
    bool m_nightMode= false;
};

#endif // QMLENGINECONTROLLER_H
