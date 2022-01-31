import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Universal 2.12
import RMindMap 1.0

Flickable {
    id: flick
    property QtObject ctrl

    contentHeight: mapmind.height
    contentWidth: mapmind.width
    contentX: 0
    contentY: 0
    interactive: true
    boundsBehavior: Flickable.StopAtBounds

    ScrollBar.vertical: ScrollBar { }
    ScrollBar.horizontal: ScrollBar { }

    Shortcut {
        sequence: StandardKey.Undo
        onActivated:  ctrl.undo();
    }
    Shortcut {
        sequence: StandardKey.Redo
        onActivated: ctrl.redo();
    }

    Component {
        id: linkComp
        Link {
            x: item.startPoint.x
            y: item.startPoint.y
            width: item.width
            height: item.height
            color: Universal.foreground
            start: item.startPoint
            end: item.endPoint
            startBox: item.startBox
            endBox: item.endBox
            visible: item.visible
            text: item.text

            /*Timer {
                running: true
                repeat: true
                onTriggered: console.log("link: ")
            }*/
        }
    }

    Component {
        id: packComp
        Item {
            x: position.x
            y: position.y
            width: widthLink
            height: heightLink
            Rectangle {
                id: titleBox
                anchors.top: parent.top
                anchors.left: parent.left
                width: 40
                height: 20
/*
                Label {
                    anchors.centerIn:  parent
                    text: label
                }*/
            }
            Rectangle {
                anchors.top: titleBox.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }
            Timer {
                running: true
                repeat: true
                onTriggered: console.log("packComp: ")
            }
        }
    }


    Component {
        id: nodeComp
        Node {
            id: nodeItem
            x: item.position.x
            y: item.position.y
            object: item
            nodeStyle: ctrl.style(item.styleIndex)
            focus: true
            text : item.text
            visible: item.visible
            selected: item.selected
            buttonColor: Universal.foreground
            onAddChild: ctrl.addBox(item.id)
            onOpenChanged: ctrl.itemModel.openItem(item.id, open)
            onReparenting: ctrl.reparenting(node,id)
            onSelectStyle: {
                stylePopup.parent = nodeItem
                stylePopup.open()
            }
            onTextChanged: item.text = nodeItem.text

            onClicked: {
                if(mouse.modifiers & Qt.ControlModifier) {
                    selected ? ctrl.selectionCtrl.removeFromSelection(item) : ctrl.selectionCtrl.addToSelection(item)
                }
                else if(!selected){
                    ctrl.selectionCtrl.clearSelection()
                    ctrl.selectionCtrl.addToSelection(item)
                }
            }

        }
    }

    Popup {
        id: stylePopup
        modal: true
        ColumnLayout {
            anchors.fill: parent
            GroupBox {
                id: styleTab
                title: qsTr("Node Styles")
                Layout.fillWidth: true
                Layout.fillHeight: true
                contentWidth: flickable.contentWidth
                contentHeight: flickable.contentHeight
                ScrollView {
                    id: flickable
                    anchors.fill: parent
                    contentWidth: grid.implicitWidth+15
                    contentHeight: grid.implicitHeight
                    ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                    clip: true
                    GridLayout {
                        id: grid
                        columns: 3

                        Repeater {
                            model: ctrl.styleModel
                            Button {
                                id: control
                                width: 80
                                height: 30
                                background: Rectangle {
                                    radius: 8
                                    border.width: 1
                                    border.color: "black"
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: control.pressed ? colorTwo : colorOne }//
                                        GradientStop { position: 1.0; color: control.pressed ? colorOne : colorTwo }//
                                    }

                                }
                                contentItem: Text {
                                    color: textColor
                                    text: qsTr("Text")
                                }
                                onClicked: {
                                    stylePopup.parent.object.styleIndex = index
                                    stylePopup.close()
                                }
                            }
                        }
                    }
                }
            }

        }

    }

    Pane {
        id: mapmind
        x: 0
        y: 0
        width: Math.max(2000, ctrl.contentRect.width)
        height: Math.max(1000, ctrl.contentRect.height)

        scale: root.viewScale
        transformOrigin: Item.Center
       /*Rectangle {
            anchors.fill: parent
            color: "red"
            opacity: 0.4
        }*/


        MouseArea {
            anchors.fill:parent
            acceptedButtons:Qt.LeftButton
            onClicked: ctrl.selectionCtrl.clearSelection()
        }
        Repeater {
            id: repeater
            anchors.fill: parent
            model: ctrl.itemModel


            delegate: Loader {
                property string text: label
                property QtObject item:  object
                property bool isSelected: selected
                property bool isVisible: visible
                /*Timer {
                    running: true
                    repeat: true
                    onTriggered: console.log("repeater: "+type+" value:"+MindItem.LinkType)
                }*/

                sourceComponent: type == MindItem.PackageType ? packComp : type == MindItem.LinkType ? linkComp : nodeComp
            }
        }

    }
}
