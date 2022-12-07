cmake_minimum_required(VERSION 3.15)

project(rmindmap VERSION 1.0.0 DESCRIPTION "Map your mind")
option(BUILD_GUI OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)



set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")


set(QT_REQUIRED_VERSION "5.12.0")
find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED COMPONENTS Core Svg Quick QuickControls2)


set(mindmap_header
    src/command/commandhelper.h
    #src/controller/mindmapcontroller.h
    src/controller/selectioncontroller.h
    src/controller/spacingcontroller.h
    src/controller/sidemenucontroller.h
    src/data/linkcontroller.h
    src/data/positioneditem.h
    src/data/mindnode.h
    src/data/packagenode.h
    src/command/addlinkcommand.h
    src/data/nodestyle.h
    src/model/linkmodel.h
    src/data/minditem.h
    src/geometry/linknode.h
    src/model/nodestylemodel.h
    src/model/nodeimageprovider.h
    src/qmlItems/linkitem.h
    src/command/additemcommand.h
    src/worker/fileserializer.h
    src/command/reparentingnodecommand.h
    src/command/dragnodecommand.h
    src/command/removenodecommand.h
    src/model/minditemmodel.h
    src/worker/theme.h)

        # main.cpp
set(mindmap_src
  src/command/addlinkcommand.cpp
    src/controller/selectioncontroller.cpp
    src/controller/spacingcontroller.cpp
    src/controller/sidemenucontroller.cpp
    src/data/linkcontroller.cpp
    src/data/mindnode.cpp
    src/data/nodestyle.cpp
    src/data/minditem.cpp
    src/data/packagenode.cpp
    src/model/linkmodel.cpp
    src/data/positioneditem.cpp
    src/command/dragnodecommand.cpp
    src/command/additemcommand.cpp
    src/command/removenodecommand.cpp
    src/model/nodestylemodel.cpp
    src/model/nodeimageprovider.cpp
    src/worker/fileserializer.cpp
    src/command/reparentingnodecommand.cpp
    src/geometry/linknode.cpp
    src/qmlItems/linkitem.cpp
    src/model/minditemmodel.cpp
    src/worker/theme.cpp)

set(mindmap_qml
    resources/qml/IconButton.qml
    resources/qml/Link.qml
    resources/qml/MindMap.qml
    resources/qml/MindMenu.qml
    resources/qml/Node.qml
)



set(mindmap_bin_src
    main.cpp
    src/controller/mindmapcontroller.cpp
    src/controller/mindmapcontroller.h)

add_library(rmindmap SHARED ${mindmap_header} ${mindmap_src} ${mindmap_qml} rmindmap.qrc)
target_link_libraries(rmindmap PUBLIC Qt5::Core Qt5::Svg Qt5::Svg Qt5::Qml Qt5::Quick Qt5::QuickControls2)
set_target_properties(rmindmap PROPERTIES VERSION ${PROJECT_VERSION})
target_include_directories(rmindmap PRIVATE src)

IF(BUILD_GUI)
    add_executable(rmindmap-gui ${mindmap_bin_src})
    target_link_libraries(rmindmap-gui PUBLIC rmindmap)
    target_include_directories(rmindmap-gui PRIVATE src)
ENDIF()
