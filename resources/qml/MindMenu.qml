import QtQuick 2.12
import QtQuick.Controls 2.2
import RMindMap 1.0

Menu {
    id: menu
    required property real mindMapHeight
    required property real mindMapWidth
    MenuItem {
        text: qsTr("New")
        onTriggered: MainController.resetData()
    }
    MenuItem {
        text: qsTr("Open File…")
        onTriggered: openDialog.open()
    }
    MenuItem {
        text: qsTr("Save")

        onTriggered: {
            if(MainController.filename)
                MainController.saveFile();
            else
                saveDialog.open()
        }
    }
    MenuItem {
        text: qsTr("Save As…")
        onTriggered: {
            saveDialog.open()
        }
    }
    MenuItem {
        text: qsTr("Remove Selection")
        onTriggered: {
            MainController.removeSelection();
        }
    }
    MenuSeparator { }
    MenuItem {
        text: qsTr("Undo")
        enabled: MainController.canUndo
        onTriggered: {
            MainController.undo()
        }
    }
    MenuItem {
        text: qsTr("Redo")
        enabled: MainController.canRedo
        onTriggered: {
            MainController.redo()
        }
    }
    MenuSeparator { }
    MenuItem {
        text: qsTr("Center")
        onTriggered: {
            MainController.centerItems(menu.mindMapWidth, menu.mindMapHeight)
        }
    }
    MenuSeparator { }
    MenuItem {
        text: qsTr("Import File…")
        onTriggered: {
            importDialog.open()
        }
    }
    MenuSeparator { }
    MenuItem {
        text: qsTr("Automatic Spacing")
        checkable: true
        checked: MainController.spacing
        onTriggered: MainController.spacing = checked
    }
}
