import QtQuick 2.6
import QtQuick.Window 2.2
import QtShark.Window 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("FramelessHelper")

    flags: Qt.FramelessWindowHint

    FramelessHelper {
        id: framelessHelper

        Component.onCompleted: {
            addTitleBar(titleBar)
            setTitleBarHeight(60)
        }
    }

    Rectangle {
        id: titleBar
        anchors.fill: parent

        color: "yellow"
        border {
            width: 2
            color: "blue"
        }
    }
}
