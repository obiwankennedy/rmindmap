HEADERS += \
        src/graphwidget.h \
    src/mindtoolbar.h \
    src/graphitemmodel.h \
    src/mainwindow.h \
    src/serializable.h \
    src/graphedgemodel.h \
    src/graphcommonmodel.h \
    src/mindmap.h \
    src/menustyleitem.h \
    src/mindmapview.h


SOURCES += \
        src/main.cpp \
        src/graphwidget.cpp \
    src/mindtoolbar.cpp \
    src/graphitemmodel.cpp \
    src/mainwindow.cpp \
    src/graphedgemodel.cpp \
    src/graphcommonmodel.cpp \
    src/mindmap.cpp \
    src/menustyleitem.cpp \
    src/mindmapview.cpp




include(src/preferences/preferences.pri)
include(src/detailpanel/detailpanel.pri)
include(src/widgets/widgets.pri)
include(src/items/items.pri)
include(src/browser/browser.pri)
include(src/stringmanager/stringmanager.pri)


INCLUDEPATH += src/widgets src

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT+= svg

target = bin/rmindmap
CONFIG += c++11
# install
#target.path = $$[QT_INSTALL_EXAMPLES]/graphicsview/elasticnodes
#sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS elasticnodes.pro
#sources.path = $$[QT_INSTALL_EXAMPLES]/graphicsview/elasticnodes
#INSTALLS += target sources

FORMS += \
    src/menustyleitem.ui

RESOURCES += \
    rmindmap.qrc

DISTFILES += \
    resources/icons/shape.png



