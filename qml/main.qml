import QtQuick 2.9
import QtQuick.Window 2.2
import QtShark.Window 1.0

Window {
    id: window
    visible: true
    width: 400
    height: 300
    title: qsTr("Qt Quick Inside")

    flags: Qt.Window | Qt.FramelessWindowHint

    FramelessHelper {
        id: framelessHelper

        titleBarHeight: 60

        Component.onCompleted: {
            addIncludeItem(titleBar)
            addExcludeItem(controls)
        }
    }

    Image {
        anchors.fill: parent
        source: "qrc:/res/background.png"
    }

    /*
    Rectangle {
        anchors {
            fill: parent
        }

        color: "transparent"

        border {
            width: 1
            color: "red"
        }
    }

    Rectangle {
        anchors {
            fill: parent
            margins: 4
        }

        color: "transparent"

        border {
            width: 1
            color: "blue"
        }
    }
    */

    Item {
        id: titleBar
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        height: 26
    }

    Row {
        id: controls
        anchors {
            top: titleBar.top
            right: titleBar.right
        }

        MinimizeButton {
            onClicked: {
                framelessHelper.triggerMinimizeButtonAction();
            }
        }
        MaximizeButton {
            maximized: Window.Maximized === window.visibility
            onClicked: {
                framelessHelper.triggerMaximizeButtonAction();
            }
        }
        CloseButton {
            onClicked: {
                framelessHelper.triggerCloseButtonAction();
            }
        }
    }

    Text {
        anchors {
            top: titleBar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        text: "Qt Quick Inside"
        font {
            family: "Arial"
            pointSize: 28
        }
        color: "#fefefe"

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    Component.onCompleted: {
        window.x = Screen.width / 2 - 410;
    }
}
