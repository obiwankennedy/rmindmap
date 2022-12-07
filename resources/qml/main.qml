import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Universal 2.12
import RMindMap 1.0

ApplicationWindow {
    id: root
    visible: true
    visibility: ApplicationWindow.Maximized
    minimumHeight: 800
    minimumWidth: 1024


    property int idx: 0
    property bool darkMode: false
    property alias addPackage: _buttonGrid.addPackage

    onDarkModeChanged: Theme.nightMode = root.darkMode

    Universal.theme: root.darkMode ? Universal.Dark: Universal.Light


    FileDialog {
        id: importDialog
        title: qsTr("Please, select a file to import")
        folder: shortcuts.home
        selectMultiple: false
        nameFilters: ["Text file (*.txt)"]
        onAccepted: {
            MainController.importFile(importDialog.fileUrl)
            close()
        }
        onRejected: close()
    }

    FileDialog {
        id: openDialog
        title: qsTr("Load Mindmap from File")
        folder: shortcuts.home
        selectMultiple: false
        nameFilters: ["Rolisteam MindMap (*.rmap)"]
        onAccepted: {
            MainController.setFilename(openDialog.fileUrl)
            MainController.loadFile();
            close()
        }
        onRejected: close()
    }

    FileDialog {
        id: saveDialog
        title: qsTr("Save Mindmap into File")
        folder: shortcuts.home
        selectExisting: false
        selectMultiple: false
        defaultSuffix: "rmap"
        nameFilters: ["Rolisteam MindMap (*.rmap)"]
        onAccepted: {
            MainController.setFilename(saveDialog.fileUrl)
            MainController.saveFile();
            close()
        }
        onRejected: close()
    }

    Component.onCompleted: {
        MainController.setFilename("file:///home/renaud/documents/03_jdr/01_Scenariotheque/16_l5r/15_riz/riz.rmap")
        MainController.loadFile();
    }


    MindMap {
        id: mindMap
        anchors.fill: parent
        addSubLink: _buttonGrid.addArrow
        zoomLevel: slider.value
        onPressed:   {          if(root.addPackage)
                MainController.addPackage(Qt.point(mouse.x, mouse.y))}
        onPositionChanged: {
            if(root.addPackage)
                MainController.updatePackage(Qt.point(mouse.x, mouse.y))
        }

        onReleased: {
            if(root.addPackage)
                root.addPackage = false
        }
    }
    RowLayout {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        Slider {
            id: slider
            from: 0.01
            to: 10
            value: 1
        }
        Button {
            text: "reset"
            onClicked: slider.value = 1.0
        }
    }

    MindMenu {
        id: menu
        mindMapHeight: mindMap.contentHeight
        mindMapWidth: mindMap.contentWidth
    }

    ButtonGrid {
        id: _buttonGrid
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.rightMargin: 14
        anchors.topMargin: 14
        onOpenDrawer: drawer.open()
        onExportScene: mindMap.makeScreenShot()
    }

    SideMenu {
        id: drawer
        edge: Qt.RightEdge
        width: 0.33 * root.width
        height: root.height
        onDarkModeChanged: root.darkMode = darkMode
        onLinkVisibilityChanged: MainController.linkLabelVisibility = linkVisibility
        onDefaultStyleChanged: MainController.defaultStyleIndex = defaultStyle
    }



    MouseArea {
        anchors.fill:parent
        acceptedButtons:Qt.MiddleButton | Qt.RightButton
        propagateComposedEvents: true

        onClicked:{
            menu.x = mouse.x
            menu.y = mouse.y
            menu.open()
        }
        /*onWheel: {
            var step = (wheel.modifiers & Qt.ControlModifier) ? 0.1 : 0.01
            if(wheel.angleDelta.y>0)
            {
                root.viewScale = Math.min(root.viewScale+step,2.0)
            }
            else
                root.viewScale = Math.max(root.viewScale-step,0.2)
        }*/
    }
}
