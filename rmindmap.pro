HEADERS += \
    src/controller/mindmapcontroller.h \
    src/controller/selectioncontroller.h \
    src/controller/spacingcontroller.h \
    src/data/link.h \
    src/data/mindnode.h \
    src/model/linkmodel.h \
    src/geometry/linknode.h \
    src/qmlItems/linkitem.h \
    src/model/boxmodel.h
   # src/qmlItems/nodeitem.h \
   # src/geometry/boxnode.h \

INCLUDEPATH += src

SOURCES += \
    src/controller/mindmapcontroller.cpp \
    src/controller/selectioncontroller.cpp \
    src/controller/spacingcontroller.cpp \
    src/data/link.cpp \
    src/data/mindnode.cpp \
    src/model/linkmodel.cpp \
    main.cpp \
    src/geometry/linknode.cpp \
    src/qmlItems/linkitem.cpp \
    src/model/boxmodel.cpp
#    src/geometry/boxnode.cpp \
    #src/qmlItems/nodeitem.cpp \

QT+= svg qml widgets quick

target = bin/rmindmap
CONFIG += c++11

RESOURCES += \
    rmindmap.qrc

DISTFILES += \
    resources/icons/shape.png



OTHER_FILES += resources/qml/*.qml
