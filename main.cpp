/***************************************************************************
 *	Copyright (C) 2011 by Renaud Guezennec                             *
 *   http://renaudguezennec.homelinux.org/accueil,3.html                   *
 *                                                                         *
 *   rmindmap is free software; you can redistribute it and/or modify  *
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

#include <QAbstractItemModel>
#include <QApplication>
#include <QOpenGLContext>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QSurfaceFormat>

#include "controller/mindmapcontroller.h"
#include "controller/selectioncontroller.h"
#include "data/nodestyle.h"
#include "qmlItems/linkitem.h"
#include "worker/theme.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setOrganizationName(QStringLiteral("Rolisteam"));
    app.setOrganizationDomain(QStringLiteral("org.rolisteam"));
    Q_INIT_RESOURCE(rmindmap);
    auto format= QSurfaceFormat::defaultFormat();
    if(QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL)
    {
        format.setVersion(3, 2);
        format.setProfile(QSurfaceFormat::CoreProfile);
    }
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setSamples(8);
    QSurfaceFormat::setDefaultFormat(format);

    QQuickStyle::setStyle("Universal");

    QQmlApplicationEngine qmlEngine;

    qmlEngine.rootContext()->setContextProperty("_engineCtrl", new Theme(&qmlEngine));
    qmlRegisterType<MindMapController>("RMindMap", 1, 0, "MindMapController");
    qmlRegisterType<SelectionController>("RMindMap", 1, 0, "SelectionController");
    qmlRegisterType<LinkItem>("RMindMap", 1, 0, "MindLink");
    qmlRegisterType<NodeStyle>("RMindMap", 1, 0, "NodeStyle");
    qRegisterMetaType<QAbstractItemModel*>();

    qmlEngine.load(QLatin1String("qrc:/resources/qml/main.qml"));

    return app.exec();
}
