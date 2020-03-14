import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.3
import QtGraphicalEffects 1.0

Button {
    id: control

    width: 45
    height: 26

    property string color: "black"
    property string colorHoverd: "white"
    property string colorPressed: "white"
    property string colorInactive: "#969696"

    property string bgcolorPressed: "#80808080"
    property string bgcolorHoverd: "#80c7c7c7"
    property string bgcolor: "transparent"

    property bool changeColor: true
    property bool active: changeColor ? Window.active : true
    property bool maximized: Window.Maximized === Window.visibility

    contentItem: Item {
        Image {
            id: img
            width : 10
            height: 10
            visible: false
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
            source: maximized ? "qrc:/res/maximize-button2.svg" : "qrc:/res/maximize-button1.svg"
        }

        ColorOverlay {
            id: overLay
            anchors.fill: img
            source: img
            cached: true
            color: (control.down || control.hovered) ? control.colorHoverd:
                       (control.active? control.color: control.colorInactive)

        }
    }

    background: Rectangle {
        visible: control.down || control.hovered
        color: control.down ? control.bgcolorPressed : (control.hovered ? control.bgcolorHoverd : control.bgcolor)
    }

    ToolTip.visible: hovered && !down
    ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
    ToolTip.text: qsTr("Maximize")
}

