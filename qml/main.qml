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

        Component.onCompleted: {
            addIncludeItem(titleBar)
            setTitleBarHeight(60)
            addExcludeItem(controls)
        }
    }

    Image {
        anchors.fill: parent
        source: "qrc:/res/background.png"
    }

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
                window.showMinimized();
            }
        }
        MaximizeButton {
            isMaximized: Window.Maximized === window.visibility
            onClicked: {
                if (Window.Maximized === window.visibility) {
                    window.showNormal();
                } else {
                    window.showMaximized();
                }
            }
        }
        CloseButton {
            onClicked: {
                window.close();
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
