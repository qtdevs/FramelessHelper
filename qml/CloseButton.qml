import QtQuick 2.9
import QtQuick.Controls 2.2

Button {
    id: control

    width: 45
    height: 26

    ToolTip.visible: hovered && !down
    ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
    ToolTip.text: qsTr("Close")

    contentItem: Item {
        Image {
            anchors.centerIn: parent
            source: control.down || control.hovered ? "qrc:/res/close-button2.png" : "qrc:/res/close-button1.png"
        }
    }

    background: Rectangle {
        visible: control.down || control.hovered
        color: control.down ? "#8c0a15" : (control.hovered ? "#e81123" : "transparent")
    }
}
