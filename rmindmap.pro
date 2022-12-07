HEADERS += \
    src/command/commandhelper.h \
    src/controller/mindmapcontroller.h \
    src/controller/selectioncontroller.h \
    src/controller/spacingcontroller.h \
    src/data/link.h \
    src/data/mindnode.h \
    src/data/nodestyle.h \
    src/model/linkmodel.h \
    src/geometry/linknode.h \
    src/model/nodestylemodel.h \
    src/qmlItems/linkitem.h \
    src/command/addnodecommand.h \
    src/worker/fileserializer.h \
    src/command/reparentingnodecommand.h \
    src/command/dragnodecommand.h  \
    src/command/removenodecommand.h \
    src/model/boxmodel.h \ \
    src/worker/theme.h



INCLUDEPATH += src

SOURCES += \
    src/controller/mindmapcontroller.cpp \
    src/controller/selectioncontroller.cpp \
    src/controller/spacingcontroller.cpp \
    src/data/link.cpp \
    src/data/mindnode.cpp \
    src/data/nodestyle.cpp \
    src/model/linkmodel.cpp \
    src/command/dragnodecommand.cpp  \
    src/command/addnodecommand.cpp  \
    src/command/removenodecommand.cpp \
    src/model/nodestylemodel.cpp \
    src/worker/fileserializer.cpp \
    src/command/reparentingnodecommand.cpp  \
    main.cpp \
    src/geometry/linknode.cpp \
    src/qmlItems/linkitem.cpp \
    src/model/boxmodel.cpp \
    src/worker/theme.cpp


QT+= svg qml widgets quick quickcontrols2

target = bin/rmindmap
CONFIG += c++11

RESOURCES += \
    rmindmap.qrc

DISTFILES += \
    resources/icons/shape.png \
    resources/qml/IconButton.qml \
    resources/qml/Link.qml \
    resources/qml/MindMap.qml \
    resources/qml/MindMenu.qml



OTHER_FILES += resources/qml/*.qml
