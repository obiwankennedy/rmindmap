import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Universal 2.12
import RMindMap 1.0

Popup {
    id: _root
    modal: true
    property QtObject node
    ScrollView {
        id: flickable
        anchors.fill: parent
        contentWidth: mainLyt.implicitWidth+30
        contentHeight: mainLyt.implicitHeight
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
        clip: true
        ColumnLayout {
            id: mainLyt
            spacing: 0

            GridLayout {
                id: info
                columns: 1
                Layout.fillWidth: true
                Layout.rightMargin: 30
                Label {
                    text: qsTr("Description: ")
                }
                ScrollView {
                    id: view
                    Layout.rowSpan: 3
                    Layout.fillWidth: true
                    TextArea {
                        implicitHeight: 150
                        placeholderText: qsTr("Description")
                        text: _root.node ? _root.node.description : ""
                        onEditingFinished: _root.node.description = text
                    }
                }


                Label {
                    text: qsTr("tags:")
                }
                TextField {
                    text:_root.node ? _root.node.tagsText : ""
                    Layout.fillWidth: true
                    onEditingFinished: _root.node.tagsText = text
                }
            }

            GroupBox {
                id: styleTab
                title: qsTr("Node Styles")
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.rightMargin: 30
                //contentWidth: flickable.contentWidth
                //contentHeight: flickable.contentHeight
                GridLayout {
                    id: grid
                    columns: 3
                    anchors.fill: parent
                    Repeater {
                        model: MainController.styleModel
                        Button {
                            id: control
                            Layout.preferredWidth: 120
                            Layout.preferredHeight: 30

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
                                horizontalAlignment: Text.AlignHCenter
                            }
                            onClicked: {
                                _root.node.styleIndex = index
                                _root.close()
                            }
                        }
                    }
                }
            }
        }
    }

}
