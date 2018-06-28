import QtQuick 2.9
import QtQuick.Controls 2.2

Button {
    id: control

    width: 45
    height: 26

    ToolTip.visible: hovered && !down
    ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
    ToolTip.text: qsTr("Minimize")

    contentItem: Item {
        Image {
            anchors.centerIn: parent
            source: "qrc:/res/minimize-button1.png"
        }
    }

    background: Rectangle {
        visible: control.down || control.hovered
        color: control.down ? "#80808080" : (control.hovered ? "#80c7c7c7" : "transparent")
    }
}
