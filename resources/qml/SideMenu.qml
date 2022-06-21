import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Universal 2.12
import RMindMap 1.0

Drawer {
    id: _drawer

    property alias darkMode: darkModeSwitch.checked
    property alias linkVisibility: linkVisible.checked
    property alias defaultStyle: combo.currentIndex
    //property alias errors: logs.text

    SideMenuController {
        id: ctrl
        controller: MainController
        criteria: criteria.currentIndex
        pattern: textField.text
    }

    ColumnLayout {
        anchors.fill: parent

        Switch {
            id: darkModeSwitch
            text: qsTr("Night Mode")
            checked: false
            //onCheckedChanged: root.darkMode = checked
        }
        Switch {
            id: linkVisible
            text: qsTr("Link label visible:")
            checked: false
            //onCheckedChanged: MainController.linkLabelVisibility = checked
        }
        RowLayout {
            Layout.fillWidth: true
            Label {
                text: qsTr("Default Style")
            }
            ComboBox {
                id: combo
                model: MainController.styleModel
                currentIndex: 0
                //onCurrentIndexChanged: MainController.defaultStyleIndex = currentIndex

                contentItem: Rectangle {
                    radius: 8
                    width: 80
                    height: 15
                    border.width: 1
                    border.color: "black"
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: MainController.style(combo.currentIndex).colorOne }
                        GradientStop { position: 1.0; color: MainController.style(combo.currentIndex).colorTwo }
                    }
                    Text {
                        anchors.centerIn: parent
                        color: MainController.style(combo.currentIndex).textColor
                        text: qsTr("Text")
                    }
                }


                delegate: ItemDelegate {
                    width: combo.width
                    height: 20

                    Rectangle {
                        radius: 8
                        width: 80
                        height: 15
                        anchors.centerIn: parent
                        border.width: 1
                        border.color: "black"
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: colorOne }
                            GradientStop { position: 1.0; color: colorTwo }
                        }
                        Label {
                            anchors.centerIn: parent
                            color: model.textColor
                            text: qsTr("Text")
                        }
                    }
                }
            }
        }
        GridLayout {
            columns: 2
            Layout.fillWidth: true
            Label {
                text: qsTr("Criteria")
            }
            ComboBox {
                id: criteria
                model: ["None", "Name", "Tag", "Parent"]
                //onCurrentIndexChanged: ctrl.criteria = currentIndex
            }
            Label {
                text: qsTr("Pattern")
            }
            TextField {
                id: textField
                Layout.fillWidth: true
            }
        }

        ListView {
            id: _list
            model: ctrl.model
            Layout.fillWidth: true
            clip: true
            Layout.fillHeight: true
            delegate:  GridLayout {
                columns: 4
                width:  _list.width

                property QtObject item:  object
                Image {
                    Layout.column: 0
                    source: hasPicture ? "image://nodes/%1".arg(item.id) : ""
                    sourceSize.width: 100
                    sourceSize.height: 100
                    Layout.preferredWidth: 100
                    fillMode: Image.PreserveAspectFit
                    Layout.rowSpan: 2
                }
                TextField {
                    Layout.column: 1
                    text: item.text
                    onEditingFinished: item.text = text
                    Layout.fillWidth: true
                }
                TextArea {
                    Layout.column: 2
                    text: item.description
                    onEditingFinished: item.description = text
                    Layout.rowSpan: 2
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
                CheckBox {
                    Layout.column: 3
                    onCheckedChanged:{
                        if(checked)
                            MainController.selectionCtrl.addToSelection(item)
                        else
                            MainController.selectionCtrl.removeFromSelection(item)
                    }
                }
                TextField {
                    text: item.tagsText
                    onEditingFinished: item.tagsText = text
                    Layout.fillWidth: true
                    Layout.column: 1
                    Layout.row :1
                }

                ToolButton {
                    text: "apply to all"
                    Layout.column: 3
                    Layout.row :1
                }
                Rectangle {
                    Layout.preferredHeight: 3
                    Layout.column: 0
                    Layout.row :2
                    Layout.columnSpan: 4
                    Layout.fillWidth: true
                    color: Theme.nightMode ? "white" : "black"
                }
            }

        }

        /*Label {
            id: logs
            Layout.fillHeight: true
            Layout.fillWidth: true
        }*/
    }
}
