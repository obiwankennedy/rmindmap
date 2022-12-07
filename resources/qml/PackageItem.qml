import QtQuick 2.0
import QtQuick.Controls 2.12

Item {
    id: _root
    property alias title: _titleBox.text
    property bool dropOver: false
    signal addItem(var id)

    Label {
        id: _titleBox
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

    }
    Rectangle {
        anchors.top: _titleBox.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        border.width: 1
        border.color: "black"
    }

    DropArea {
        anchors.fill: parent
        keys: [ "rmindmap/reparenting","text/plain" ]
        onDropped: {
                addItem(drop.text)
        }
        onEntered: {
            if(drag.source === root)
                drag.accepted = false

            if(drag.source !== root)
                _root.dropOver = true
        }
        onExited:_root.dropOver = false
    }
}

