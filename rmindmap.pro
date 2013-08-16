HEADERS += \
        edge.h \
        node.h \
        graphwidget.h \
    mindtoolbar.h \
    graphitemmodel.h \
    mainwindow.h \
    serializable.h \
    graphedgemodel.h \
    graphcommonmodel.h \


SOURCES += \
        edge.cpp \
        main.cpp \
        node.cpp \
        graphwidget.cpp \
    mindtoolbar.cpp \
    graphitemmodel.cpp \
    mainwindow.cpp \
    graphedgemodel.cpp \
    graphcommonmodel.cpp \




include(preferences/preferences.pri)
include(detailpanel/detailpanel.pri)
include(widgets/widgets.pri)
include(items/items.pri)
include(browser/browser.pri)
include(stringmanager/stringmanager.pri)


INCLUDEPATH += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT+= svg

target = bin/rmindmap

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/graphicsview/elasticnodes
#sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS elasticnodes.pro
#sources.path = $$[QT_INSTALL_EXAMPLES]/graphicsview/elasticnodes
#INSTALLS += target sources



