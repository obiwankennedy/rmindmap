import QtQuick 2.0
import QtQuick.Controls 2.12
import RMindMap 1.0

MindLink {
    id: root
    property alias text: label.text
    property bool editable: false
    property alias visibleLabel: label.visible
    property alias opacityLabel: label.opacity
    property color colorBorder: "gray"
    property int borderWidth: 1
    property color backgroundLabel: "lightgray"
    property int radius: 5
    property real opacityLabel: 0.8
    property QtObject object


    TextField {
        id: label
        anchors.centerIn: parent
        text: label
        readOnly: !root.editable
        onReadOnlyChanged: focus = root.editable
        onEditingFinished: root.editable = false

        background: Rectangle {
            border.width: root.borderWidth
            border.color: root.colorBorder
            color: root.backgroundLabel
            radius: root.radius
            opacity: root.opacityLabel

        }
        MouseArea {
            id: mouse
            anchors.fill: parent
            onDoubleClicked: {
                console.log("double click")

                root.editable = true
            }
        }
    }
}
