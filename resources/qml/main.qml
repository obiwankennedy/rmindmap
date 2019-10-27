import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.3
import RMindMap 1.0

ApplicationWindow {
  id: root
  visible: true
  width: 800
  height: 400
  property real viewScale: 1
  property int idx: 0

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




  // make own item
  Flickable {
    id: flick
    anchors.fill: parent
    contentHeight: mapmind.width
    contentWidth: mapmind.height
    contentX: 0
    contentY: 0
    interactive: true


    Item {
      id: mapmind
      x: 0
      y: 0
      width: 2000
      height:2000
      scale: root.viewScale
      transformOrigin: Item.Center
      Repeater {
        anchors.fill: parent
        model: ctrl.linkModel
        delegate: MindLink{
          width: widthLink
          height: heightLink
          start:position
          end:last
          visible: link.visible
        }
      }
      Repeater {
        anchors.fill: parent
        model: ctrl.nodeModel
        delegate: Node {
            x: position.x
            y: position.y
            object: node
            focus:true
            text : label
            visible: node.visible
            onAddChild: ctrl.nodeModel.addBox(node.id)
            onOpenChanged: ctrl.nodeModel.openNode(node.id, open)
          }
      }
    }
  }
  Menu {
      id: menu
      MenuItem {
          text: qsTr("New")
          onTriggered: ctrl.resetData()
      }
      MenuItem {
          text: qsTr("Open File…")
          onTriggered: openDialog.open()
      }
      MenuItem {
          text: qsTr("Save")
          onTriggered: ctrl.saveFile();
      }
      MenuItem {
          text: qsTr("Save As…")
          onTriggered: {
              saveDialog.open()
              ctrl.saveFile();
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
          checked: ctrl.spacing
          onTriggered: ctrl.spacing = checked
      }
  }
    MouseArea {
      anchors.fill:parent
      acceptedButtons:Qt.MiddleButton | Qt.RightButton
      onPressed:{
          menu.x = mouse.x
          menu.y = mouse.y
          menu.open()
      }
      onWheel: {
        var step = (wheel.modifiers & Qt.ControlModifier) ? 0.1 : 0.01
        if(wheel.angleDelta.y>0)
        {
          root.viewScale = Math.min(step+root.viewScale,2.0)
        }
        else
          root.viewScale = Math.max(step-root.viewScale,0.2)
      }
    }
}
