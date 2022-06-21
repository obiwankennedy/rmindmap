import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import RMindMap 1.0

GridLayout{
    id: _root
    columns: 4
    property alias addPackage: _pack.checked
    property alias addArrow: addArrow.checked

    signal openDrawer()
    signal exportScene()

    IconButton {//undo
        source: Theme.undoIcon
        enabled: MainController.canUndo
        onClicked: MainController.undo()
    }
    IconButton {//redo
        source: Theme.redoIcon
        enabled: MainController.canRedo
        onClicked: MainController.redo()
    }
    IconButton {//add package
        id: _pack
        source: Theme.editIcon
        checkable: true
    }
    IconButton {
        source: Theme.listIcon
        onClicked: _root.openDrawer()
    }
    IconButton {//Add gray arrow
        id: addArrow
        source: Theme.addGrayArrow
        checkable: true
    }
    IconButton {//remove selection
        source: Theme.trashIcon
        enabled: MainController.hasSelection
        onClicked: MainController.removeSelection()
    }
    IconButton {//remove selection
        source: Theme.saveIcon

        onClicked: MainController.saveFile()
    }
    IconButton {//remove selection
        source: Theme.refreshIcon
        onClicked: MainController.refresh()
    }
    IconButton {//
        source: Theme.saveIcon
        ToolTip.visible: down
        ToolTip.text: qsTr("Export as Png")
        onClicked: _root.exportScene()
    }
}
