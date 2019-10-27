import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12


Pane
{
    id: root
    property alias source: img.source
    property alias text: text.text
    property color colorOne: "white"
    property color colorTwo: "gray"
    property color colorBorder: "black"
    property bool isEditable: false
    property bool selected: false | isEditable
    property alias open: control.open
    property QtObject object
    onWidthChanged: node.contentWidth = width
    onHeightChanged: node.contentHeight = height
    onXChanged: {
        if(mouse.drag.active)
            node.position=Qt.point(x, y)
    }
    onYChanged: {
       if(mouse.drag.active)
            node.position=Qt.point(x, y)
   }

    signal addChild()

    Connections {
        target: node
        onPositionChanged: {
            x=node.position.x
            y=node.position.y
        }
    }

    ColumnLayout {
        Image {
            id: img
            visible: source
        }
        TextInput{
            id: text
            enabled: root.isEditable
            onEnabledChanged: focus = enabled
            onEditingFinished: root.isEditable = false
        }
    }

    background: Rectangle {
        radius: 8
        border.width: root.selected ? 4 : 1
        border.color: root.selected ? "blue": "black"
        gradient: Gradient {
            GradientStop { position: 0.0; color: root.colorOne }
            GradientStop { position: 1.0; color: root.colorTwo }
        }
        MouseArea {
            id: mouse
            anchors.fill: parent
            drag.target: root
            drag.axis: Drag.XAndYAxis
            drag.minimumX: 0
            drag.minimumY: 0
            onPressed: root.selected = true
            onDoubleClicked: root.isEditable = true
            drag.onActiveChanged: root.object.isDragged = drag.active

        }
        AbstractButton {
            id: control
            property bool open: !checked
            checkable: true
            visible: object.hasLink
            width: 20
            height: 20
            anchors.top: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            topPadding: 0
            padding: 0
            contentItem: Text {
                topPadding: 0
                padding: 0
                verticalAlignment: Text.AlignTop
                horizontalAlignment: Text.AlignHCenter
                text: control.checked ? qsTr("-") : qsTr("+")
            }
            background: Rectangle{
                border.width: 1
                border.color: "black"
                color: "transparent"
            }
        }

        Rectangle {
            color: "blue"
            width: 10
            opacity: 0.7
            radius: parent.radius
            height: parent.height
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            MouseArea {
                anchors.fill: parent
                onClicked: root.addChild()
            }
        }
    }
}
