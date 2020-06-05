import QtQuick 2.9
import QtQuick.Window 2.2
import QtShark.Window 1.0
import QtQuick.Controls 2.5

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


    Text {
        id: txt
        anchors.horizontalCenter: parent.horizontalCenter

        anchors.verticalCenter: parent.verticalCenter

        text: "Qt Quick Inside"
        font {
            family: "Arial"
            pointSize: 28
        }
        color: "#fefefe"

    }

    Row {
        id: colorPicker
        width: parent.width
        anchors.top: txt.bottom
        anchors.topMargin: 20
        Slider {
            id: sr
            from: 0
            to: 1
            value: 0
            width: (parent.width - 20)/3
        }
        Slider {
            id: sg
            from: 0
            to: 1
            value: 0
            width: (parent.width - 20)/3

        }
        Slider {
            id: sb
            from: 0
            to: 1
            value: 0
            width: (parent.width - 20)/3

        }

    }


    CustomTitleBar {
        id: titleBar
    }

    WindowBorder {
        foregroundColor: Qt.rgba(sr.value,sg.value,sb.value,1)
    }

    ThreeButtons {
        id: controls
        helper: framelessHelper
        color: Qt.rgba(sr.value,sg.value,sb.value,1)
    }

    Component.onCompleted: {
        window.x = Screen.width / 2 - 410;
    }
}
