import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Universal 2.12
import RMindMap 1.0

Flickable {
    id: flick
    property bool addSubLink: false
    property real zoomLevel: 1
    property string selectedNodeId:  ""
    property bool isDark: (Universal.theme == Universal.Dark)

    signal pressed(var mouse)
    signal positionChanged(var mouse)
    signal released(var mouse)

    function makeScreenShot(){
        inner.grabToImage(function(result){
                 result.saveToFile("/home/renaud/documents/03_jdr/01_Scenariotheque/16_l5r/15_riz/plan.png")
                });
    }


    onAddSubLinkChanged: flick.selectedNodeId = ""

    contentHeight: (MainController.contentRect.height +200) * flick.zoomLevel
    contentWidth: (MainController.contentRect.width +200) * flick.zoomLevel

    /* onZoomLevelChanged: {
             var zoomPoint = Qt.point(flick.width/2 + flick.contentX,
                                  flick.height/2 + flick.contentY);

             flick.resizeContent((inner.width * zoomLevel), (inner.height * zoomLevel), zoomPoint);
             flick.returnToBounds();
     }*/
    /*Component.onCompleted: {
        zoomLevel=1.01
        zoomLevel=1
    }*/

    //ScrollBar.horizontal.interactive: true
    //ScrollBar.vertical.interactive: true
    contentX: -contentWidth/2 * flick.zoomLevel
    contentY: -contentHeight/2 * flick.zoomLevel
    interactive: true
    /*Timer {
        running: true
        repeat: true
        onTriggered: console.log("content:"+flick.contentY+" "+flick.contentWidth+" "+flick.contentX+" "+flick.contentHeight)
    }*/

    boundsBehavior: Flickable.StopAtBounds

    ScrollBar.vertical: ScrollBar { }
    ScrollBar.horizontal: ScrollBar { }

    Shortcut {
        sequence: StandardKey.Undo
        onActivated:  MainController.undo();
    }
    Shortcut {
        sequence: StandardKey.Redo
        onActivated: MainController.redo();
    }

    Component {
        id: linkComp
        Link {
            x: item.topLeftCorner.x
            y: item.topLeftCorner.y
            width: item.normalizedWidth
            height: item.normalizedHeight
            color: Universal.foreground
            opacity: item.constraint ? 1.0 : 0.4
            start: item.startPoint
            end: item.endPoint
            orientation: item.orientation
            startBox: item.startBox
            endBox: item.endBox
            object: item
            visible: item.visible
            text: item.text
            visibleLabel: MainController.linkLabelVisibility

            onSelected: {
                MainController.selectionCtrl.clearSelection()
                MainController.selectionCtrl.addToSelection(item)
            }
            Rectangle {
                border.width: 1
                border.color: "red"
                color: "transparent"
                visible: item.selected
                anchors.fill: parent
            }
        }
    }

    Component {
        id: packComp
        PackageItem {
            x: item.position.x
            y: item.position.y
            width: item.width
            height: item.height
            title: item.title
            onAddItem: MainController.addItemIntoPackage(id, item.id)
        }
    }


    Component {
        id: nodeComp
        Node {
            id: nodeItem
            x: item.position.x
            y: item.position.y
            object: item
            nodeStyle: MainController.style(item.styleIndex)
            focus: true
            text : item.text ? item.text : "new node"
            source: hasAvatar ? "image://nodes/%1".arg(item.id) : ""
            visible: item.visible
            selected: item.selected
            buttonColor: Universal.foreground
            onAddChild: MainController.addBox(item.id)
            onOpenChanged: MainController.itemModel.openItem(item.id, open)
            onReparenting: MainController.reparenting(item,id)
            onDropImage:  MainController.addImageFor(item.id, paths[0])
            onSelectStyle: {
                _stylePopup.parent = nodeItem
                _stylePopup.node = item
                _stylePopup.open()
            }


            onClicked: {
                if(flick.addSubLink)
                {
                    if(flick.selectedNodeId.length > 0)
                    {
                        MainController.addLink(flick.selectedNodeId, item.id)
                        flick.selectedNodeId = ""
                    }
                    else
                        flick.selectedNodeId = item.id

                }
                else if(mouse.modifiers & Qt.ControlModifier) {
                    selected ? MainController.selectionCtrl.removeFromSelection(item) : MainController.selectionCtrl.addToSelection(item)
                }
                else if(!selected){
                    MainController.selectionCtrl.clearSelection()
                    MainController.selectionCtrl.addToSelection(item)
                }
            }

        }
    }

    NodeSettingPopup {
        id: _stylePopup
    }


    Item {
        id: inner
        width: (MainController.contentRect.width + 200)
        height: (MainController.contentRect.height + 200)
        transformOrigin: Item.Center
        scale: flick.zoomLevel
        MouseArea {
            anchors.fill:parent
            acceptedButtons: Qt.LeftButton
            onClicked: MainController.selectionCtrl.clearSelection()
            onPressed: flick.pressed(mouse)
            onPositionChanged: flick.positionChanged(mouse)
            onReleased: flick.released(mouse)
        }
        Repeater {
            id: repeater
            anchors.fill: parent
            model: MainController.itemModel


            delegate: Loader {
                property string text: label
                property QtObject item:  object
                property bool isSelected: selected
                property bool isVisible: visible
                property bool hasAvatar: hasPicture

                sourceComponent: type == MindItem.PackageType ? packComp : type == MindItem.LinkType ? linkComp : nodeComp
            }
        }

    }
}
