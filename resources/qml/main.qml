import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Universal 2.12
import RMindMap 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 800
    height: 400
    property real viewScale: 1
    property int idx: 0
    property bool darkMode: false

    onDarkModeChanged: _engineCtrl.nightMode = root.darkMode

    Universal.theme: root.darkMode ? Universal.Dark: Universal.Light

    MindMapController {
        id: ctrl
    }

    FileDialog {
        id: importDialog
        title: qsTr("Please, select a file to import")
        folder: shortcuts.home
        selectMultiple: false
        nameFilters: ["Text file (*.txt)"]
        onAccepted: {
            ctrl.importFile(importDialog.fileUrl)
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
            ctrl.setFilename(openDialog.fileUrl)
            close()
        }
        onRejected: close()
    }

    FileDialog {
        id: saveDialog
        title: qsTr("Save Mindmap into File")
        folder: shortcuts.home
        selectMultiple: false
        nameFilters: ["Rolisteam MindMap (*.rmap)"]
        onAccepted: {
            ctrl.setFilename(saveDialog.fileUrl)
            close()
        }
        onRejected: close()
    }

    Popup {
        id: stylePopup
        GridLayout {
            columns: 3
            Repeater {
                model: ctrl.styleModel
                delegate: Rectangle {
                    radius: 8
                    border.width: 1
                    border.color: "black"
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: colorOne }
                        GradientStop { position: 1.0; color: colorTwo }
                    }

                    Text {
                        anchors.centerIn: parent
                        color: textColor
                    }
                }
            }
        }

    }

    MindMap {
        anchors.fill: parent
        ctrl: ctrl
    }
    MindMenu {
        id: menu
        ctrl: ctrl
    }


    RowLayout{
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.rightMargin: 14
        anchors.topMargin: 14
        IconButton {//undo
            source: _engineCtrl.undoIcon
            enabled: ctrl.canUndo
            onClicked: ctrl.undo()
        }
        IconButton {//redo
            source: _engineCtrl.redoIcon
            enabled: ctrl.canRedo
            onClicked: ctrl.redo()
        }
        IconButton {
            source: _engineCtrl.listIcon
            onClicked: drawer.open()
        }
    }

    Drawer {
        id: drawer
        edge: Qt.RightEdge
        width: 0.33 * root.width
        height: root.height
        Label {
            text: "Content goes here!"
            anchors.centerIn: parent
        }
        Switch {
            text: qsTr("Night Mode")
            checked: false
            onCheckedChanged: root.darkMode = checked
        }
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
        onWheel: {
            var step = (wheel.modifiers & Qt.ControlModifier) ? 0.1 : 0.01
            if(wheel.angleDelta.y>0)
            {
                root.viewScale = Math.min(root.viewScale+step,2.0)
            }
            else
                root.viewScale = Math.max(root.viewScale-step,0.2)
        }
    }
}
