import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Universal 2.12

Flickable {
  id: flick
  property QtObject ctrl

  contentHeight: mapmind.width
  contentWidth: mapmind.height
  contentX: 0
  contentY: 0
  interactive: true

  Shortcut {
      sequence: StandardKey.Undo
      onActivated:  ctrl.undo();
  }
  Shortcut {
      sequence: StandardKey.Redo
      onActivated: ctrl.redo();
  }

  Pane {
    id: mapmind
    x: 0
    y: 0
    width: 2000
    height:2000
    scale: root.viewScale
    transformOrigin: Item.Center
    MouseArea {
      anchors.fill:parent
      acceptedButtons:Qt.LeftButton
      onClicked: ctrl.selectionCtrl.clearSelection()
    }
    Repeater {
      anchors.fill: parent
      model: ctrl.linkModel
      delegate: Link {
        x: position.x
        y: position.y
        color: Universal.foreground
        width: widthLink
        height: heightLink
        start: position
        end:last
        startBox: startBoxRole
        endBox: endBoxRole
        visible: link.visible
        text: label
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
          selected: node.selected
          onAddChild: ctrl.addBox(node.id)
          onOpenChanged: ctrl.nodeModel.openNode(node.id, open)
          onReparenting: ctrl.reparenting(node,id)
          onClicked: {
              if(mouse.modifiers & Qt.ControlModifier) {
                  selected ? ctrl.selectionCtrl.removeFromSelection(node) : ctrl.selectionCtrl.addToSelection(node)
              }
              else if(!selected){
                  ctrl.selectionCtrl.clearSelection()
                  ctrl.selectionCtrl.addToSelection(node)
              }
          }

        }
    }

  }
}
